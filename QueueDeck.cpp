#include "QueueDeck.h"
#include <filesystem>
#include <sstream>
#include <fstream>

BAKKESMOD_PLUGIN(QueueDeck, "Full control over matchmaking via commands.", "2.9", PERMISSION_ALL)

enum EPlaylistRows
{
	Casual_Standard = 0,
	Casual_Doubles = 1,
	Casual_Duels = 2,
	Casual_Chaos = 3,
	Ranked_Standard = 4,
	Ranked_Doubles = 5,
	Ranked_Duels = 6,
	Extras_Rumble = 12,
	Extras_Dropshot = 13,
	Extras_Hoops = 14,
	Extras_Snowday = 15,
	Playlists_Max = 16
};

enum ERegionRows
{
	US_East = 0,
	Europe = 1,
	US_Weast = 2,
	Asia_Mainland = 3,
	Asia_Martitime = 4,
	Asia_East = 5,
	Middle_East = 6,
	Oceania = 7,
	South_Africa = 8,
	South_America = 9,
	Regions_Max = 10,
};

void QueueDeck::checkVersion()
{
	std::filesystem::path bakkesModPath = gameWrapper->GetBakkesModPath();
	bakkesModPath.append("version.txt");

	if (std::filesystem::exists(bakkesModPath))
	{
		std::ifstream versionFile(std::filesystem::absolute(bakkesModPath));
		std::string version;

		versionFile.seekg(0, std::ios::end);
		version.reserve(versionFile.tellg());
		versionFile.seekg(0, std::ios::beg);
		version.assign((std::istreambuf_iterator<char>(versionFile)), std::istreambuf_iterator<char>());

		if (version == localVersion)
		{
			versionSafe = true;
		}
		else
		{
			versionSafe = false;
		}
	}
	else
	{
		versionSafe = false;
	}
}

bool QueueDeck::areGObjectsValid()
{
	if (GObjects->Count > 0 && GObjects->Max > GObjects->Num())
	{
		if (std::string(UObject::GObjObjects()->Data[0]->GetFullName()) == "Class Core.Config_ORS")
			return true;
	}

	return false;
}

bool QueueDeck::areGNamesValid()
{
	if (GNames->Count > 0 && GNames->Max > GNames->Num())
		return true;

	return false;
}

void QueueDeck::onLoad()
{
	checkVersion();

	if (!versionSafe)
	{
		cvarManager->log("This plugin has not been verified to work with the latest BakkesMod version. If you are on the latest plugin version that means I already know so stop messaging me on twitter.");
		return;
	}

	// Yeah I know, but I'm too lazy to make a pattern that works for both platforms.
	if (gameWrapper->IsUsingSteamVersion())
	{
		uintptr_t BaseAddress = (uintptr_t)GetModuleHandle(NULL);
		uintptr_t GObjectsAddress = BaseAddress + STEAMGObjects_Offset;
		uintptr_t GNamesAddress = BaseAddress + STEAMGNames_Offset;
		GObjects = (TArray<UObject*>*)GObjectsAddress;
		GNames = (TArray<FNameEntry*>*)GNamesAddress;
	}
	else
	{
		uintptr_t BaseAddress = (uintptr_t)GetModuleHandle(NULL);
		uintptr_t GObjectsAddress = BaseAddress + EPICGObjects_Offset;
		uintptr_t GNamesAddress = BaseAddress + EPICGNames_Offset;
		GObjects = (TArray<UObject*>*)GObjectsAddress;
		GNames = (TArray<FNameEntry*>*)GNamesAddress;
	}

	if (areGObjectsValid() && areGNamesValid())
	{
		loadInstances();

		if (!classesSafe)
		{
			cvarManager->log("(onLoad) Error: RLSDK classes are wrong, please contact ItsBranK if he doesn't already know!");
			return;
		}

		cvarManager->registerNotifier("queue_search", [this](std::vector<std::string> params) { search(); }, "Start searching for your last selected playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_cancel", [this](std::vector<std::string> params) { cancel(); }, "Cancel searching.", PERMISSION_ALL);

		cvarManager->registerNotifier("queue_select_regions", [this](std::vector<std::string> params) { selectAllRegions(); }, "Selects all regions.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_playlists", [this](std::vector<std::string> params) { selectAllPlaylists(); }, "Selects all playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_casual", [this](std::vector<std::string> params) { selectCasuals(); }, "Selects all casual playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_ranked", [this](std::vector<std::string> params) { selectRanked(); }, "Selects all normal ranked playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_extras", [this](std::vector<std::string> params) { selectExtras(); }, "Selects all extras ranked playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_regions", [this](std::vector<std::string> params) { deselectAllRegions(); }, "Deselects all regions.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_playlists", [this](std::vector<std::string> params) { deselectAllPlaylists(); }, "Deselects all playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_casual", [this](std::vector<std::string> params) { deselectCasuals(); }, "Deselects all casual playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_ranked", [this](std::vector<std::string> params) { deselectRanked(); }, "Deselects all normal ranked playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_extras", [this](std::vector<std::string> params) { deselectExtras(); }, "Deselects all extras playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_view_casual", [this](std::vector<std::string> params) { selectViewTab((unsigned char)EMatchmakingViewTab::MatchmakingViewTab_Unranked); }, "Selects your view tab to casual playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_view_ranked", [this](std::vector<std::string> params) { selectViewTab((unsigned char)EMatchmakingViewTab::MatchmakingViewTab_Ranked); }, "Selects your view tab to ranked playlists.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_view_extras", [this](std::vector<std::string> params) { selectViewTab((unsigned char)EMatchmakingViewTab::MatchmakingViewTab_RankedSports); }, "Selects your view tab to extras playlists.", PERMISSION_ALL);

		cvarManager->registerNotifier("queue_select_cstandard", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Casual_Standard); }, "Selects the casual standard playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_cdoubles", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Casual_Doubles); }, "Selects the casual doubles playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_cduels", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Casual_Duels); }, "Selects the casual duels playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_cchaos", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Casual_Chaos); }, "Selects the casual chaos playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_rstandard", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Ranked_Standard); }, "Selects the ranked standard playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_rdoubles", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Ranked_Doubles); }, "Selects the ranked doubles playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_rduels", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Ranked_Duels); }, "Selects the ranked duels playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_erumble", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Extras_Rumble); }, "Selects the extras rumble playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_edropshot", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Extras_Dropshot); }, "Selects the extras dropshot playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_ehoops", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Extras_Hoops); }, "Selects the extras hoops playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_esnowday", [this](std::vector<std::string> params) { selectPlaylist(EPlaylistRows::Extras_Snowday); }, "Selects the extras snowday playlist.", PERMISSION_ALL);

		cvarManager->registerNotifier("queue_deselect_cstandard", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Casual_Standard); }, "Deselects the casual standard playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_cdoubles", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Casual_Doubles); }, "Deselects the casual doubles playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_cduels", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Casual_Duels); }, "Deselects the casual duels playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_cchaos", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Casual_Chaos); }, "Deselects the casual chaos playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_rstandard", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Ranked_Standard); }, "Deselects the ranked standard playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_rdoubles", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Ranked_Doubles); }, "Deselects the ranked doubles playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_rduels", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Ranked_Duels); }, "Deselects the ranked duels playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_erumble", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Extras_Rumble); }, "Deselects the extras rumble playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_edropshot", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Extras_Dropshot); }, "Deselects the extras dropshot playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_ehoops", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Extras_Hoops); }, "Deselects the ranked extras playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_esnowday", [this](std::vector<std::string> params) { deselectPlaylist(EPlaylistRows::Extras_Snowday); }, "Deselects the extras snowday playlist.", PERMISSION_ALL);

		cvarManager->registerNotifier("queue_search_cstandard", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Casual_Standard); }, "Starts searching in only the casual standard playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_cdoubles", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Casual_Doubles); }, "Starts searching in only the casual doubles playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_cduels", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Casual_Duels); }, "Starts searching in only the casual duels playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_cchaos", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Casual_Chaos); }, "Starts searching in only the casual chaos playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_rstandard", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Ranked_Standard); }, "Starts searching in only the ranked standard playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_rdoubles", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Ranked_Doubles); }, "Starts searching in only the ranked doubles playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_rduels", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Ranked_Duels); }, "Starts searching in only the ranked duels playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_erumble", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Extras_Rumble); }, "Starts searching in only the extras rumble playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_edropshot", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Extras_Dropshot); }, "Starts searching in only the extras dropshot playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_ehoops", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Extras_Hoops); }, "Starts searching in only the extras hoops playlist.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_search_esnowday", [this](std::vector<std::string> params) { searchPlaylist(EPlaylistRows::Extras_Snowday); }, "Starts searching in only the extras snowday playlist.", PERMISSION_ALL);

		cvarManager->registerNotifier("queue_select_rec", [this](std::vector<std::string> params) { deselectAllRegions(); }, "Selects the Recommended region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_use", [this](std::vector<std::string> params) { selectRegion(ERegionRows::US_East); }, "Selects the US-East region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_eu", [this](std::vector<std::string> params) { selectRegion(ERegionRows::Europe); }, "Selects the Europe region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_usw", [this](std::vector<std::string> params) { selectRegion(ERegionRows::US_Weast); }, "Selects the US-Weast region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_aml", [this](std::vector<std::string> params) { selectRegion(ERegionRows::Asia_Mainland); }, "Selects the Asia-SE Mainland region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_amt", [this](std::vector<std::string> params) { selectRegion(ERegionRows::Asia_Martitime); }, "Selects the Asia-SE Martitime region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_ae", [this](std::vector<std::string> params) { selectRegion(ERegionRows::Asia_East); }, "Selects the Asia-East region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_me", [this](std::vector<std::string> params) { selectRegion(ERegionRows::Middle_East); }, "Selects the Middle-East region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_oce", [this](std::vector<std::string> params) { selectRegion(ERegionRows::Oceania); }, "Selects the Oceania region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_saf", [this](std::vector<std::string> params) { selectRegion(ERegionRows::South_Africa); }, "Selects the South Africa region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_select_sam", [this](std::vector<std::string> params) { selectRegion(ERegionRows::South_America); }, "Selects the South America region.", PERMISSION_ALL);

		cvarManager->registerNotifier("queue_deselect_use", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::US_East); }, "Deselects the US-East region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_eu", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::Europe); }, "Deselects the Europe region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_usw", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::US_Weast); }, "Deselects the US-Weast region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_aml", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::Asia_Mainland); }, "Deselects the Asia-SE Mainland region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_amt", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::Asia_Martitime); }, "Deselects the Asia-SE Martitime region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_ae", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::Asia_East); }, "Deselects the Asia-East region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_me", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::Middle_East); }, "Deselects the Middle-East region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_oce", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::Oceania); }, "Deselects the Oceania region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_saf", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::South_Africa); }, "Deselects the South Africa region.", PERMISSION_ALL);
		cvarManager->registerNotifier("queue_deselect_sam", [this](std::vector<std::string> params) { deselectRegion(ERegionRows::South_America); }, "Deselects the South America region.", PERMISSION_ALL);

		gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.OnMatchWinnerSet", [this](std::string eventName) { setCanSearch(true); });
		gameWrapper->HookEvent("Function TAGame.Team_TA.PostBeginPlay", [this](std::string eventName) { setCanSearch(false); });
	}
	else
	{
		cvarManager->log("(onLoad) Error: RLSDK classes are wrong, please contact ItsBranK if he doesn't already know!");
	}
}

void QueueDeck::onUnload()
{
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.OnMatchWinnerSet");
	gameWrapper->UnhookEvent("Function TAGame.Team_TA.PostBeginPlay");
}

void QueueDeck::loadInstances()
{
	classInstances.emplace("Core.Object", 0);
	classInstances.emplace("Core.Class", 0);
	classInstances.emplace("Core.Function", 0);
	classInstances.emplace("TAGame.GFxData_Matchmaking_TA", 0);

	functionInstances.emplace("GFxData_Matchmaking_TA.StartMatchmaking", 0);
	functionInstances.emplace("GFxData_Matchmaking_TA.CancelSearch", 0);
	functionInstances.emplace("GFxData_Matchmaking_TA.SetMatchmakingViewTab", 0);
	functionInstances.emplace("GFxData_Matchmaking_TA.SetPlaylistSelection", 0);
	functionInstances.emplace("GFxData_Matchmaking_TA.SetRegionSelection", 0);
	functionInstances.emplace("GFxData_Matchmaking_TA.UpdateSelectedRegions", 0);

	for (int i = 0; i < UObject::GObjObjects()->Num(); i++)
	{
		UObject* object = UObject::GObjObjects()->Data[i];

		if (object && object->Outer)
		{
			std::string objectName = std::string(object->Outer->GetName()) + "." + std::string(object->GetName());

			std::map<std::string, int>::iterator classIt = classInstances.find(objectName);
			std::map<std::string, int>::iterator functionIt = functionInstances.find(objectName);

			if (classIt != classInstances.end())
				classInstances[objectName] = object->ObjectInternalInteger;

			if (functionIt != functionInstances.end())
				functionInstances[objectName] = object->ObjectInternalInteger;
		}
	}

	UObject::StaticClass(classInstances["Core.Object"]);
	UClass::StaticClass(classInstances["Core.Class"]);
	UFunction::StaticClass(classInstances["Core.Function"]);
	UGFxData_Matchmaking_TA::StaticClass(classInstances["TAGame.GFxData_Matchmaking_TA"]);

	pFnStartMatchmaking = (UFunction*)UObject::GObjObjects()->Data[functionInstances["GFxData_Matchmaking_TA.StartMatchmaking"]];
	pFnCancelSearch = (UFunction*)UObject::GObjObjects()->Data[functionInstances["GFxData_Matchmaking_TA.CancelSearch"]];
	pFnSetMatchmakingViewTab = (UFunction*)UObject::GObjObjects()->Data[functionInstances["GFxData_Matchmaking_TA.SetMatchmakingViewTab"]];
	pFnSetPlaylistSelection = (UFunction*)UObject::GObjObjects()->Data[functionInstances["GFxData_Matchmaking_TA.SetPlaylistSelection"]];
	pFnSetRegionSelection = (UFunction*)UObject::GObjObjects()->Data[functionInstances["GFxData_Matchmaking_TA.SetRegionSelection"]];
	pFnUpdateSelectedRegions = (UFunction*)UObject::GObjObjects()->Data[functionInstances["GFxData_Matchmaking_TA.UpdateSelectedRegions"]];
	matchmaking = GetInstanceOf<UGFxData_Matchmaking_TA>();

	if (UObject::StaticClass()
		&& UClass::StaticClass()
		&& UFunction::StaticClass()
		&& UGFxData_Matchmaking_TA::StaticClass()
		&& pFnStartMatchmaking
		&& pFnCancelSearch
		&& pFnSetMatchmakingViewTab
		&& pFnSetPlaylistSelection
		&& pFnSetRegionSelection
		&& pFnUpdateSelectedRegions
		&& matchmaking)
	{
		classesSafe = true;
	}
	else
	{
		classesSafe = false;
	}
}

void QueueDeck::setCanSearch(bool search)
{
	canSearch = search;
}

bool QueueDeck::isPlaylistCasual(int playlist)
{
	if (playlist == EPlaylistRows::Casual_Standard
		|| playlist == EPlaylistRows::Casual_Doubles
		|| playlist == EPlaylistRows::Casual_Duels
		|| playlist == EPlaylistRows::Casual_Chaos) {
		return true;
	}
	return false;
}

bool QueueDeck::isPlaylistRanked(int playlist)
{
	if (playlist == EPlaylistRows::Ranked_Standard
		|| playlist == EPlaylistRows::Ranked_Doubles
		|| playlist == EPlaylistRows::Ranked_Duels) {
		return true;
	}
	return false;
}

bool QueueDeck::isPlaylistExtras(int playlist)
{
	if (playlist == EPlaylistRows::Extras_Rumble
		|| playlist == EPlaylistRows::Extras_Dropshot
		|| playlist == EPlaylistRows::Extras_Hoops
		|| playlist == EPlaylistRows::Extras_Snowday) {
		return true;
	}
	return false;
}

void QueueDeck::search()
{
	if (!gameWrapper->IsInOnlineGame())
	{
		StartMatchmaking(matchmaking);
	}
	else
	{
		if (canSearch)
		{
			StartMatchmaking(matchmaking);
		}
		else
		{
			cvarManager->log("Error: You cannot search during an online game!");
		}
	}
}

void QueueDeck::cancel()
{
	CancelSearch(matchmaking);
}

void QueueDeck::selectAllRegions()
{
	for (int i = 0; i < ERegionRows::Regions_Max; i++)
		SetRegionSelection(matchmaking, i, true);

	UpdateSelectedRegions(matchmaking);
}

void QueueDeck::selectAllPlaylists()
{
	for (int i = 0; i < EPlaylistRows::Playlists_Max; i++)
		SetPlaylistSelection(matchmaking, i, true);
}

void QueueDeck::selectCasuals()
{
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Standard, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Doubles, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Duels, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Chaos, true);
}

void QueueDeck::selectRanked()
{
	SetPlaylistSelection(matchmaking, EPlaylistRows::Ranked_Standard, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Ranked_Doubles, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Ranked_Duels, true);
}

void QueueDeck::selectExtras()
{
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Rumble, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Dropshot, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Hoops, true);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Snowday, true);
}

void QueueDeck::selectViewTab(int tab)
{
	SetMatchmakingViewTab(matchmaking, tab);
}

void QueueDeck::deselectAllRegions()
{
	for (int i = 0; i < ERegionRows::Regions_Max; i++)
		SetRegionSelection(matchmaking, i, false);

	UpdateSelectedRegions(matchmaking);
}

void QueueDeck::deselectAllPlaylists(){
	for (int i = 0; i < EPlaylistRows::Playlists_Max; i++)
		SetPlaylistSelection(matchmaking, i, false);
}

void QueueDeck::deselectCasuals()
{
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Standard, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Doubles, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Duels, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Casual_Chaos, false);
}

void QueueDeck::deselectRanked()
{
	SetPlaylistSelection(matchmaking, EPlaylistRows::Ranked_Standard, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Ranked_Doubles, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Ranked_Duels, false);
}

void QueueDeck::deselectExtras()
{
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Rumble, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Dropshot, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Hoops, false);
	SetPlaylistSelection(matchmaking, EPlaylistRows::Extras_Snowday, false);
}

void QueueDeck::selectRegion(int region)
{
	SetRegionSelection(matchmaking, region, true);
	UpdateSelectedRegions(matchmaking);
}

void QueueDeck::selectPlaylist(int playlist)
{
	SetPlaylistSelection(matchmaking, playlist, true);
}


void QueueDeck::deselectPlaylist(int playlist)
{
	SetPlaylistSelection(matchmaking, playlist, false);
}

void QueueDeck::deselectRegion(int region)
{
	SetRegionSelection(matchmaking, region, false);
	UpdateSelectedRegions(matchmaking);
}

void QueueDeck::searchPlaylist(int playlist)
{
	deselectAllPlaylists();
	// Since Pysonix decided you can't search casual and ranked playlists at the same time, we have to set the view tab first internally then search
	if (isPlaylistCasual(playlist))
	{
		SetMatchmakingViewTab(matchmaking, static_cast<unsigned char>(EMatchmakingViewTab::MatchmakingViewTab_Unranked));
	}
	else if (isPlaylistRanked(playlist))
	{
		SetMatchmakingViewTab(matchmaking, static_cast<unsigned char>(EMatchmakingViewTab::MatchmakingViewTab_Ranked));
	}
	else if (isPlaylistExtras(playlist))
	{
		SetMatchmakingViewTab(matchmaking, static_cast<unsigned char>(EMatchmakingViewTab::MatchmakingViewTab_RankedSports));
	}

	selectPlaylist(playlist);
	search();
}

void QueueDeck::StartMatchmaking(UGFxData_Matchmaking_TA* caller)
{
	UGFxData_Matchmaking_TA_execStartMatchmaking_Parms StartMatchmaking_Parms;

	caller->ProcessEvent(pFnStartMatchmaking, &StartMatchmaking_Parms, nullptr);
}

void QueueDeck::CancelSearch(UGFxData_Matchmaking_TA* caller)
{
	UGFxData_Matchmaking_TA_execCancelSearch_Parms CancelSearch_Parms;

	caller->ProcessEvent(pFnCancelSearch, &CancelSearch_Parms, nullptr);
}

void QueueDeck::SetRegionSelection(UGFxData_Matchmaking_TA* caller, int Row, unsigned long bSelected)
{
	UGFxData_Matchmaking_TA_execSetRegionSelection_Parms SetRegionSelection_Parms;
	memcpy(&SetRegionSelection_Parms.Row, &Row, 0x4);
	SetRegionSelection_Parms.bSelected = bSelected;

	caller->ProcessEvent(pFnSetRegionSelection, &SetRegionSelection_Parms, nullptr);
}

void QueueDeck::UpdateSelectedRegions(UGFxData_Matchmaking_TA* caller)
{
	UGFxData_Matchmaking_TA_execUpdateSelectedRegions_Parms UpdateSelectedRegions_Parms;

	caller->ProcessEvent(pFnUpdateSelectedRegions, &UpdateSelectedRegions_Parms, nullptr);
}

void QueueDeck::SetPlaylistSelection(UGFxData_Matchmaking_TA* caller, int Row, unsigned long bSelected)
{
	UGFxData_Matchmaking_TA_execSetPlaylistSelection_Parms SetPlaylistSelection_Parms;
	memcpy(&SetPlaylistSelection_Parms.Row, &Row, 0x4);
	SetPlaylistSelection_Parms.bSelected = bSelected;

	caller->ProcessEvent(pFnSetPlaylistSelection, &SetPlaylistSelection_Parms, nullptr);
}

void QueueDeck::SetMatchmakingViewTab(UGFxData_Matchmaking_TA* caller, unsigned char InMatchmakingViewTab)
{
	UGFxData_Matchmaking_TA_execSetMatchmakingViewTab_Parms SetMatchmakingViewTab_Parms;
	SetMatchmakingViewTab_Parms.InMatchmakingViewTab = InMatchmakingViewTab;

	caller->ProcessEvent(pFnSetMatchmakingViewTab, &SetMatchmakingViewTab_Parms, nullptr);
}
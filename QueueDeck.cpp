#include "QueueDeck.hpp"

BAKKESMOD_PLUGIN(QueueDeck, "Full control over matchmaking via commands.", "4.0", PERMISSION_ALL)

void QueueDeck::onLoad()
{
	cvarManager->registerNotifier("queue_search", [this](std::vector<std::string> params) { Search(); }, "Start searching for your last selected playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_cancel", [this](std::vector<std::string> params) { Cancel(); }, "Cancel searching.", PERMISSION_ALL);

	cvarManager->registerNotifier("queue_select_regions", [this](std::vector<std::string> params) { SelectAllRegions(); }, "Selects all regions.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_playlists", [this](std::vector<std::string> params) { SelectAllPlaylists(); }, "Selects all playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_casual", [this](std::vector<std::string> params) { SelectCasuals(); }, "Selects all casual playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_ranked", [this](std::vector<std::string> params) { SelectRanked(); }, "Selects all normal ranked playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_extras", [this](std::vector<std::string> params) { SelectExtras(); }, "Selects all extras ranked playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_regions", [this](std::vector<std::string> params) { DeselectAllRegions(); }, "Deselects all regions.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_playlists", [this](std::vector<std::string> params) { DeselectAllPlaylists(); }, "Deselects all playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_casual", [this](std::vector<std::string> params) { DeselectCasuals(); }, "Deselects all casual playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_ranked", [this](std::vector<std::string> params) { DeselectRanked(); }, "Deselects all normal ranked playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_view_casual", [this](std::vector<std::string> params) { SetViewTab(PlaylistCategory::CASUAL); }, "Selects your view tab to casual playlists.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_view_ranked", [this](std::vector<std::string> params) { SetViewTab(PlaylistCategory::RANKED); }, "Selects your view tab to ranked playlists.", PERMISSION_ALL);
	
	cvarManager->registerNotifier("queue_select_cstandard", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_STANDARD, true); }, "Selects the casual standard playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_cdoubles", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_DOUBLES, true); }, "Selects the casual doubles playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_cduels", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_DUELS, true); }, "Selects the casual duels playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_cchaos", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_CHAOS, true); }, "Selects the casual chaos playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_rstandard", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::RANKED_STANDARD, true); }, "Selects the ranked standard playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_rdoubles", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::RANKED_DOUBLES, true); }, "Selects the ranked doubles playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_rduels", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::RANKED_DUELS, true); }, "Selects the ranked duels playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_erumble", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_RUMBLE, true); }, "Selects the extras rumble playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_edropshot", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_DROPSHOT, true); }, "Selects the extras dropshot playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_ehoops", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_HOOPS, true); }, "Selects the extras hoops playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_esnowday", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_SNOWDAY, true); }, "Selects the extras snowday playlist.", PERMISSION_ALL);

	cvarManager->registerNotifier("queue_deselect_cstandard", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_STANDARD, false); }, "Deselects the casual standard playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_cdoubles", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_DOUBLES, false); }, "Deselects the casual doubles playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_cduels", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_DUELS, false); }, "Deselects the casual duels playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_cchaos", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::CASUAL_CHAOS, false); }, "Deselects the casual chaos playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_rstandard", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::RANKED_STANDARD, false); }, "Deselects the ranked standard playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_rdoubles", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::RANKED_DOUBLES, false); }, "Deselects the ranked doubles playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_rduels", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::RANKED_DUELS, false); }, "Deselects the ranked duels playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_erumble", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_RUMBLE, false); }, "Deselects the extras rumble playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_edropshot", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_DROPSHOT, false); }, "Deselects the extras dropshot playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_ehoops", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_HOOPS, false); }, "Deselects the ranked extras playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_esnowday", [this](std::vector<std::string> params) { SetPlaylistSelection(Playlist::EXTRAS_SNOWDAY, false); }, "Deselects the extras snowday playlist.", PERMISSION_ALL);

	cvarManager->registerNotifier("queue_search_cstandard", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::CASUAL_STANDARD); }, "Starts searching in only the casual standard playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_cdoubles", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::CASUAL_DOUBLES); }, "Starts searching in only the casual doubles playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_cduels", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::CASUAL_DUELS); }, "Starts searching in only the casual duels playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_cchaos", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::CASUAL_CHAOS); }, "Starts searching in only the casual chaos playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_rstandard", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::RANKED_STANDARD); }, "Starts searching in only the ranked standard playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_rdoubles", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::RANKED_DOUBLES); }, "Starts searching in only the ranked doubles playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_rduels", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::RANKED_DUELS); }, "Starts searching in only the ranked duels playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_erumble", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::EXTRAS_RUMBLE); }, "Starts searching in only the extras rumble playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_edropshot", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::EXTRAS_DROPSHOT); }, "Starts searching in only the extras dropshot playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_ehoops", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::EXTRAS_HOOPS); }, "Starts searching in only the extras hoops playlist.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_search_esnowday", [this](std::vector<std::string> params) { SearchPlaylist(Playlist::EXTRAS_SNOWDAY); }, "Starts searching in only the extras snowday playlist.", PERMISSION_ALL);

	cvarManager->registerNotifier("queue_select_rec", [this](std::vector<std::string> params) { DeselectAllRegions(); }, "Selects the Recommended region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_use", [this](std::vector<std::string> params) { SetRegionSelection(Region::USE, true); }, "Selects the US-East region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_eu", [this](std::vector<std::string> params) { SetRegionSelection(Region::EU, true); }, "Selects the Europe region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_usw", [this](std::vector<std::string> params) { SetRegionSelection(Region::USW, true); }, "Selects the US-Weast region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_aml", [this](std::vector<std::string> params) { SetRegionSelection(Region::ASC, true); }, "Selects the Asia-SE Mainland region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_amt", [this](std::vector<std::string> params) { SetRegionSelection(Region::ASM, true); }, "Selects the Asia-SE Martitime region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_ae", [this](std::vector<std::string> params) { SetRegionSelection(Region::JPN, true); }, "Selects the Asia-East region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_me", [this](std::vector<std::string> params) { SetRegionSelection(Region::ME, true); }, "Selects the Middle-East region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_oce", [this](std::vector<std::string> params) { SetRegionSelection(Region::OCE, true); }, "Selects the Oceania region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_saf", [this](std::vector<std::string> params) { SetRegionSelection(Region::SAF, true); }, "Selects the South Africa region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_sam", [this](std::vector<std::string> params) { SetRegionSelection(Region::SAM, true); }, "Selects the South America region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_select_ind", [this](std::vector<std::string> params) { SetRegionSelection(Region::IND, true); }, "Selects the India region.", PERMISSION_ALL);

	cvarManager->registerNotifier("queue_deselect_use", [this](std::vector<std::string> params) { SetRegionSelection(Region::USE, false); }, "Deselects the US-East region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_eu", [this](std::vector<std::string> params) { SetRegionSelection(Region::EU, false); }, "Deselects the Europe region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_usw", [this](std::vector<std::string> params) { SetRegionSelection(Region::USW, false); }, "Deselects the US-Weast region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_aml", [this](std::vector<std::string> params) { SetRegionSelection(Region::ASC, false); }, "Deselects the Asia-SE Mainland region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_amt", [this](std::vector<std::string> params) { SetRegionSelection(Region::ASM, false); }, "Deselects the Asia-SE Martitime region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_ae", [this](std::vector<std::string> params) { SetRegionSelection(Region::JPN, false); }, "Deselects the Asia-East region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_me", [this](std::vector<std::string> params) { SetRegionSelection(Region::ME, false); }, "Deselects the Middle-East region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_oce", [this](std::vector<std::string> params) { SetRegionSelection(Region::OCE, false); }, "Deselects the Oceania region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_saf", [this](std::vector<std::string> params) { SetRegionSelection(Region::SAF, false); }, "Deselects the South Africa region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_sam", [this](std::vector<std::string> params) { SetRegionSelection(Region::SAM, false); }, "Deselects the South America region.", PERMISSION_ALL);
	cvarManager->registerNotifier("queue_deselect_ind", [this](std::vector<std::string> params) { SetRegionSelection(Region::IND, false); }, "Deselects the India region.", PERMISSION_ALL);

	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", [this](std::string eventName) { SetCanSearch(true); });
	gameWrapper->HookEvent("Function TAGame.AchievementSystem_TA.CheckWonMatch", [this](std::string eventName) { SetCanSearch(true); });
	gameWrapper->HookEvent("Function TAGame.Team_TA.PostBeginPlay", [this](std::string eventName) { SetCanSearch(false); });
}

void QueueDeck::onUnload()
{
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed");
	gameWrapper->UnhookEvent("Function TAGame.AchievementSystem_TA.CheckWonMatch");
	gameWrapper->UnhookEvent("Function TAGame.Team_TA.PostBeginPlay");
}

void QueueDeck::SetCanSearch(bool search)
{
	CanSearch = search;
}

bool QueueDeck::IsPlaylistCasual(Playlist playlist)
{
	return ((playlist == Playlist::CASUAL_STANDARD) || (playlist == Playlist::CASUAL_DOUBLES) || (playlist == Playlist::CASUAL_DUELS) || (playlist == Playlist::CASUAL_CHAOS));
}

bool QueueDeck::IsPlaylistRanked(Playlist playlist)
{
	return ((playlist == Playlist::RANKED_STANDARD) || (playlist == Playlist::RANKED_DOUBLES) || (playlist == Playlist::RANKED_DUELS));
}

void QueueDeck::Search()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		if (!gameWrapper->IsInOnlineGame())
		{
			mmw.StartMatchmaking(static_cast<PlaylistCategory>(mmw.GetActiveViewTab()));
		}
		else
		{
			if (CanSearch)
			{
				mmw.StartMatchmaking(static_cast<PlaylistCategory>(mmw.GetActiveViewTab()));
			}
			else
			{
				cvarManager->log("Error: You cannot search during an online game!");
			}
		}	
	}
}

void QueueDeck::Cancel()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.CancelMatchmaking();
	}
}

void QueueDeck::SelectAllPlaylists()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(Playlist::CASUAL_STANDARD, true);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DOUBLES, true);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DUELS, true);
		mmw.SetPlaylistSelection(Playlist::CASUAL_CHAOS, true);
		mmw.SetPlaylistSelection(Playlist::RANKED_STANDARD, true);
		mmw.SetPlaylistSelection(Playlist::RANKED_DOUBLES, true);
		mmw.SetPlaylistSelection(Playlist::RANKED_DUELS, true);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_RUMBLE, true);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_DROPSHOT, true);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_HOOPS, true);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_SNOWDAY, true);
	}
}

void QueueDeck::SelectAllRegions()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetRegionSelection(Region::USE, true);
		mmw.SetRegionSelection(Region::EU, true);
		mmw.SetRegionSelection(Region::USW, true);
		mmw.SetRegionSelection(Region::ASC, true);
		mmw.SetRegionSelection(Region::ASM, true);
		mmw.SetRegionSelection(Region::JPN, true);
		mmw.SetRegionSelection(Region::ME, true);
		mmw.SetRegionSelection(Region::OCE, true);
		mmw.SetRegionSelection(Region::SAF, true);
		mmw.SetRegionSelection(Region::SAM, true);
	}
}

void QueueDeck::SelectCasuals()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(Playlist::CASUAL_STANDARD, true);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DOUBLES, true);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DUELS, true);
		mmw.SetPlaylistSelection(Playlist::CASUAL_CHAOS, true);
	}
}

void QueueDeck::SelectRanked()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(Playlist::RANKED_STANDARD, true);
		mmw.SetPlaylistSelection(Playlist::RANKED_DOUBLES, true);
		mmw.SetPlaylistSelection(Playlist::RANKED_DUELS, true);
	}
}

void QueueDeck::SelectExtras()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(Playlist::EXTRAS_RUMBLE, true);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_DROPSHOT, true);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_HOOPS, true);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_SNOWDAY, true);
	}
}

void QueueDeck::DeselectAllPlaylists()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(Playlist::CASUAL_STANDARD, false);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DOUBLES, false);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DUELS, false);
		mmw.SetPlaylistSelection(Playlist::CASUAL_CHAOS, false);
		mmw.SetPlaylistSelection(Playlist::RANKED_STANDARD, false);
		mmw.SetPlaylistSelection(Playlist::RANKED_DOUBLES, false);
		mmw.SetPlaylistSelection(Playlist::RANKED_DUELS, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_RUMBLE, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_DROPSHOT, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_HOOPS, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_SNOWDAY, false);
	}
}


void QueueDeck::DeselectAllRegions()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetRegionSelection(Region::USE, false);
		mmw.SetRegionSelection(Region::EU, false);
		mmw.SetRegionSelection(Region::USW, false);
		mmw.SetRegionSelection(Region::ASC, false);
		mmw.SetRegionSelection(Region::ASM, false);
		mmw.SetRegionSelection(Region::JPN, false);
		mmw.SetRegionSelection(Region::ME, false);
		mmw.SetRegionSelection(Region::OCE, false);
		mmw.SetRegionSelection(Region::SAF, false);
		mmw.SetRegionSelection(Region::SAM, false);
	}
}

void QueueDeck::DeselectCasuals()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(Playlist::CASUAL_STANDARD, false);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DOUBLES, false);
		mmw.SetPlaylistSelection(Playlist::CASUAL_DUELS, false);
		mmw.SetPlaylistSelection(Playlist::CASUAL_CHAOS, false);
	}
}

void QueueDeck::DeselectRanked()
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(Playlist::RANKED_STANDARD, false);
		mmw.SetPlaylistSelection(Playlist::RANKED_DOUBLES, false);
		mmw.SetPlaylistSelection(Playlist::RANKED_DUELS, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_RUMBLE, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_DROPSHOT, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_HOOPS, false);
		mmw.SetPlaylistSelection(Playlist::EXTRAS_SNOWDAY, false);
	}
}

void QueueDeck::SetViewTab(PlaylistCategory tab)
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetViewTab(tab);
	}
}

void QueueDeck::SetPlaylistSelection(Playlist playlist, bool bSelected)
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetPlaylistSelection(playlist, bSelected);
	}
}

void QueueDeck::SetRegionSelection(Region region, bool bSelected)
{
	MatchmakingWrapper mmw = gameWrapper->GetMatchmakingWrapper();

	if (mmw)
	{
		mmw.SetRegionSelection(region, bSelected);
	}
}

void QueueDeck::SearchPlaylist(Playlist playlist)
{
	DeselectAllPlaylists();

	// Since Pysonix decided you can't search casual and ranked playlists at the same time, we have to set the view tab first internally then search.

	if (IsPlaylistCasual(playlist))
	{
		SetViewTab(PlaylistCategory::CASUAL);
	}
	else if (IsPlaylistRanked(playlist))
	{
		SetViewTab(static_cast<PlaylistCategory>(7)); // New value is 7, bmsdk hasn't updated yet.
	}

	SetPlaylistSelection(playlist, true);
	Search();
}
#pragma once
#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "pluginsdk.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "RLSDK/SdkHeaders.h"
#include "RLSDK/Utils.h"

class QueueDeck : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::map<std::string, int> classInstances;
	std::map<std::string, int> functionInstances;

	UGFxData_Matchmaking_TA* matchmaking = nullptr; // Class TAGame.GFxData_Matchmaking_TA

	UFunction* pFnStartMatchmaking = nullptr; // Function TAGame.OnlineGameTourMatchmaking_TA.StartMatchmaking
	UFunction* pFnCancelSearch = nullptr; // Function TAGame.GFxData_PrivateMatch_TA.CancelSearch
	UFunction* pFnSetMatchmakingViewTab = nullptr; // Function TAGame.GFxData_Matchmaking_TA.SetMatchmakingViewTab
	UFunction* pFnSetPlaylistSelection = nullptr; // Function TAGame.GFxData_Matchmaking_TA.SetPlaylistSelection
	UFunction* pFnSetRegionSelection = nullptr; // Function TAGame.GFxData_Matchmaking_TA.SetRegionSelection
	UFunction* pFnUpdateSelectedRegions = nullptr; // Function TAGame.GFxData_Matchmaking_TA.UpdateSelectedRegions
private:
	std::string localVersion = "138";
	bool versionSafe = false;
	bool classesSafe = false;
	bool canSearch = true;
public:
	void checkVersion();
	bool areGObjectsValid();
	bool areGNamesValid();

	virtual void onLoad();
	virtual void onUnload();

	void loadInstances();
	void setCanSearch(bool search);
	bool isPlaylistCasual(int playlist);
	bool isPlaylistRanked(int playlist);
	bool isPlaylistExtras(int playlist);
public:
	void search();
	void cancel();

	void selectAllRegions();
	void selectAllPlaylists();
	void selectCasuals();
	void selectRanked();
	void selectExtras();
	void selectViewTab(int tab);
	void deselectAllRegions();
	void deselectAllPlaylists();
	void deselectCasuals();
	void deselectRanked();
	void deselectExtras();

	void selectRegion(int region);
	void selectPlaylist(int playlist);
	void deselectPlaylist(int playlist);
	void deselectRegion(int region);
	void searchPlaylist(int playlist);
public:
	void StartMatchmaking(UGFxData_Matchmaking_TA* caller);
	void CancelSearch(UGFxData_Matchmaking_TA* caller);
	void SetRegionSelection(UGFxData_Matchmaking_TA* caller, int Row, unsigned long bSelected);
	void UpdateSelectedRegions(UGFxData_Matchmaking_TA* caller);
	void SetPlaylistSelection(UGFxData_Matchmaking_TA* caller, int Row, unsigned long bSelected);
	void SetMatchmakingViewTab(UGFxData_Matchmaking_TA* caller, unsigned char InMatchmakingViewTab);
};
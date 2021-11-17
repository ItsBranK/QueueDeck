#pragma once
#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "pluginsdk.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/wrappers/MatchmakingWrapper.h"

class QueueDeck : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	PlaylistCategory SelectedViewTab = PlaylistCategory::CASUAL;
	bool CanSearch = true;

public:
	virtual void onLoad();
	virtual void onUnload();

	void SetCanSearch(bool search);
	bool IsPlaylistCasual(Playlist playlist);
	bool IsPlaylistRanked(Playlist playlist);
	bool IsPlaylistExtras(Playlist playlist);

public:
	void Search();
	void Cancel();

	void SelectAllRegions();
	void SelectAllPlaylists();
	void SelectCasuals();
	void SelectRanked();
	void SelectExtras();

	void DeselectAllRegions();
	void DeselectAllPlaylists();
	void DeselectCasuals();
	void DeselectRanked();
	void DeselectExtras();

	void SetViewTab(PlaylistCategory tab);
	void SetPlaylistSelection(Playlist playlist, bool bSelected);
	void SetRegionSelection(Region region, bool bSelected);
	void SearchPlaylist(Playlist playlist);
};
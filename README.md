## QueueDeck

This plugin gives you full control over matchmaking all through commands. This means you can bind these commands to certain hotkeys to select/deselect playlists, start/stop matchmaking, start searching in a specific playlist, and even change regions! All of these commands will work anywhere whether it be in freeplay or right after a match has ended.

### Example
Let's say you wanted to bind a combination of the commands from this plugin to a singular button on your keyboard.
In this example I'll show you how to search for both standard and doubles in casual automatically.

First you would create your own `example.cfg` file in `bakkesmod\cfg` and name it whatever you'd like.
In that config file you would put the following commands:

(Please note that all `Search Commands` do all the following below automatically, just making all possible combinations into commands wouldn't be feasible, so this is how you would do a completely custom version of selecting and queuing)
```
queue_view_casual // It's important to set the view tab first or searching will not work properly, you don't need to do this for "queue_search_cstandard" related commands as it does this automatically
queue_deselect_casual // Optional, this would deselect any playlists you had selected previously that you might have forgotten
queue_select_cstandard
queue_select_cdoubles
queue_search
```

This sets the view tab to casual, deselects all casual playlists, selects both standard and doubles, then starts searching for those playlists.

Now to bind that file to a button you would do this in the BakkesMod console.
```
bind NumPadOne "exec example.cfg";writeconfig
```
Ta-da! You now have you own custom queue config that's bound to the one button on your numpad.

### Commands

There's a grand total of 70 commands included in this plugin, yes, that's a lot; so here is a full list of them and what they do. I've organized them based on what they are related to, searching, playlists, and regions.

| Other Commands | Description |
| ------ | ------ |
| queue_view_casual | Selects your view tab to casual playlists. |
| queue_view_ranked | Selects your view tab to ranked playlists. |
| queue_view_extras | Selects your view tab to extras playlists. |

| Search Commands | Description |
| ------ | ------ |
| queue_search | Start searching for the playlists you last selected. |
| queue_cancel | Cancel searching. |
| queue_search_cstandard | Starts searching in only the casual standard playlist. |
| queue_search_cdoubles | Starts searching in only the casual doubles playlist. |
| queue_search_cduels | Starts searching in only the casual duels playlist. |
| queue_search_cchaos | Starts searching in only the casual chaos playlist. |
| queue_search_rstandard | Starts searching in only the ranked standard playlist. |
| queue_search_rdoubles | Starts searching in only the ranked doubles playlist. |
| queue_search_rduels | Starts searching in only the ranked duels playlist. |
| queue_search_erumble | Starts searching in only the extras rumble playlist. |
| queue_search_edropshot | Starts searching in only the extras dropshot playlist. |
| queue_search_ehoops | Starts searching in only the extras hoops playlist. |
| queue_search_esnowday | Starts searching in only the extras snowday playlist. |

| Playlist Commands | Description |
| ------ | ------ |
| queue_select_playlists | Selects all playlists. |
| queue_select_casual | Selects all casual playlists. |
| queue_select_ranked | Selects all normal ranked playlists. |
| queue_select_extras | Selects all extras ranked playlists. |
| queue_deselect_playlists | Deselects all playlists. |
| queue_deselect_casual | Deselects all casual playlists. |
| queue_deselect_ranked | Deselects all normal ranked playlists. |
| queue_deselect_extras | Deselects all extras playlists. |
| queue_select_cstandard | Selects the casual standard playlist. |
| queue_select_cdoubles | Selects the casual doubles playlist. |
| queue_select_cduels | Selects the casual duels playlist. |
| queue_select_cchaos | Selects the casual chaos playlist. |
| queue_select_rstandard | Selects the ranked standard playlist. |
| queue_select_rdoubles | Selects the ranked doubles playlist. |
| queue_select_rduels | Selects the ranked duels playlist. |
| queue_select_erumble | Selects the extras rumble playlist. |
| queue_select_edropshot | Selects the extras dropshot playlist. |
| queue_select_ehoops | Selects the extras hoops playlist. |
| queue_select_esnowday | Selects the extras snowday playlist. |
| queue_deselect_cstandard | Deselects the casual standard playlist. |
| queue_deselect_cdoubles | Deselects the casual doubles playlist. |
| queue_deselect_cduels | Deselects the casual duels playlist. |
| queue_deselect_cchaos | Deselects the casual chaos playlist. |
| queue_deselect_rstandard | Deselects the ranked standard playlist. |
| queue_deselect_rdoubles | Deselects the ranked doubles playlist. |
| queue_deselect_rduels | Deselects the ranked duels playlist. |
| queue_deselect_erumble | Deselects the extras rumble playlist. |
| queue_deselect_edropshot | Deselects the extras dropshot playlist. |
| queue_deselect_ehoops | Deselects the extras hoops playlist. |
| queue_deselect_esnowday | Deselects the extras snowday playlist. |

| Region Commands | Description |
| ------ | ------ |
| queue_select_regions | Selects all regions. |
| queue_deselect_regions | Deselects all regions. |
| queue_select_rec | Selects the Recommended region. |
| queue_select_use | Selects the US-East region. |
| queue_select_eu | Selects the Europe region. |
| queue_select_usw | Selects the US-Weast region. |
| queue_select_aml | Selects the Asia-SE Mainland region. |
| queue_select_amt | Selects the Asia-SE Martitime region. |
| queue_select_ae | Selects the Asia-East region. |
| queue_select_me | Selects the Middle-East region. |
| queue_select_oce | Selects the Oceania region. |
| queue_select_saf | Selects the South Africa region. |
| queue_select_sam | Selects the South America region. |
| queue_select_ind | Selects the India region. |
| queue_deselect_use | Deselects the US-East region. |
| queue_deselect_eu | Deselects the Europe region. |
| queue_deselect_usw | Deselects the US-Weast region. |
| queue_deselect_aml | Deselects the Asia-SE Mainland region. |
| queue_deselect_amt | Deselects the Asia-SE Martitime region. |
| queue_deselect_ae | Deselects the Asia-East region. |
| queue_deselect_me | Deselects the Middle-East region. |
| queue_deselect_oce | Deselects the Oceania region. |
| queue_deselect_saf | Deselects the South Africa region. |
| queue_deselect_sam | Deselects the South America region. |
| queue_deselect_ind | Deselects the India region. |
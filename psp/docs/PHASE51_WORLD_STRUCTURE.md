# Phase 51 official world structure

This is the high-level structure for the first complete Veylings game. It
reuses current region names where they fit and keeps the full world compact:
eight dense regions, each with a clear purpose. This phase defines no new map
IDs or tile layouts.

| Order | Official region | Current status | Type | Purpose and connections | Healing | Boss | Encounters | Progression role |
| ---: | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | **Hearth Clearing** | Existing `MAP_CLEARING`, to be rebuilt as a settlement | Starting settlement | Home, local services, first challenger, and the west end of the Fernveil Road. Connects to Wayfarer Lodge and Fernveil Woods. | Wayfarer Lodge | First challenger only | None in settlement | Start; clearing Ren's challenge opens Fernveil. |
| 2 | **Fernveil Woods** | Existing `MAP_FOREST`, to be rebuilt as East Forest | Forest wilderness | First true wilderness; the road from Hearth becomes a forest route. Connects Hearth, Sunthread Marsh, Hollowstone Cave, and Northwood Reach. | Fernveil Spring | Elder Sylva | Forest grass | First challenger victory permits entry; Sylva's victory opens Sunthread. |
| 3 | **Sunthread Marsh** | Existing `MAP_MARSH` | Wetland wilderness | The second major wilderness: reeds, streams, raised boardwalks, and the route to Lantern Rest. Connects Fernveil and Lantern Rest. | Sunthread Shrine | None in this act | Reeds | Opened by Elder Sylva; expands party and ecological variety. |
| 4 | **Lantern Rest** | Existing `MAP_REST`, to be rebuilt as a small settlement | Safe location | A compact waterside rest stop where travelers prepare for the northern route. Connects Sunthread Marsh and Northwood Reach. | Lantern Dais | None | None in settlement | Available after Sunthread; midpoint information and preparation hub. |
| 5 | **Northwood Reach** | Planned; currently only painted as `NORTHERN WOODS` on the world map | Highland wilderness | Wind-cut pine ridges above Lantern Rest. Connects Lantern Rest, Fernveil's northern trail, and the Hollowstone approach. | One field camp, planned | Warden Rune | Wind/Frost/Stone edge ecology | Rune's victory grants the cave clearance. |
| 6 | **Hollowstone Cave** | Existing `MAP_CAVE`, to be rebuilt as the cave system | Cave system | A naturally formed passage beneath Northwood Reach and Fernveil. Connects the northern route to the late-game region. | Hollowstone Camp | Story discovery, not a guardian battle by default | Cave terrain | Locked until Rune's victory; contains the midgame revelation. |
| 7 | **Glasswake Expanse** | Planned | Late-game wilderness | An exposed basin beyond the cave where altered Vey currents reshape the land. Connects Hollowstone Cave to the final location. | One limited safe point, planned | Late-game guardian, planned | Late-game mixed ecology | Opened after the cave revelation. |
| 8 | **The Stillward** | Planned | Final story location | A focused ancient site at the source of the world problem. Reached only from Glasswake Expanse. | No routine healing point | Final encounter, planned | No ordinary encounters by default | Final act and ending. |

## Existing support locations

- **Wayfarer Lodge** remains a purposeful interior in Hearth Clearing: shop,
  healing dais, and early travel advice. It is not a separate overworld region.
- **Fernveil Spring**, **Sunthread Shrine**, **Hollowstone Camp**, and the
  future Northwood field camp give the route a deliberate healing rhythm.

## Design constraints

- Each wilderness has a different traversal identity: canopy paths in
  Fernveil, boardwalk and water boundaries in Sunthread, elevation in
  Northwood, cave landmarks in Hollowstone, and exposed currents in Glasswake.
- No region exists solely to add distance. Every one contributes a new
  exploration choice, story beat, battle context, or recovery decision.
- The existing six map IDs remain valid save-compatible content. New map IDs
  will be appended later rather than replacing saved IDs.
- The precise physical links, elevation, rivers, and road layout are deferred
  to Phase 52.

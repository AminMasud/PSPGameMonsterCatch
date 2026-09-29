# Phase 60 Hearth Clearing layout

Hearth Clearing is the final compact starting settlement. It is a lived-in
trailhead on the sheltered west side of the valley, not a collection of empty
buildings. Phase 61 will replace the current prototype tiles from this plan.

## Footprint and route logic

Use the existing `MAP_CLEARING` 40×24 footprint for the production settlement.
The player begins in the south-central common, where the open ground makes the
eastbound route visible without forcing a tutorial corridor.

```text
                         NORTH
     [Protagonist Home]     garden / marker grove       [Wayfarer Lodge]
              |                    |                          |
     west hedgerow ---- village path + central common ---- lodge court
                                   |
                         well / quiet spring
                                   |
                       south path to local fields

                    Ren's trail post ---- east gate ---- Fernveil Road
```

The main path begins at the home doorway, passes the central common and lodge
court, then bends east toward Fernveil. The spring is visible from the path but
set one step aside, making it a deliberate recovery stop rather than an
obstacle. A low hedgerow, vegetable plots, and a small stream edge close the
west and south boundaries naturally; the east road is the only full wilderness
exit.

## Places and purposes

| Place | Approximate map zone | Purpose | Required production content |
| --- | --- | --- | --- |
| Protagonist Home | Northwest, west of the main path | Personal starting point and the only necessary private interior. | Door for Phase 62, small garden, mail/notice marker; no decorative duplicate house. |
| Central Common | Center | Orientation space where people cross paths and the opening disturbance can be seen. | Clear walking lanes, one bench/log, fading trail marker, no shop clutter. |
| Quiet Spring | South of the common | Emotional evidence that the route is failing; later supports the opening event. | Distinct water/stone landmark, interactable location reserved for story use. |
| Wayfarer Lodge | Northeast of the common | Healing dais, basic supplies, trail advice, save-adjacent safe hub. | Existing lodge entrance retained; lodge court keeps its door and dais legible. |
| East Gate and Fernveil Road | Far east | First progression gate and the visual promise of the journey. | A narrow maintained road, marker post, and enough space for Ren without blocking the whole settlement. |
| South Field Path | South center | Local boundary and future optional field flavor. | Closed short path or scenery only in Phase 61; it must not imply an unfinished required map. |

## NPC placement rationale

| Character | Location | Why they stand there |
| --- | --- | --- |
| Mira | Central Common, facing the fading marker | She notices the first anomaly and can direct the player between home, lodge, and road. |
| Ren | East gate trail post | As junior route steward, Ren is exactly where an early traveler needs to be assessed before entering Fernveil. |
| Orin | Lodge court / path maintenance edge | Orin maintains the public path and gives practical local texture without competing with Mira's story role. |
| Tavi | Inside Wayfarer Lodge | Tavi's shop and rest guidance belong with the supplies and healing service. |

The player start should face north or east toward the common, never directly at
the exit. Mira should be reachable before Ren, while the lodge remains visible
as a safe place from the first minute.

## Building rules

- The protagonist home, Wayfarer Lodge, and their future interiors are the
  only buildings required for the first settlement release.
- Do not add a generic shop house, empty neighbor house, or research building.
  Supplies and advice are already served by the lodge.
- Fences, gardens, storage stacks, wells, benches, and marker posts are useful
  environmental detail because they explain daily life and guide movement.
- Every walkable route must either lead to a service, a readable landmark, or
  the Fernveil exit. Decorative dead ends stay short.

## Phase 61 implementation checklist

1. Rebuild `MAP_CLEARING` as this 40×24 layout while retaining its map ID.
2. Define collision for buildings, water, hedges, gardens, and boundary edges.
3. Preserve the Wayfarer Lodge portal and its correct return tile.
4. Keep the east exit's existing gate logic and position Ren at the trail post.
5. Add the future home doorway only when its interior map ID is introduced in
   Phase 62; until then, visually mark it as a home without a false entrance.
6. Test home-side navigation, lodge transition, healing access, NPC blocking,
   and the locked/unlocked Fernveil route.

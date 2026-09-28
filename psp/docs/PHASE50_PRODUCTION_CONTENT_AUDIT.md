# Phase 50 production content audit

This audit records the production starting point after Phases 10–49. It makes
no gameplay change.

## Classification key

- **Production-ready system:** technically complete enough to reuse.
- **Prototype content:** playable content that needs a story, place, balance,
  or presentation pass.
- **Placeholder:** temporary label, layout, or behavior that should not define
  the final game.

## Maps and world map

| ID | Current name | Size | Current role | Status |
| --- | --- | ---: | --- | --- |
| `MAP_CLEARING` | Hearth Clearing | 40×24 | Start area, lodge door, Ren's gate | Prototype content |
| `MAP_FOREST` | Fernveil Woods | 32×20 | Grass, two guardian encounters, Marsh and Cave gates | Prototype content |
| `MAP_LODGE` | Wayfarer Lodge | 15×9 | Shop and healing dais | Prototype content |
| `MAP_CAVE` | Hollowstone Cave | 20×14 | Locked wilderness cave and camp healing point | Prototype content |
| `MAP_MARSH` | Sunthread Marsh | 28×18 | Reed encounters and Lantern Rest connection | Prototype content |
| `MAP_REST` | Lantern Rest | 15×9 | Later safe location | Prototype content |

The world map draws all six locations and a painted `NORTHERN WOODS` landmark.
Northern Woods has no separate map ID or traversable map yet, so that landmark
is a **placeholder**. Map loading, collision, camera, transitions, discovery,
and the world-map UI are **production-ready systems**.

## NPCs, gatekeepers, and bosses

| Map | Current characters | Status |
| --- | --- | --- |
| Hearth Clearing | Mira, Orin, Ren | Prototype content; Ren is the first challenger and east-route gatekeeper. |
| Fernveil Woods | Sen, Varel, Maren, Elder Sylva, Warden Rune | Prototype content; Varel and Maren explain gates, Sylva and Rune are guardians. |
| Wayfarer Lodge | Tavi | Prototype content; shopkeeper. |
| Hollowstone Cave | Nel | Placeholder-level ambient dialogue. |
| Sunthread Marsh | Ela | Placeholder-level ambient dialogue. |
| Lantern Rest | Ilsen | Prototype content; safe-location guide. |

Ren has a level-3 Mossprig. Elder Sylva has level-6 Mossprig and level-7
Gustlet. Warden Rune has level-12 Toxlet, level-13 Pebchick, and level-14
Galetalon. NPC movement, gating, challenge flow, AI profiles, one-time rewards,
and post-victory states are **production-ready systems**. Character identities,
motivations, and recurring story appearances remain **prototype content**.

## Veylings and moves

The roster is **production-ready for early world development**: 30 supplied
sprites form ten three-stage families. Every form has stats, habitat assignment,
descriptions, learnsets, evolution data, capture support, party/storage support,
and battle artwork.

| Family | Forms | Current habitat |
| --- | --- | --- |
| Cindlet | Cindlet → Emberyn → Pyrovern | Hollowstone Cave |
| Bubfin | Bubfin → Rivafin → Tiderion | Sunthread Marsh |
| Mossprig | Mossprig → Thornel → Elderthorn | Fernveil Woods |
| Zappip | Zappip → Ampreel → Voltrench | Sunthread Marsh |
| Grubbl | Grubbl → Cragbeet → Titanocera | Hollowstone Cave |
| Veilfin | Veilfin → Spectray → Abyssveil | Hollowstone Cave |
| Pebchick | Pebchick → Frostuin → Glacimper | Sunthread Marsh |
| Gustlet | Gustlet → Galetalon → Skyraptor | Fernveil Woods |
| Toxlet | Toxlet → Venofrog → Dreadart | Fernveil Woods |
| Glimgrub | Glimgrub → Cocoglow → Lunarae | Fernveil Woods |

There are 21 moves across Plain, Grove, Ember, Stone, Wind, Tide, Spark, Veil,
Frost, and Toxin elements. Level evolution and rank upgrades at levels 15 and
30 are **production-ready systems**. Exact encounter ecology, region rarity,
and difficulty curves are **prototype content**.

## Healing, gates, and progression

Five healing points exist: Wayfarer Dais, Lantern Dais, Fernveil Spring,
Hollowstone Camp, and Sunthread Shrine. Healing prompts and full team recovery
are **production-ready systems**.

| Gate | Requirement | Current explanation | Status |
| --- | --- | --- | --- |
| Hearth Clearing → Fernveil | First challenger defeated | Ren requests one calm battle | Prototype content using production-ready gate system |
| Fernveil → Sunthread | East Forest guardian defeated | Varel identifies the guardian requirement | Prototype content using production-ready gate system |
| Fernveil → Hollowstone | Cave unlocked after Northern Woods guardian | Maren explains the two-guardian requirement | Prototype content using production-ready gate system |

Persisted progression flags are `FIRST_CHALLENGER_DEFEATED`,
`EAST_FOREST_BOSS_DEFEATED`, `NORTH_FOREST_BOSS_DEFEATED`, and
`CAVE_UNLOCKED`. Flags, gated routes, reconciliation after loading, and boss
reward idempotence are **production-ready systems**. The flags have no final
story names or act structure yet, so their content meaning is **prototype**.

## Dialogue and story content

The dialogue system supports one or two pages, titles, NPC lines, gate states,
pre-battle and post-battle lines, healing prompts, ready prompts, and save
messages. This is a **production-ready small-dialogue system**.

Current dialogue establishes a loose Emberwake/Fernveil journey: local guides,
forest guardians, sealed cave, and an eastward road. There is no defined Vey
lore, protagonist identity, central problem, beginning/middle/end structure,
or production character cast. Existing dialogue is therefore **prototype
content**, not a final story.

## Save fields and player state

`SavePayload` version 4 persists map ID, tile and facing direction, encounter
RNG/safe-step state, party and collection creatures, move uses/ranks through
the creature encoding, inventory quantities, Embermarks, defeated NPC bits,
progression bits, and discovered-map bits. New Game, save, load, corrupt-save
rejection, and title-screen Continue are **production-ready systems**.

Not persisted: audio/motion preferences, title selection state, active dialogue,
open menus, and transient battle state. Those are appropriate session-only
values. The single save slot is a known scope limitation.

## Production conclusion

The technical framework is reusable. The next work should define the official
world, geography, lore, and story before replacing map layouts or writing large
amounts of dialogue. Do not delete current maps, characters, creatures, or
systems until that plan identifies what each should become.

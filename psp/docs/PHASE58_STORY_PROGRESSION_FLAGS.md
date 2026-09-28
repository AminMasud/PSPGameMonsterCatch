# Phase 58 story progression flags

The game persists progression as a 32-bit `ProgressionState`. Flag values are
save-data bit positions, so existing flags are never reordered. Phase 58
appends six story milestones and leaves 22 bits available for later, concrete
needs.

| Flag | Status | Set when | Unlocks or records |
| --- | --- | --- | --- |
| `PROGRESSION_INTRO_COMPLETE` | Planned | Mira finishes the opening observation request. | The story has formally begun; early story dialogue can change. |
| `PROGRESSION_FIRST_CHALLENGER_DEFEATED` | Implemented | Ren's first challenge is completed. | Fernveil Woods gate. |
| `PROGRESSION_EAST_FOREST_BOSS_DEFEATED` | Implemented | Elder Sylva's guardian challenge is completed. | Sunthread route through Varel's gate. |
| `PROGRESSION_SUNTHREAD_ENTERED` | Planned | The player reaches Sunthread Marsh for the first time. | Marsh-specific investigation dialogue and Lantern lead. |
| `PROGRESSION_LANTERN_RECORD_FOUND` | Planned | Ilsen shares the Northwood route record. | Northwood objective and contextual dialogue. |
| `PROGRESSION_NORTH_FOREST_BOSS_DEFEATED` | Implemented | Warden Rune's guardian challenge is completed. | Rune's approval condition for the cave. |
| `PROGRESSION_CAVE_UNLOCKED` | Implemented | Rune's completion is processed. | Hollowstone Cave gate. |
| `PROGRESSION_HOLLOWSTONE_REVELATION_SEEN` | Planned | The player reaches the regulator revelation in Hollowstone. | Glasswake objective and later story dialogue. |
| `PROGRESSION_GLASSWAKE_ENTERED` | Planned | The player enters Glasswake Expanse. | Stillward approach content. |
| `PROGRESSION_STILLWARD_RESTORED` | Planned | The final synchronization is resolved. | Ending and post-ending world state. |

## Implementation rules

- Current implemented flags remain the source of truth for the active gates,
  bosses, NPC state, and save/load path.
- Planned flags are defined now so later map events use stable, meaningful
  names instead of temporary booleans.
- `progression_save_bits()` and `progression_load_bits()` already serialize the
  full 32-bit state. The expanded regression test sets, saves, reloads, and
  verifies every valid flag.
- A flag is set once when its named story event completes. Repeated dialogue,
  normal map visits, and optional discovery should query these flags instead of
  creating additional one-off state.

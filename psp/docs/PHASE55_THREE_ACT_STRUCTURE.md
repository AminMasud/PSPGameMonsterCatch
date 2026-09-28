# Phase 55 three-act story structure

This outline turns **The Quieting Current** into exploration-led major beats.
It does not prescribe individual conversations, cutscenes, or map scripting.
Existing progression flags are named exactly where they already exist; later
flags are planning names until their implementation phase.

## Act I — Beginning: a familiar route goes quiet

| Event | Location | Participants | Required progression | Result | Newly unlocked content |
| --- | --- | --- | --- | --- | --- |
| First partner and fading markers | Hearth Clearing / Wayfarer Lodge | Player, Mira, Ren, starter Veyling | New Game | The Hearth spring's response weakens and an unsettled Veyling flees toward Fernveil. Mira asks the player to carry observations east. | First partner, lodge services, and the reason to seek Fernveil. |
| First trail challenge | Hearth east path | Player, Ren | None | Ren tests whether the player can respond to a partner rather than simply command one. | `PROGRESSION_FIRST_CHALLENGER_DEFEATED`; Fernveil Woods. |
| Fernveil's refusal | Fernveil Woods | Player, Sen, Elder Sylva | First challenger defeated | Forest signs show that the Quieting follows the water out of the canopy. Sylva makes a guardian challenge the condition for continuing the investigation. | Fernveil spring context and guardian objective. |
| Watershed lead | Fernveil eastern trail | Player, Elder Sylva, Varel | East Forest guardian defeated | Sylva recognizes the player's bond. Varel opens the old route after learning the altered pattern travels toward Sunthread. | `PROGRESSION_EAST_FOREST_BOSS_DEFEATED`; Sunthread Marsh. |

**Act I turn:** the player learns the disturbance is not local to Hearth. The
journey becomes an investigation of a connected living route.

## Act II — Discovery and escalation: the route points uphill

| Event | Location | Participants | Required progression | Result | Newly unlocked content |
| --- | --- | --- | --- | --- | --- |
| The water is being pulled, not poisoned | Sunthread Marsh | Player, Ela, marsh Veylings | East Forest guardian defeated | Reed channels and unsettled families show that the current is being redirected upward and east, not corrupted by a local source. | Marsh encounter ecology and Lantern Rest route. |
| The old mountain record | Lantern Rest | Player, Ilsen, Lantern keepers | Sunthread reached | Lantern's route markers identify Northwood's saddle as the meeting point of the valley currents. | Northwood Reach objective and preparation hub. |
| Two paths, one choke point | Fernveil northern trail / Northwood Reach | Player, Warden Rune | Fernveil and Lantern evidence found | The player reaches Northwood by the forest ridge or Lantern switchback. Rune explains that the cave route was closed to protect an unstable regulator. | Warden Rune guardian challenge. |
| Permission to enter Hollowstone | Northwood saddle | Player, Warden Rune, Maren | Rune challenge completed | Rune trusts the player to enter the cave; Maren withdraws the existing block. | `PROGRESSION_NORTHERN_BOSS_DEFEATED`, `PROGRESSION_CAVE_UNLOCKED`; Hollowstone Cave. |
| The regulator's answer | Hollowstone Cave | Player, cave Veylings, a recorded keeper presence | Cave unlocked | The regulator is intact. It is drawing Vey east because the larger network at The Stillward has stopped sharing the load. | Glasswake route revealed; `PROGRESSION_HOLLOWSTONE_REVELATION` planned. |

**Act II turn:** the player learns there is no single attacker to defeat. The
Quieting is a system trying to avoid a greater failure, and the route must be
restored together.

## Act III — Resolution: reconnect the living network

| Event | Location | Participants | Required progression | Result | Newly unlocked content |
| --- | --- | --- | --- | --- | --- |
| Crossing the altered basin | Glasswake Expanse | Player, returning regional keepers by signal, local Veylings | Hollowstone revelation | The player uses the observed forest, marsh, highland, and cave patterns to cross terrain that shifts with the current. | `PROGRESSION_GLASSWAKE_CROSSED` planned; The Stillward approach. |
| The Stillward synchronization | The Stillward | Player, partner Veyling, strained guardian-pattern | Glasswake crossed | The final encounter calms a pattern carrying too much of the network alone. The player reconnects the regional currents instead of claiming control over them. | `PROGRESSION_STILLWARD_RESTORED` planned; ending sequence. |
| Return of the trail | Hearth, Fernveil, Sunthread, Lantern, Northwood | Player, Mira, Ren, Sylva, Rune, local communities | Stillward restored | Habitats resume small familiar rhythms and communities commit to maintaining the shared route. The player is recognized as a trailkeeper. | Post-ending exploration and stable-region dialogue. |

## Exploration contract

- Each event is triggered by reaching and understanding a place, then opens a
  route with a physical reason to exist.
- Guardian challenges are safety and stewardship checks, never rank tokens.
- Current implemented flags remain valid. Future flags are marked planned so
  this outline does not imply that later maps or scenes already exist.

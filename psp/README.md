# Emberwake — Phase 24 Forest Boss Catalog

PSP homebrew creature-catching RPG in C / PSPSDK. This build replaces the old
placeholder creatures with the user's 30 PNGs: ten families with three forms
apiece, a round-based battle controller in which a faster enemy acts before
player command selection, a clear spotlight on the Veyling performing each
action, a full-screen battle party selector, the reusable ready prompt, a
data-driven framework for NPC challengers, the first in-world challenger at the
East Forest entrance, a reusable named progression-flag system, and reusable
flag-controlled entrances, progression-aware forest gatekeepers, a sealed
Hollowstone Cave entrance, a reusable boss battle framework with optional
special presentation, Elder Sylva, Fernveil's first in-world Guardian,
data-linked boss-gated routes, and a reusable forest boss catalog.
The PNG number minus one is the internal
species ID. All 30 forms have stats, descriptions, attacks, capture support, and
their own supplied artwork.

## Play this build

Use **EBOOT-PHASE24.PBP**, titled **Emberwake - Phase 24**. Copy it to:

    ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP

The images and audio are embedded. No separate asset folders are needed on the
Memory Stick. Earlier EBOOT-PHASE10.PBP, EBOOT-PHASE11.PBP,
EBOOT-PHASE12.PBP through EBOOT-PHASE23.PBP, EBOOT-PETS.PBP, and numbered phase
builds are retained locally for comparison.

- D-pad: move; select menu entries. Release finishes the current tile.
- X: talk, confirm, or advance a message.
- Circle: close dialogue or return one menu page.
- Triangle: open the Field Kit while stationary; close all player menus.
- Select: lead partner's stats, XP, and description while stationary.
- Start: save while stationary or from the Field Kit.
- L: load while stationary or from the Field Kit.
- HOME: PSP exit menu.

Start a new session with a level-5 Cindlet, or press L to load an existing save.
Saving/loading is unavailable during battles. Dialogue and menus pause movement
and NPC patrols. Button actions use new-press detection.

## Ten evolution families

Base forms evolve on level-up at **level 8**; middle forms evolve at **level 16**.
Large XP awards can cross both thresholds. Nicknames and known moves survive
evolution. Every form, including evolved forms, can also be caught in the wild.

| PNG numbers | Base → middle → final | Element | Habitat |
| --- | --- | --- | --- |
| 001–003 | Cindlet → Emberyn → Pyrovern | Ember | Hollowstone Cave |
| 004–006 | Bubfin → Rivafin → Tiderion | Tide | Sunthread Marsh |
| 007–009 | Mossprig → Thornel → Elderthorn | Grove | Fernveil Woods |
| 010–012 | Zappip → Ampreel → Voltrench | Spark | Sunthread Marsh |
| 013–015 | Grubbl → Cragbeet → Titanocera | Stone | Hollowstone Cave |
| 016–018 | Veilfin → Spectray → Abyssveil | Veil | Hollowstone Cave |
| 019–021 | Pebchick → Frostuin → Glacimper | Frost | Sunthread Marsh |
| 022–024 | Gustlet → Galetalon → Skyraptor | Wind | Fernveil Woods |
| 025–027 | Toxlet → Venofrog → Dreadart | Toxin | Fernveil Woods |
| 028–030 | Glimgrub → Cocoglow → Lunarae | Grove; final form Wind | Fernveil Woods |

The party screen displays stage 1/3, 2/3, or 3/3, list portraits, and a larger
portrait of the selected creature. Battles use the supplied images. The ally's
image is mirrored to face the opponent; these are not separately drawn back sprites.

## World and encounters

| Area | Contents and connections |
| --- | --- |
| Hearth Clearing | Mira, patrolling Orin; northwest lodge doorway; Ren guards the east path to the woods |
| Fernveil Woods | Sen and Elder Sylva, tall-grass encounters; west to clearing; Maren seals the northeast cave; Varel guards the east road |
| Wayfarer Lodge | Tavi's supply shop, green healing dais; south to clearing |
| Hollowstone Cave | Nel, rough-floor encounters; southwest doorway to woods |
| Sunthread Marsh | Ela, reed encounters, ponds and safe boardwalk; west to woods, northeast rest house |
| Lantern Rest | Ilsen, green healing dais; south to marsh |

Only completed movement steps trigger encounter rolls. Paths, ordinary grass,
flowers, boardwalks, and interiors are safe. Eligible terrain has an 18% encounter
chance after four safe steps following map entry or battle. Standing still never
triggers an encounter. Entering a map resets ordinary patrols; a defeated route
challenger returns to the stepped-aside position stored by progression.

Families within each habitat have equal selection weight. Stage probabilities
within a chosen family are 90% base, 9% middle, and 1% final. Forest base forms
are levels 2–4, cave base forms 3–5, and marsh base forms 5–7. Middle forms are
levels 8–10 and final forms 16–18. Rare evolved encounters can be much stronger
than a new team; RUN is guaranteed on the third attempt.

## Battles and progression

Important NPC and boss encounters can open a reusable **ARE YOU READY?** screen.
YES starts the exact pending encounter; NO or Circle closes the prompt and leaves
the player at the same overworld position. The prompt pauses movement and NPC
patrols while it is open. Ordinary random wild encounters continue directly to
battle without showing this confirmation. Trainer parties and NPC challenge
data are supported by the reusable NPC battle framework. The existing
exploration NPCs retain their dialogue, shop, and healing behavior.

Ren stands on Hearth Clearing's east exit and blocks entry to Fernveil Woods.
Facing Ren and pressing X opens two introductory lines followed by **ARE YOU
READY?**. YES begins an easy battle against one level-3 Mossprig; NO, Circle, or
leaving the intro closes the challenge without moving the player. Winning grants
50 Embermarks once, shows the victory dialogue, and makes Ren step north so the
portal is open. The defeated bit survives save/load and map changes. Later talks
repeat the victory dialogue without forcing another battle.

Progression uses one `ProgressionState` instead of separate gameplay booleans.
The named flags currently cover the first challenger, the east and north forest
bosses, and cave access. `progression_has`, `progression_set`, and
`progression_clear` provide the shared query/update interface for map gates, NPC
dialogue, and future events. Adding a flag requires one enum entry; a compile-time
capacity check keeps the list within the 32 bits stored by the save format.
Serialization uses `progression_save_bits` and `progression_load_bits`, retaining
all raw bits so later flags are not discarded by an intermediate build.

Ren's first victory now sets `PROGRESSION_FIRST_CHALLENGER_DEFEATED` through the
same data-driven NPC battle definition that marks Ren defeated. Loading a Phase
16 save also reconciles Ren's defeated ID into this named flag, so existing
progress continues cleanly.

Every entrance now uses a reusable `Gate` record containing its source tile,
destination and arrival tile, optional required progression flag, gatekeeper,
and locked and unlocked dialogue. `gate_is_locked`, `gate_can_enter`, and
`gate_current_dialogue` provide the common state checks. A locked gate is
rejected by the same collision path as terrain and NPCs, so it cannot be crossed.

`gate_requires_boss_completion` explicitly connects a gate's required progression
flag to a boss definition's completion flag. The connection is data-driven, so it
does not depend on where either the boss or gate is placed. East Forest uses
Sylva's completion flag for Varel's Sunthread route; a later boss can unlock a
different route by using another named flag.

The Hearth Clearing east entrance is the first configured locked gate. It
requires `PROGRESSION_FIRST_CHALLENGER_DEFEATED` and uses Ren as its gatekeeper.
Before victory, Ren starts the existing challenge and the gate blocks entry.
Afterward, Ren uses the gate's unlocked dialogue and the destination opens.

Varel now guards Fernveil Woods' east road into Sunthread Marsh. The road starts
locked and his dialogue tells the player to defeat Fernveil's Veyling Guardian.
The gate requires `PROGRESSION_EAST_FOREST_BOSS_DEFEATED`, the stable flag that
Elder Sylva awards. When present, Varel steps north, confirms
that the Guardian has yielded, and opens the road. This position and dialogue
are reconstructed from the saved progression state after every map entry or
load. The reverse road remains open so an older save already in Sunthread Marsh
cannot become trapped.

Maren now stands visibly inside the Hollowstone Cave doorway. The entrance uses
the same `Gate` data and collision path as the forest roads and requires
`PROGRESSION_CAVE_UNLOCKED`. Maren explains that the Veyling Guardians of
Fernveil and the Northern Woods must both be defeated. When the cave flag is
present, Maren steps beside the path, announces that the seal is open, and the
doorway becomes traversable. Cave state is rebuilt from the saved flag. The
return portal remains open so an existing save inside Hollowstone can always
leave safely.

Each NPC battle definition has a stable ID, name, up to four Veylings with
species and levels, one or two pages of dialogue before battle and after
victory, optional dialogue after defeat, an AI profile, an Embermark reward,
and an optional progression bit. The flow is intro dialogue → ready prompt →
party battle → outcome dialogue. Circle can leave the intro and NO can leave the
ready prompt without beginning a battle.

NPC parties send out their next healthy configured Veyling after the current
one is defeated. The active player Veyling earns XP for each opponent. Capture
and run commands are rejected without spending a turn. A victory is recorded
only after the whole NPC party is defeated; the reward and progression flag are
granted once. Future interactions use the post-victory dialogue instead of
starting another battle. A loss does not mark the NPC defeated and can show its
optional defeat dialogue after the normal return to Hearth Clearing.

Boss definitions are a separate reusable layer with a stable boss ID, speaker
name, battle title, party of up to four Veylings, AI profile, intro, victory and
optional defeat dialogue, Embermark reward, prerequisite flag, completion flag,
and optional battle presentation. The boss flow is intro dialogue → ready prompt
→ party battle → outcome dialogue. A completed boss uses its victory dialogue on
later interactions and does not start another battle.

The completion flag is the source of truth for boss victory and already travels
through the version-3 save payload. Victory sets it idempotently, grants the
reward only on the first transition from incomplete to complete, and reapplies
gatekeeper state immediately. The first optional presentation, GUARDIAN, gives
boss battles a dark violet field, gold frame, and GUARDIAN header while reusing
the normal battle controller.

Elder Sylva stands near the east end of Fernveil Woods. Talking to Sylva opens
two intro pages and the ready prompt, then starts a Guardian battle against a
level-6 Mossprig and level-7 Gustlet. This two-member party and boss AI profile
make the encounter stronger than Ren's single level-3 challenger. Victory grants
300 Embermarks once, sets `PROGRESSION_EAST_FOREST_BOSS_DEFEATED`, moves Sylva
off the road, and immediately moves Varel aside to open the Sunthread route.
Later talks use Sylva's post-victory dialogue without another battle or reward.

The boss catalog now owns forest boss data. Sylva is loaded from its East Forest
entry; a Northern Woods Guardian placeholder is catalog data only and is not
placed in the world. It requires Sylva's completion flag, has a distinct
three-Veyling party, dialogue, 650-Embermark reward, North Forest completion
flag, and AI profile. Adding a later boss therefore needs a new data entry and
placement, without copying the battle flow.

The easy NPC AI selects randomly from the acting Veyling's currently usable
move slots. Empty slots, invalid move IDs, and attacks with zero uses are never
selected. If every attack is unavailable, it explicitly selects PRESS ON, the
existing weak unlimited fallback. Selection does not spend a move; the battle
resolver spends it only when the attack executes.

The AI interface receives only its own Veyling, its configured profile, and its
private random state. It cannot read the player's pending command. The choice is
made once at round start and remains locked while the player navigates menus.
Standard and boss profile entries currently inherit the safe easy policy through
the same dispatch point, ready for later strategy functions.

At the start of each round, the enemy chooses one action and turn order is locked
from the creatures' speeds. A faster enemy attacks immediately, before the game
opens the player command menu. A faster player receives the menu first; speed
ties are random. Canceling or navigating menus does not reroll the enemy action
or change the order. The controller separately tracks round start, enemy choice,
player input, first and second action resolution, faint checks, result checks,
and round completion.

While an action message is visible, the acting Veyling stays at full brightness
and the other sprite is dimmed. The highlight switches with the actor and clears
for command selection, attack selection, replacement menus, and result messages.
Names, HP panels, and battle text are never dimmed. The brightness cue remains
visible when animation is disabled.

FIGHT chooses one of four attacks. Each attempt consumes one use, including
misses. A creature knocked out by the first action cannot perform the queued
second action. Each action/result advances with X. Spent attacks cannot be
selected; when all attacks are spent, PRESS ON is a weak unlimited fallback.
Circle returns from attack, item, capture, and voluntary-switch menus without
spending a turn. RUN succeeds 70% of the time and always on the third attempt.

Damage uses integer arithmetic:

    base = ((2 * level / 5 + 2) * power * attack / defense) / 20 + 2
    damage = max(1, base * elemental_multiplier * random(90..100) / 100)

Strong matchups deal 2x, reverse matchups and matching elements 0.5x, and others
1x. Plain attacks/defenders are neutral. Strong matchups are Ember→Grove,
Grove→Stone, Stone→Wind, Wind→Ember, Tide→Ember, Spark→Tide, Grove→Tide,
Stone→Spark, Ember→Frost, Frost→Wind, Veil→Toxin, Toxin→Grove, and Wind→Toxin.
The five new elements have ten new attacks. Toxin attacks currently deal damage;
there is no persistent poison status effect.

Victory awards species XP yield times opponent level to the active battler.
Earlier participants and stored creatures receive no share. Capture, escape,
and defeat grant no XP. Level L requires 20 * (L - 1)^2 cumulative XP; the cap is
100. Stats are base HP + 5*level, attack + 3*level, defense + 2*level, and speed
+ 2*level. Evolution changes the bases.

New attacks fill empty slots automatically. If all four are occupied, choose a
move to replace or keep the current moves. Circle declines. Known moves and
nicknames survive evolution. There is no nickname editor yet.

**Temporary prototype rule:** every party and stored creature regains all HP and
attack uses after every battle, including escape, capture, and defeat. The last
active battler becomes the lead. Defeat returns the player to Hearth Clearing;
other outcomes return to the same exploration position. Four safe steps follow.

## Capture, party, storage, and items

CAPTURE uses the Resonance Loom, with three charges per battle. The confirmation
shows the chance; X attempts, Circle cancels. A successful capture adds the wild
creature to the first free party slot, or to storage if the party is full.
A failed capture spends one charge and consumes the player's action. The enemy
performs its one scheduled action only if it has not already acted that round.
Full capacity, no charges, or canceling spends no turn. Knocked-out targets
cannot be caught.

    chance = clamp(35 - 12*stage + floor(50*(maxHP-HP)/maxHP) + 10*(strength-1), 5, 95)

Stage is zero-based (0, 1, 2). At full HP, base/middle/final chances are 35%, 23%,
and 11%. The current Loom uses strength 1. A random 0–99 roll below the chance
succeeds.

The party holds four creatures; storage now holds **32**, enough to collect all
30 forms. Triangle opens the Field Kit with CREATURES, ITEMS, COLLECTION, PLAYER,
SAVE, and OPTIONS. Party and storage tabs support lead selection, deposits,
withdrawals, and swapping when the party is full. The final party member cannot
be deposited. There is no release/delete action. Transfers preserve all state.
Circle returns to the Field Kit; Triangle closes it completely.

SWAP opens a full-screen BATTLE PARTY view with each partner's artwork, HP,
level, readiness, and selected-creature details. The current active Veyling and
fainted Veylings cannot be selected. Circle cancels a voluntary swap without
spending the action. A valid selection closes the party screen immediately.

Voluntary switching consumes the player's action, so the enemy performs its
scheduled action only if it has not already acted before the normal command menu
returns. Replacing a knocked-out ally is free, mandatory, and cannot be canceled
while a healthy reserve remains. The replacement starts a fresh speed-ordered
round and never inherits the knocked-out ally's queued attack. The entire party
must be defeated before the battle is lost.

ITEMS heals the active battler in combat or the lead partner from the Field Kit.
Pulse Tonic restores up to 25 HP; a full-restoration item restores maximum HP.
Invalid, full-HP, fainted, or out-of-stock uses consume nothing. A valid combat
use consumes the player's action; the enemy acts afterward only when still due
in that round. Buy supplies with Embermarks after finishing Tavi's dialogue in
the lodge. Face either green healing dais and press X to restore the whole
roster's HP and attack uses.

## Existing saves

New saves use **version 3**, a 2512-byte payload, retaining the existing PSP slot
EMBRWAKE0000 / DATA.BIN. It records map/tile/facing, encounter RNG and safe steps,
party, lead, storage, levels, XP, nicknames, HP, moves/uses, inventory, and money.
It also records up to 32 defeated NPC IDs and progression flags. NPC patrol
positions, open menus, dialogue, and battles are not saved.

Version-2 saves from Phases 10–13 load through a frozen 2504-byte layout with
empty NPC progress. Version-1 saves from Phases 8/9 also migrate from their
frozen 1960-byte layout. Old version-1 creatures are converted as follows:

| Old creature | New creature |
| --- | --- |
| Cindlet | Cindlet |
| Emberlyn | Emberyn |
| Mosslet | Mossprig |
| Mosshorn | Thornel |
| Twiglint | Glimgrub |
| Glowmoth | Lunarae |
| Flintling | Grubbl |
| Flintaur | Cragbeet |
| Duskwisp | Veilfin |
| Echocrag | Titanocera |
| Reedskip | Toxlet |
| Sunfinch | Gustlet |

Migration keeps levels, XP, nicknames, moves/uses, currency, inventory, roster
positions, lead, and location. HP preserves damage taken against the replacement's
new maximum; fainted creatures remain fainted. Converted partners already at an
evolution threshold advance to their eligible form, including level-100 saves.
Loading does not rewrite the file.
Saving afterward writes version 3, which earlier game builds cannot load. Unknown
versions, truncated files, and invalid old species IDs are rejected.

The savedata service waits for shutdown completion and checks the final utility
result, including utilities that do not expose FINISHED to polling. The game
validates loaded gameplay values before replacing the current session.

## Sound and animation

Four original synthesized phrases cover settlements, wilderness, cave, and
battles. Menu, impact, healing, encounter, victory, and error effects are also
synthesized. The integer callback performs no asset reads or heap allocation.
Savedata dialogs mute game sound. Failed audio initialization allows silent play.

OPTIONS independently toggles music, effects, and animation. These settings apply
to the current play session. Animation includes battle bobbing, impact shake,
smooth HP bars, water/reed movement, map shutters, and location labels. Turning
animation off makes HP changes immediate and removes those motion effects.
Dialogue wraps inside its panel, using smaller text when needed.

## Artwork and source structure

The original images remain unchanged under assets/pets/. The converter crops
transparent margins, preserves aspect ratio, and fits each form into a 96x96
canvas. Base/middle/final artwork fits 76/84/92 pixels respectively. Each canvas
occupies a padded 128x128 RGBA4444 texture. All 30 textures together use 983,040
bytes of system RAM; the two screen buffers retain the existing VRAM allocation.

assets/generated/pets.rgba4444 and pets.json are committed build inputs. The
manifest records the source and texture checksums. src/pet_assets.S embeds the
texture data, pet_draw.c chooses the numbered sprite, and graphics.c draws
alpha-blended textured strips with an optional hardware color tint before
restoring the rectangle rendering state. The spotlight reuses the two existing
sprite draws and adds no geometry. No PNG decoder, runtime asset loading, or
additional PSP libraries are required.

- include/: public interfaces and data models.
- src/: game systems, GU renderer, menus, audio, savedata service and migration.
- assets/maps/: six embedded character-grid maps.
- assets/pets/: the 30 original PNGs.
- assets/generated/: compiled textures and their manifest.
- tools/build_pet_assets.py: asset compiler (Python 3 + Pillow).
- tools/check_pet_assets.py: build-time integrity check (Python standard library).
- tests/: host C regression suites, PSP utility stubs, and software previews.
- SOURCE_CONTENTS.md: complete text-source snapshot; binary assets are referenced
  through the manifest and committed alongside the source.

Map tiles: ordinary grass '.', path '=', flowers ',', trees '#', rocks/furniture
'O', water '~', walls 'W', doorway 'D', exits '<' and '>', interior floor '_',
healing dais 'H', encounter grass 'g', cave floor 'c', and reeds 'r'. Tree, rock,
water, and wall tiles block movement. Portal and gate coordinates, requirements,
dialogue, and destinations are defined in map.c; arrival tiles are clear of
return triggers.

## Build and verification

From PowerShell on this machine:

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && sh tests/run.sh && make PSP_EBOOT=EBOOT-PHASE24.PBP EXTRA_TARGETS=EBOOT-PHASE24.PBP'

From a configured Linux/WSL PSPDEV shell in this directory:

    sh tests/run.sh
    make PSP_EBOOT=EBOOT-PHASE24.PBP EXTRA_TARGETS=EBOOT-PHASE24.PBP

The build checks that all PNGs, numbered species IDs, and compiled textures match.
For changed PNGs, regenerate first using Python 3 with Pillow installed:

    python3 tools/build_pet_assets.py

`make rebuild-pets` runs the same converter. Ordinary builds use the committed
compiled assets and do not require Pillow. The user-mode PRX targets 6.60/6.61
custom firmware. Warnings are treated as errors. Library order keeps PSP import
stubs together, with pspaudiolib first and the utility import library last.

All fifteen C suites run with AddressSanitizer and UndefinedBehaviorSanitizer. The
dedicated Phase 10 suite covers player-first, enemy-first, equal-speed, every
first/second-action knockout combination, command actions, forced replacements,
repeated rounds without duplicates, and Phase 11 actor ownership. The renderer
checks the normal, ally-action, and enemy-action tint states. Phase 12 checks
voluntary cancel, active/fainted rejection, immediate selector exit, exactly one
enemy response, enemy-first swaps, and fast/slow forced replacements. Phase 13
checks YES, selected NO, Circle cancellation, frozen overworld actors, request
validation, and direct wild-battle entry. Phase 14 checks battle-data validation,
multi-Veyling opponents, AI profile transport, locked capture/run commands,
intro/ready/outcome flow, one-time rewards, optional defeat dialogue, defeated
state, progression flags, and version-3 persistence with v1/v2 migration. The
Phase 15 suite checks every move-slot validity rule, selection across available
attacks, profile dispatch, no mutation during selection, and PRESS ON without an
unnecessary RNG roll. The Phase 16 integration checks the blocked east portal,
intro and ready flow, one weak opponent, easy AI profile, one-time reward,
step-aside behavior, repeat interaction, portal access, and saved defeat state.
The Phase 17 suite checks every named flag, invalid queries, idempotent updates,
clearing, raw-bit round trips, NPC-granted progression, save/load, and Phase 16
save reconciliation.
The Phase 18 suite validates gate definitions, destinations, progression
requirements, locked and unlocked state, gatekeeper dialogue, open entrances,
invalid data, movement blocking, and the complete Ren unlock flow.
The Phase 19 integration checks both forest gatekeepers, Varel's exact Guardian
requirement, changed post-flag dialogue, immediate step-aside behavior, blocked
and open traversal, reverse-route safety, and save/load restoration.
The Phase 20 integration checks the visibly occupied cave doorway, both-Guardian
requirement text, shared gate collision, cave-flag unlock, changed dialogue,
step-aside position, two-way travel, old-save escape route, and persistence.
The Phase 21 suite validates boss IDs, names and titles, parties, AI profiles,
dialogue, rewards, required completion flags, presentation modes, and one-time
completion. Its integration path covers cancel, loss and retry, the guardian
visual treatment, victory, immediate progression updates, one-time rewards, and
post-victory interaction without another battle.
The Phase 22 integration reaches Elder Sylva through the real Fernveil NPC,
checks the two-member party and level curve, runs the intro and ready flow,
renders the Guardian battle, records victory and its one-time reward, updates
Sylva and Varel immediately, repeats the post-victory dialogue, enters the newly
opened Sunthread route, and rebuilds both NPC positions from saved progression.
Phase 23 additionally verifies the explicit boss-to-gate flag link, checks a
different future boss flag can map to a different route, opens Varel's dialogue
through the real post-boss interaction, and confirms the unlocked dialogue after
save/load restoration.
Phase 24 validates both catalog entries, their distinct parties, dialogue,
rewards, completion flags, and AI profiles. It also verifies the Northern Woods
entry remains unavailable until the East Forest flag is set, then enters the
same reusable intro flow without an in-world placement.
The full suite also covers movement, all portals,
collisions, capture, inventory, menus, all 30 forms at levels 1–100, all ten
two-step evolution chains, wild availability of every form, 32-slot storage,
savedata lifecycle, text bounds, drawing budget, and PCM audio.

Software previews use the real draw functions and embedded texture data. They
include ready-prompt.png, ready-prompt-no.png, npc-battle.png, boss-battle.png,
east-forest-boss.png, east-forest-boss-ready.png,
east-forest-boss-battle.png, east-forest-boss-victory.png,
east-forest-route-open.png,
east-challenger.png, east-challenger-ready.png, east-challenger-battle.png,
east-challenger-victory.png, forest-gatekeeper-locked.png,
forest-gatekeeper-open.png, cave-gatekeeper-locked.png,
cave-gatekeeper-open.png, spotlight-idle.png,
spotlight-ally.png, spotlight-enemy.png, pet-001.png through pet-030.png,
party/collection/battle/menu scenes, and pet-roster.png from the asset compiler.
They are not hardware screenshots.

PSP test route:
1. From Hearth Clearing, follow the path east. Verify Ren blocks the forest exit.
2. Talk to Ren, decline once, then accept. Verify the battle has one level-3
   Mossprig and that capture and run remain locked during the NPC battle.
3. Win, read both victory lines, and verify Ren steps north and the forest exit
   opens. Talk again and verify the ready prompt does not return.
4. Save, restart or leave the map, load, and verify Ren remains defeated, stays
   beside the open path, and does not offer another battle.
5. In Fernveil Woods, follow the main road east to Elder Sylva. Decline once,
   then accept. Verify the Guardian battle has a level-6 Mossprig followed by a
   level-7 Gustlet and uses the violet and gold boss presentation.
6. Win and verify Sylva grants 300 Embermarks, steps north, and talks about the
   recognized bond on repeat interaction without starting another battle. Verify
   Varel also steps aside and the Sunthread route opens. Save and load and verify
   the defeated state, positions, open route, and reward remain unchanged.
7. Follow Fernveil's northeast path. Verify Maren visibly blocks Hollowstone Cave
   and explains that the Fernveil and Northern Woods Guardians must be defeated.
   Save and load there and verify the cave remains sealed.
8. Load an existing version-2 save and verify roster, items, money, and location
   remain intact; saving again upgrades the slot to version 3.

Host tests and PSP compilation validate the code; actual PSP texture rendering,
sound, and performance still require this device test.

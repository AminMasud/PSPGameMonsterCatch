# Emberwake — Phase 12 Battle Swap Navigation

PSP homebrew creature-catching RPG in C / PSPSDK. This build replaces the old
placeholder creatures with the user's 30 PNGs: ten families with three forms
apiece, a round-based battle controller in which a faster enemy acts before
player command selection, a clear spotlight on the Veyling performing each
action, and a full-screen battle party selector with automatic return after a
swap. The PNG number minus one is the internal species ID. All 30 forms have
stats, descriptions, attacks, capture support, and their own supplied artwork.

## Play this build

Use **EBOOT-PHASE12.PBP**, titled **Emberwake - Phase 12**. Copy it to:

    ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP

The images and audio are embedded. No separate asset folders are needed on the
Memory Stick. Earlier EBOOT-PHASE10.PBP, EBOOT-PHASE11.PBP, EBOOT-PETS.PBP,
and numbered phase builds are retained locally for comparison.

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
| Hearth Clearing | Mira, patrolling Orin; northwest lodge doorway; east path to woods |
| Fernveil Woods | Sen, tall-grass encounters; west to clearing, northeast cave, east marsh |
| Wayfarer Lodge | Tavi's supply shop, green healing dais; south to clearing |
| Hollowstone Cave | Nel, rough-floor encounters; southwest doorway to woods |
| Sunthread Marsh | Ela, reed encounters, ponds and safe boardwalk; west to woods, northeast rest house |
| Lantern Rest | Ilsen, green healing dais; south to marsh |

Only completed movement steps trigger encounter rolls. Paths, ordinary grass,
flowers, boardwalks, and interiors are safe. Eligible terrain has an 18% encounter
chance after four safe steps following map entry or battle. Standing still never
triggers an encounter. Entering a map resets NPCs to their starting positions.

Families within each habitat have equal selection weight. Stage probabilities
within a chosen family are 90% base, 9% middle, and 1% final. Forest base forms
are levels 2–4, cave base forms 3–5, and marsh base forms 5–7. Middle forms are
levels 8–10 and final forms 16–18. Rare evolved encounters can be much stronger
than a new team; RUN is guaranteed on the third attempt.

## Battles and progression

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

New saves use **version 2**, a 2504-byte payload, retaining the existing PSP slot
EMBRWAKE0000 / DATA.BIN. It records map/tile/facing, encounter RNG and safe steps,
party, lead, storage, levels, XP, nicknames, HP, moves/uses, inventory, and money.
NPC patrol positions, open menus, dialogue, and battles are not saved.

Version-1 saves from Phases 8/9 load through an explicit migration of the frozen
1960-byte layout. Old creatures are converted as follows:

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
Saving afterward writes version 2, which earlier game builds cannot load. Unknown
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
water, and wall tiles block movement. Portal coordinates and destinations are
explicitly defined in map.c; arrival tiles are clear of return triggers.

## Build and verification

From PowerShell on this machine:

    wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/polo1/OneDrive/Documents/app/psp && sh tests/run.sh && make PSP_EBOOT=EBOOT-PHASE12.PBP EXTRA_TARGETS=EBOOT-PHASE12.PBP'

From a configured Linux/WSL PSPDEV shell in this directory:

    sh tests/run.sh
    make PSP_EBOOT=EBOOT-PHASE12.PBP EXTRA_TARGETS=EBOOT-PHASE12.PBP

The build checks that all PNGs, numbered species IDs, and compiled textures match.
For changed PNGs, regenerate first using Python 3 with Pillow installed:

    python3 tools/build_pet_assets.py

`make rebuild-pets` runs the same converter. Ordinary builds use the committed
compiled assets and do not require Pillow. The user-mode PRX targets 6.60/6.61
custom firmware. Warnings are treated as errors. Library order keeps PSP import
stubs together, with pspaudiolib first and the utility import library last.

All ten C suites run with AddressSanitizer and UndefinedBehaviorSanitizer. The
dedicated Phase 10 suite covers player-first, enemy-first, equal-speed, every
first/second-action knockout combination, command actions, forced replacements,
repeated rounds without duplicates, and Phase 11 actor ownership. The renderer
checks the normal, ally-action, and enemy-action tint states. Phase 12 checks
voluntary cancel, active/fainted rejection, immediate selector exit, exactly one
enemy response, enemy-first swaps, and fast/slow forced replacements. The full
suite also covers movement, all portals, collisions, capture, inventory, menus,
all 30 forms at levels 1–100, all ten two-step evolution chains, wild availability
of every form, 32-slot storage, v1 migration, v2 game save/load with 36 creatures,
savedata lifecycle, text bounds, drawing budget, and PCM audio.

Software previews use the real draw functions and embedded texture data. They
include spotlight-idle.png, spotlight-ally.png, spotlight-enemy.png, pet-001.png
through pet-030.png, party/collection/battle/menu scenes, and pet-roster.png from
the asset compiler. They are not hardware screenshots.

PSP test route:
1. Choose SWAP and verify the full-screen BATTLE PARTY view opens.
2. Select the active Veyling and a fainted Veyling; both must remain rejected.
3. Select a ready reserve and verify the party screen closes immediately.
4. In a player-first round, verify the enemy acts once before the next menu.
5. In an enemy-first round, verify swapping does not grant a second enemy action.
6. Force a replacement after a knockout; Circle must not cancel it, and a valid
   reserve must enter a fresh round with the correct speed order.

Host tests and PSP compilation validate the code; actual PSP texture rendering,
sound, and performance still require this device test.

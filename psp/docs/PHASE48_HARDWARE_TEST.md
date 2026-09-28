# Phase 48 PSP-3000 hardware checklist

Run this checklist on a PSP-3000 with `EBOOT-PHASE48.PBP` installed as
`ms0:/PSP/GAME/EMBERWAKE/EBOOT.PBP`.

1. Start the game and confirm the 480×272 splash, title image, field, menus,
   battle screen, and text are fully visible with no edge corruption.
2. Start New Game, walk between Hearth Clearing, Wayfarer Lodge, Fernveil,
   Sunthread Marsh, Lantern Rest, and Hollowstone Cave. Repeat the loop ten
   times and confirm movement and map transitions remain responsive.
3. Fight several wild battles, one NPC battle, and a guardian battle. Check
   menu response, party switching, audio, victory, and defeat return paths.
4. Save in the field, exit with HOME, relaunch, choose Continue, and verify
   the saved map, tile, party, inventory, and progression return correctly.
5. Leave the game running while alternating field movement, menus, battles,
   and map transitions for at least 30 minutes. Listen for audio glitches and
   watch for slowdowns, visual corruption, or failed input.
6. If your PSP firmware supports it for homebrew, suspend and resume once in
   the field and once after a battle. Confirm controls, audio, and rendering
   recover normally.

Record any failed step with the map, action, and whether it happened after a
save/load or sleep/resume. The software checks cannot emulate the PSP HOME
menu or hardware sleep behavior.

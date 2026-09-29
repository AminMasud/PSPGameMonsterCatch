# Phase 59 dialogue production review

## Capability audit

| Required capability | Current support | Decision |
| --- | --- | --- |
| Single dialogue | `dialogue_open()` | Already supported. |
| Multi-page dialogue | `dialogue_open_pages()` supports up to three pages; the legacy two-page helper remains. | Added the missing third-page public API. |
| NPC dialogue | NPC records provide name plus before/after text; interaction opens dialogue. | Already supported. |
| Pre-battle dialogue | NPC and guardian data open before text, then the existing readiness prompt. | Already supported. |
| Post-battle dialogue | Battle completion opens victory text and reconciles progress. | Already supported. |
| Progression-dependent dialogue | Gates, bosses, and NPC progress select text based on `ProgressionState`. | Already supported. |
| Yes/no choice | Compact Ready Prompt and Healing Prompt provide controller-friendly Yes/No selection. | Sufficient for current battle and service choices; no generic branching tree is needed yet. |
| Story-event dialogue | `dialogue_open_pages()` can present a bounded event sequence; future event code sets flags after the sequence. | Added the needed page API without callbacks or a branching engine. |

## Production rules

- Keep ordinary field dialogue to one or two pages; reserve the third page for
  discoveries, guardian context, and major revelations.
- Use the existing progression flags to select the correct version of a line.
- Put state changes in the event or battle code, not inside dialogue text.
- Keep yes/no prompts for actions with an immediate result. The story does not
  currently require persistent dialogue choices or a branching narrative graph.
- Every page is bounded to 159 characters and is rendered through the existing
  dialogue panel, preserving PSP readability and the current input flow.

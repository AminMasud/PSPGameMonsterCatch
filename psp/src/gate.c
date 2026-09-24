#include "gate.h"

static int dialogue_valid(GateDialogue dialogue)
{
    return dialogue.first && dialogue.first[0] &&
           (!dialogue.second || dialogue.second[0]);
}

int gate_valid(const Gate *gate)
{
    if(!gate || gate->from<0 || gate->x<0 || gate->y<0 || gate->to<0 ||
       gate->arrival_x<0 || gate->arrival_y<0) return 0;
    if(gate->required_flag!=PROGRESSION_NONE &&
       !progression_flag_valid(gate->required_flag)) return 0;
    if(!gate->gatekeeper)
        return gate->required_flag==PROGRESSION_NONE && !gate->locked_dialogue.first &&
               !gate->locked_dialogue.second && !gate->unlocked_dialogue.first &&
               !gate->unlocked_dialogue.second;
    if(!gate->gatekeeper[0] || !dialogue_valid(gate->unlocked_dialogue)) return 0;
    if(gate->required_flag==PROGRESSION_NONE)
        return !gate->locked_dialogue.first && !gate->locked_dialogue.second;
    return dialogue_valid(gate->locked_dialogue);
}

int gate_is_locked(const Gate *gate,const ProgressionState *progression)
{
    return gate && gate->required_flag!=PROGRESSION_NONE &&
           !progression_has(progression,gate->required_flag);
}

int gate_can_enter(const Gate *gate,const ProgressionState *progression)
{
    return gate && !gate_is_locked(gate,progression);
}

const GateDialogue *gate_current_dialogue(const Gate *gate,
                                          const ProgressionState *progression)
{
    if(!gate || !gate->gatekeeper) return 0;
    return gate_is_locked(gate,progression)?&gate->locked_dialogue:
                                             &gate->unlocked_dialogue;
}

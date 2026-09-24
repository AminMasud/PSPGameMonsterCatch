#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "gate.h"

int main(void)
{
    const Gate cave={
        .from=1,.x=8,.y=4,.to=2,.arrival_x=3,.arrival_y=7,
        .required_flag=PROGRESSION_CAVE_UNLOCKED,.gatekeeper="WARDEN",
        .locked_dialogue={"THE CAVE IS SEALED.","RETURN WITH THE CAVE SIGIL."},
        .unlocked_dialogue={"THE SIGIL ANSWERS.","THE CAVE IS OPEN."}
    };
    ProgressionState progression;progression_init(&progression);
    assert(gate_valid(&cave));
    assert(gate_is_locked(&cave,&progression));
    assert(!gate_can_enter(&cave,&progression));
    const GateDialogue *words=gate_current_dialogue(&cave,&progression);
    assert(words && !strcmp(words->first,"THE CAVE IS SEALED."));

    assert(progression_set(&progression,PROGRESSION_CAVE_UNLOCKED));
    assert(!gate_is_locked(&cave,&progression));
    assert(gate_can_enter(&cave,&progression));
    words=gate_current_dialogue(&cave,&progression);
    assert(words && !strcmp(words->first,"THE SIGIL ANSWERS."));

    Gate open={.from=0,.x=1,.y=1,.to=1,.arrival_x=2,.arrival_y=2,
               .required_flag=PROGRESSION_NONE};
    assert(gate_valid(&open) && gate_can_enter(&open,&progression));
    assert(!gate_is_locked(&open,&progression));
    assert(!gate_current_dialogue(&open,&progression));

    Gate welcome=open;
    welcome.gatekeeper="GUIDE";
    welcome.unlocked_dialogue=(GateDialogue){"THE ROAD IS OPEN.",0};
    assert(gate_valid(&welcome));
    assert(gate_current_dialogue(&welcome,&progression)==&welcome.unlocked_dialogue);

    Gate bad=cave;bad.required_flag=PROGRESSION_FLAG_COUNT;
    assert(!gate_valid(&bad));
    bad=cave;bad.gatekeeper=0;assert(!gate_valid(&bad));
    bad=cave;bad.locked_dialogue.first=0;assert(!gate_valid(&bad));
    bad=cave;bad.unlocked_dialogue.first="";assert(!gate_valid(&bad));
    assert(!gate_valid(0));
    assert(!gate_can_enter(0,&progression));
    assert(!gate_current_dialogue(0,&progression));

    puts("PASS: generic locked gates, destinations, flags, gatekeepers, and state dialogue");
    return 0;
}

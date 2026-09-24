#ifndef EMBERWAKE_BATTLE_H
#define EMBERWAKE_BATTLE_H
#include <stdint.h>
#include "input.h"
#include "attacks.h"
#include "creature.h"
#include "party.h"
#include "inventory.h"
#define BATTLE_MOVES 4
typedef Creature Battler;
typedef enum { BATTLE_MESSAGE, BATTLE_MENU, BATTLE_ATTACKS, BATTLE_LEARN, BATTLE_SWITCH, BATTLE_CAPTURE, BATTLE_ITEMS, BATTLE_DONE } BattlePhase;
typedef enum { BATTLE_ONGOING, BATTLE_WIN, BATTLE_LOSS, BATTLE_ESCAPED, BATTLE_CAUGHT } BattleResult;
typedef enum { AFTER_MENU, AFTER_TURN, AFTER_GROWTH, AFTER_DONE, AFTER_BEGIN_TURN } BattleAfter;
/* Turn progression is separate from UI pages and message acknowledgements. */
typedef enum {
    TURN_BEGIN, TURN_SELECT_ENEMY, TURN_WAIT_PLAYER,
    TURN_RESOLVE_FIRST, TURN_RESOLVE_SECOND,
    TURN_CHECK_FAINTED, TURN_CHECK_RESULT, TURN_COMPLETE
} BattleTurnState;
typedef struct {
    Battler ally, enemy;
    Party party;
    Inventory inventory;
    int active, switch_cursor, forced_switch, capture_charges;
    BattlePhase phase;
    BattleResult result;
    BattleAfter after;
    uint32_t random;
    int cursor, move_cursor, item_cursor, previous_direction;
    int choices[2], order[2], turn_index;
    BattleTurnState turn_state;
    unsigned int turn_number;
    int escape_attempts;
    CreatureGrowth growth;
    int growth_stage, growth_move, learn_cursor, reward_given, experience_reward;
    float animation, hit_time, ally_hp_shown, enemy_hp_shown;
    int hit_side, acting_side; /* 0 ally, 1 enemy, -1 outside action messages. */
    char switch_message[80];
    char message[160];
} Battle;
void battler_starter(Battler *b);
void battler_restore(Battler *b);
void battle_begin(Battle *b,const Battler *ally,SpeciesId species,int level,uint32_t seed);
void battle_begin_party(Battle *b,const Party *party,SpeciesId species,int level,uint32_t seed);
void battle_begin_party_with_inventory(Battle *b,const Party *party,const Inventory *inventory,SpeciesId species,int level,uint32_t seed);
void battle_update(Battle *b,const Input *input);
int battle_damage(const Battler *attacker,const Battler *defender,const Attack *attack,int variation);
void battle_draw(const Battle *b);
void battle_animate(Battle *b,float seconds,int motion);
#endif

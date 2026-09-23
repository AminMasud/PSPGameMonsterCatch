#ifndef EMBERWAKE_BATTLE_H
#define EMBERWAKE_BATTLE_H
#include <stdint.h>
#include "input.h"
#include "attacks.h"
#include "creature.h"
#define BATTLE_MOVES 4
typedef Creature Battler;
typedef enum { BATTLE_MESSAGE, BATTLE_MENU, BATTLE_ATTACKS, BATTLE_LEARN, BATTLE_DONE } BattlePhase;
typedef enum { BATTLE_ONGOING, BATTLE_WIN, BATTLE_LOSS, BATTLE_ESCAPED } BattleResult;
typedef enum { AFTER_MENU, AFTER_TURN, AFTER_GROWTH, AFTER_DONE } BattleAfter;
typedef struct {
    Battler ally, enemy;
    BattlePhase phase;
    BattleResult result;
    BattleAfter after;
    uint32_t random;
    int cursor, move_cursor, previous_direction;
    int choices[2], order[2], turn_index;
    int escape_attempts;
    CreatureGrowth growth;
    int growth_stage, growth_move, learn_cursor, reward_given, experience_reward;
    char message[160];
} Battle;
void battler_starter(Battler *b);
void battler_restore(Battler *b);
void battle_begin(Battle *b,const Battler *ally,SpeciesId species,int level,uint32_t seed);
void battle_update(Battle *b,const Input *input);
int battle_damage(const Battler *attacker,const Battler *defender,const Attack *attack,int variation);
void battle_draw(const Battle *b);
#endif

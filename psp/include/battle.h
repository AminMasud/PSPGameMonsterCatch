#ifndef EMBERWAKE_BATTLE_H
#define EMBERWAKE_BATTLE_H
#include <stdint.h>
#include "input.h"
#include "attacks.h"
#define BATTLE_MOVES 4
typedef struct {
    const char *name;
    Element element;
    int level, max_hp, hp, attack, defense, speed;
    int moves[BATTLE_MOVES], uses[BATTLE_MOVES];
} Battler;
typedef enum { BATTLE_MESSAGE, BATTLE_MENU, BATTLE_ATTACKS, BATTLE_DONE } BattlePhase;
typedef enum { BATTLE_ONGOING, BATTLE_WIN, BATTLE_LOSS, BATTLE_ESCAPED } BattleResult;
typedef enum { AFTER_MENU, AFTER_TURN, AFTER_DONE } BattleAfter;
typedef struct {
    Battler ally, enemy;
    BattlePhase phase;
    BattleResult result;
    BattleAfter after;
    uint32_t random;
    int cursor, move_cursor, previous_direction;
    int choices[2], order[2], turn_index;
    int escape_attempts;
    char message[160];
} Battle;
void battler_starter(Battler *b);
void battler_restore(Battler *b);
void battle_begin(Battle *b,const Battler *ally,const char *enemy_name,int level,uint32_t seed);
void battle_update(Battle *b,const Input *input);
int battle_damage(const Battler *attacker,const Battler *defender,const Attack *attack,int variation);
void battle_draw(const Battle *b);
#endif

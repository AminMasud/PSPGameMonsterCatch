#include <stdio.h>
#include "party_menu.h"
#include "graphics.h"
#include "text.h"
#include "pet_draw.h"

#define C(r,g,b) (0xff000000u | ((unsigned int)(b)<<16) | ((unsigned int)(g)<<8) | (unsigned int)(r))
#define LIST_ROWS 6

static void feedback(PartyMenu *menu, const char *message)
{
    snprintf(menu->message,sizeof(menu->message),"%s",message);
}

static void normalize(PartyMenu *menu, const Party *party)
{
    const int counts[2]={party->count,party->stored};
    for(int tab=0;tab<2;++tab) {
        if(menu->cursor[tab]>=counts[tab]) menu->cursor[tab]=counts[tab]-1;
        if(menu->cursor[tab]<0) menu->cursor[tab]=0;
        if(menu->scroll[tab]>menu->cursor[tab]) menu->scroll[tab]=menu->cursor[tab];
        if(menu->cursor[tab]>=menu->scroll[tab]+LIST_ROWS)
            menu->scroll[tab]=menu->cursor[tab]-LIST_ROWS+1;
        int last=counts[tab]>LIST_ROWS?counts[tab]-LIST_ROWS:0;
        if(menu->scroll[tab]>last) menu->scroll[tab]=last;
    }
}

void party_menu_open(PartyMenu *menu)
{
    *menu=(PartyMenu){0};
    feedback(menu,"TRAVEL WITH UP TO FOUR VEYLINGS.\nMOVE OTHERS TO YOUR COLLECTION.");
}

int party_menu_update(PartyMenu *menu, Party *party, const Input *input)
{
    int horizontal=input->horizontal!=menu->previous_horizontal?input->horizontal:0;
    int vertical=input->vertical!=menu->previous_vertical?input->vertical:0;
    menu->previous_horizontal=input->horizontal;
    menu->previous_vertical=input->vertical;
    normalize(menu,party);
    if(input->menu & INPUT_MENU_OPEN) return 0;
    if(input->cancel) {
        if(menu->mode==PARTY_MENU_BROWSE) return 0;
        menu->mode=PARTY_MENU_BROWSE;
        feedback(menu,"NO CHANGE MADE.\nX CHOOSE A VEYLING.");
        return 1;
    }
    if(menu->mode==PARTY_MENU_ACTION) {
        if(vertical) menu->action=(menu->action+vertical+3)%3;
        if(!input->confirm) return 1;
        if(menu->action==0) {
            if(party_set_lead(party,menu->cursor[0]))
                feedback(menu,"LEAD PARTNER UPDATED.\nTHIS VEYLING STARTS THE NEXT BATTLE.");
            else feedback(menu,"THAT VEYLING CANNOT LEAD.\nCHOOSE A PARTNER WITH HP.");
        } else if(menu->action==1) {
            if(party->count<=1) {
                feedback(menu,"KEEP AT LEAST ONE VEYLING IN YOUR PARTY.\nCATCH ANOTHER BEFORE DEPOSITING THIS ONE.");
                return 1;
            }
            if(party->stored>=COLLECTION_MAX) {
                feedback(menu,"YOUR COLLECTION IS FULL.\nWITHDRAW A VEYLING TO MAKE ROOM.");
                return 1;
            }
            if(party_deposit(party,menu->cursor[0]))
                feedback(menu,"VEYLING MOVED TO YOUR COLLECTION.\nUSE LEFT OR RIGHT TO VIEW IT.");
            else feedback(menu,"THIS VEYLING MUST STAY IN YOUR PARTY.\nCHOOSE ANOTHER PARTNER.");
        } else feedback(menu,"NO CHANGE MADE.\nX CHOOSE A VEYLING.");
        menu->mode=PARTY_MENU_BROWSE;
        normalize(menu,party);
        return 1;
    }
    if(menu->mode==PARTY_MENU_SWAP) {
        if(vertical && party->count>0) menu->swap=(menu->swap+vertical+party->count)%party->count;
        if(input->confirm) {
            if(party_exchange(party,menu->cursor[1],menu->swap))
                feedback(menu,"VEYLINGS EXCHANGED.\nYOUR NEW PARTNER IS READY TO TRAVEL.");
            else feedback(menu,"EXCHANGE COULD NOT BE COMPLETED.\nCHOOSE ANOTHER PARTNER.");
            menu->mode=PARTY_MENU_BROWSE;
            normalize(menu,party);
        }
        return 1;
    }
    if(horizontal) {
        menu->tab=1-menu->tab;
        feedback(menu,menu->tab?"X WITHDRAW A VEYLING.\nIF THE PARTY IS FULL, CHOOSE ONE TO SWAP.":
                 "X CHOOSE A VEYLING.\nSET YOUR LEAD OR MOVE IT TO COLLECTION.");
    }
    int count=menu->tab?party->stored:party->count;
    if(vertical && count>0) {
        menu->cursor[menu->tab]=(menu->cursor[menu->tab]+vertical+count)%count;
        normalize(menu,party);
    }
    if(!input->confirm) return 1;
    if(count==0) {
        feedback(menu,"YOUR COLLECTION IS EMPTY.\nCATCH A VEYLING OR DEPOSIT A PARTY MEMBER.");
    } else if(menu->tab==0) {
        menu->mode=PARTY_MENU_ACTION;
        menu->action=0;
        feedback(menu,"SET LEAD TO CHOOSE YOUR FIRST BATTLER.\nDEPOSIT MOVES THIS VEYLING TO COLLECTION.");
    } else if(party->count<PARTY_MAX) {
        if(party_exchange(party,menu->cursor[1],party->count))
            feedback(menu,"VEYLING ADDED TO YOUR PARTY.\nSWITCH TO PARTY TO SET YOUR LEAD.");
        else feedback(menu,"WITHDRAWAL COULD NOT BE COMPLETED.\nCHOOSE ANOTHER VEYLING.");
        normalize(menu,party);
    } else {
        menu->mode=PARTY_MENU_SWAP;
        menu->swap=0;
        feedback(menu,"YOUR PARTY IS FULL. CHOOSE WHO TO SWAP.\nTHE OTHER VEYLING MOVES TO COLLECTION.");
    }
    return 1;
}

static void details_draw(const Creature *creature)
{
    char line[80];
    snprintf(line,sizeof(line),"%.19s  LV %d",creature_name(creature),creature->level);
    text_draw(244,72,line,C(244,217,169),1);
    snprintf(line,sizeof(line),"%s  STAGE %d/3",element_name(creature->element),creature->species%3+1);
    text_draw(244,87,line,C(166,196,188),1);
    pet_draw(creature->species,388,69,64,0);
    snprintf(line,sizeof(line),"HP %d / %d",creature->hp,creature->max_hp);
    text_draw(244,100,line,C(229,233,220),1);
    graphics_rectangle(244,112,130,5,C(57,70,77));
    if(creature->max_hp>0 && creature->hp>0) {
        int filled=130*creature->hp/creature->max_hp;
        if(filled>130) filled=130;
        if(filled>0) graphics_rectangle(244,112,filled,5,C(125,201,154));
    }
    snprintf(line,sizeof(line),"ATK %d  DEF %d  SPD %d",creature->attack,creature->defense,creature->speed);
    text_draw(244,135,line,C(207,216,209),1);
    if(creature->level>=CREATURE_MAX_LEVEL) snprintf(line,sizeof(line),"XP %d - MAX LEVEL",creature->experience);
    else snprintf(line,sizeof(line),"XP %d - NEXT %d",creature->experience,creature_xp_remaining(creature));
    text_draw(244,148,line,C(233,173,115),1);
    for(int i=0;i<CREATURE_MOVES;++i) {
        if(creature->moves[i]<0) snprintf(line,sizeof(line),"- EMPTY -");
        else {
            const Attack *move=attack_get(creature->moves[i]);
            snprintf(line,sizeof(line),"%.22s %d/%d",move->name,creature->uses[i],move->uses);
        }
        text_draw(244,163+i*11,line,C(188,205,197),1);
    }
}

void party_menu_draw_battle(const Party *party,int active,int cursor,int forced,
                            const char *message)
{
    char line[80];
    if(party->count<=0) return;
    if(cursor<0 || cursor>=party->count) cursor=0;
    graphics_rectangle(0,0,480,272,C(17,26,33));
    text_draw(14,12,"BATTLE PARTY",C(241,221,184),2);
    text_draw(254,18,forced?"CHOOSE A READY REPLACEMENT":"CHOOSE WHO WILL TAKE THE FIELD",C(165,188,181),1);
    graphics_rectangle(12,52,216,157,C(25,35,45));
    graphics_rectangle(238,52,230,157,C(25,35,45));
    for(int i=0;i<party->count;++i) {
        const Creature *member=&party->members[i];
        int y=57+i*37;
        if(i==cursor) graphics_rectangle(16,y,208,34,C(59,78,76));
        pet_draw(member->species,20,y+2,30,0);
        snprintf(line,sizeof(line),"%.18s  LV %d",creature_name(member),member->level);
        text_draw(55,y+5,line,C(235,230,209),1);
        snprintf(line,sizeof(line),"HP %d/%d  %s",member->hp,member->max_hp,
                 i==active?"ACTIVE":member->hp<=0?"NEEDS REST":"READY");
        text_draw(55,y+17,line,i==active?C(255,201,132):member->hp<=0?C(214,132,123):C(168,199,183),1);
        graphics_rectangle(55,y+27,150,4,C(57,70,77));
        if(member->max_hp>0 && member->hp>0) {
            int filled=150*member->hp/member->max_hp;
            if(filled>150) filled=150;
            graphics_rectangle(55,y+27,filled,4,C(125,201,154));
        }
    }
    details_draw(&party->members[cursor]);
    graphics_rectangle(12,214,456,32,C(32,44,49));
    if(message && message[0]) text_draw(21,220,message,C(238,198,137),1);
    else if(cursor==active) text_draw(21,220,"THIS VEYLING IS ALREADY ACTIVE.\nCHOOSE ANOTHER PARTNER.",C(238,198,137),1);
    else if(party->members[cursor].hp<=0) text_draw(21,220,"THIS VEYLING NEEDS A REST.\nCHOOSE A READY PARTNER.",C(214,151,137),1);
    else text_draw(21,220,forced?"X SEND OUT THIS VEYLING.":"X SWAP - THIS USES YOUR ACTION.",C(220,213,190),1);
    text_draw(12,255,forced?"UP/DOWN SELECT   X SEND OUT - REPLACEMENT REQUIRED":
              "UP/DOWN SELECT   X SWAP   O BACK",C(161,189,181),1);
}

static void actions_draw(const PartyMenu *menu, const Party *party)
{
    graphics_rectangle(238,64,230,145,C(174,143,94));
    graphics_rectangle(240,66,226,141,C(25,35,45));
    if(menu->mode==PARTY_MENU_ACTION) {
        const char *const options[]={"SET AS LEAD","MOVE TO COLLECTION","CANCEL"};
        text_draw(250,76,"PARTY ACTION",C(246,206,146),1);
        for(int i=0;i<3;++i) {
            if(i==menu->action) graphics_rectangle(246,97+i*30,214,24,C(67,85,80));
            text_draw(254,105+i*30,options[i],C(238,229,207),1);
        }
    } else {
        text_draw(250,76,"SWAP WITH PARTY MEMBER",C(246,206,146),1);
        for(int i=0;i<party->count;++i) {
            char line[64];
            if(i==menu->swap) graphics_rectangle(246,93+i*27,214,24,C(67,85,80));
            snprintf(line,sizeof(line),"%.19s LV %d%s",creature_name(&party->members[i]),party->members[i].level,i==party->lead?" LEAD":"");
            text_draw(251,102+i*27,line,C(238,229,207),1);
        }
    }
}

void party_menu_draw(const PartyMenu *menu, const Party *party)
{
    char line[80];
    int count=menu->tab?party->stored:party->count;
    graphics_rectangle(0,0,480,272,C(17,26,33));
    text_draw(14,12,"YOUR VEYLINGS",C(241,221,184),2);
    text_draw(302,18,"PARTNERS FOR THE JOURNEY",C(165,188,181),1);
    for(int tab=0;tab<2;++tab) {
        int x=12+tab*230;
        graphics_rectangle(x,39,224,20,tab==menu->tab?C(76,86,78):C(29,41,49));
        snprintf(line,sizeof(line),tab==0?"PARTY  %d / %d":"COLLECTION  %d / %d",
                 tab==0?party->count:party->stored,tab==0?PARTY_MAX:COLLECTION_MAX);
        text_draw(x+10,46,line,tab==menu->tab?C(255,214,146):C(177,194,188),1);
    }
    graphics_rectangle(12,64,216,145,C(25,35,45));
    graphics_rectangle(238,64,230,145,C(25,35,45));
    if(count==0) {
        text_draw(22,80,"NO VEYLINGS STORED",C(234,219,191),1);
        text_draw(22,100,"CATCH NEW PARTNERS OR\nMOVE ONE HERE FROM\nYOUR PARTY.",C(171,194,183),1);
        text_draw(249,85,"ROOM FOR FUTURE PARTNERS.",C(238,214,176),1);
        text_draw(249,108,"EXTRA CAPTURES GO HERE\nWHEN YOUR PARTY IS FULL.\n\nYOU CAN WITHDRAW OR SWAP\nTHEM INTO YOUR PARTY.",C(171,194,183),1);
    } else {
        for(int row=0;row<LIST_ROWS;++row) {
            int index=menu->scroll[menu->tab]+row;
            if(index>=count) break;
            int y=67+row*23;
            const Creature *creature=menu->tab?&party->collection[index]:&party->members[index];
            if(index==menu->cursor[menu->tab]) graphics_rectangle(16,y,208,22,C(59,78,76));
            pet_draw(creature->species,20,y,22,0);
            snprintf(line,sizeof(line),"%02d %.19s",index+1,creature_name(creature));
            text_wrap(46,y+3,144,9,line,C(235,230,209),1);
            if(menu->tab==0 && index==party->lead) text_draw(193,y+3,"LEAD",C(255,201,132),1);
            snprintf(line,sizeof(line),"LV %d  HP %d/%d",creature->level,creature->hp,creature->max_hp);
            text_draw(46,y+13,line,C(168,199,183),1);
        }
        const Creature *selected=menu->tab?&party->collection[menu->cursor[1]]:&party->members[menu->cursor[0]];
        details_draw(selected);
        if(menu->mode!=PARTY_MENU_BROWSE) actions_draw(menu,party);
    }
    graphics_rectangle(12,214,456,32,C(32,44,49));
    text_draw(21,220,menu->message,C(220,213,190),1);
    text_draw(12,255,menu->mode==PARTY_MENU_BROWSE?
              "L/R TAB   UP/DOWN SELECT   X ACTION   O BACK   TRIANGLE CLOSE":
              "UP/DOWN SELECT   X CONFIRM   O CANCEL   TRIANGLE CLOSE",C(161,189,181),1);
}

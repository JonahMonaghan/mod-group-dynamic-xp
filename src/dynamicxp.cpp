/*
Credits

GroupDynamicXP
Script reworked by BreakingBeaker
Module created by BreakingBeaker

DynamicXP - https://github.com/azerothcore/mod-dynamic-xp
Script reworked by Micrah/Milestorme and Poszer (Poszer is the Best)
Module Created by Micrah/Milestorme

Original Script from AshmaneCore https://github.com/conan513 Single Player Project
*/

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Group.h"


class spp_dynamic_xp_rate : public PlayerScript
{
public:
    spp_dynamic_xp_rate() : PlayerScript("spp_dynamic_xp_rate") { };

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("Dynamic.XP.Rate.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Level Dynamic XP |rmodule.");
            if(sConfigMgr->GetOption<uint32>("Dynamic.XP.Group", true)){
                ChatHandler(player->GetSession()).SendSysMessage("Group scaling is |ccf4CFF00Enabled|r.");
            }else{
                ChatHandler(player->GetSession()).SendSysMessage("Group scaling is |ccfFF0000Disabled|r.");
            }
        }
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/, uint8 /*xpSource*/) override
    {

        //Less function calls
        uint32 level = player->GetLevel();
        Group* group = player->GetGroup();

        if(group != nullptr){        
            //Incentivize group play over solo leveling
            if(sConfigMgr->GetOption<bool>("Dynamic.XP.Group", true)){
                uint32 partySize = group->GetMembersCount();
                amount *= partySize * 2;
            }

            //Keep level gaps of group standard
            uint32 groupScale = sConfigMgr->GetOption<uint32>("Dynamic.XP.Group.Scale", 1);

            if(groupScale == 1){
                group->DoForAllMembers([&](Player* member){
                    if(member->GetLevel() < level){
                        level = member->GetLevel();
                    }
                });
            }else if(groupScale == 2){
                group->DoForAllMembers([&](Player* member){
                    if(member->GetLevel() > level){
                        level = member->GetLevel();
                    }
                });
            }
        }

        if (sConfigMgr->GetOption<bool>("Dynamic.XP.Rate", true))
        {
            if (level <= 9)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.1-9", 1);
            else if (level <= 19)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.10-19", 2);
            else if (level <= 29)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.20-29", 3);
            else if (level <= 39)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.30-39", 4);
            else if (level <= 49)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.40-49", 5);
            else if (level <= 59)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.50-59", 6);
            else if (level <= 69)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.60-69", 7);
            else if (level <= 79)
                amount *= sConfigMgr->GetOption<uint32>("Dynamic.XP.Rate.70-79", 8);
        }
    }
};

void AddSC_group_dynamic_xp_rate()
{
    new spp_dynamic_xp_rate();
}

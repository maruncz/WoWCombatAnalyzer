#ifndef DEFS_H
#define DEFS_H

#include <QString>
#include <QStringList>


enum class SubEvent
{
SPELL_CAST_SUCCESS,
    SPELL_AURA_APPLIED,
    SPELL_AURA_APPLIED_DOSE,
    SPELL_AURA_REMOVED,
    SPELL_AURA_REFRESH,
    SPELL_DAMAGE,
    SPELL_ENERGIZE,
    SPELL_PERIODIC_DAMAGE,
    SWING_DAMAGE,
    DAMAGE_SHIELD,
    SPELL_PERIODIC_HEAL,
    SPELL_CAST_START,
    SPELL_PERIODIC_ENERGIZE,
    SPELL_HEAL,
    SWING_MISSED,
    PARTY_KILL,
    UNIT_DIED,
    SPELL_CAST_FAILED,
    ENCHANT_APPLIED,
    SPELL_MISSED,
    SPELL_INTERRUPT,
    SPELL_PERIODIC_MISSED,
    SPELL_DISPEL,
    SPELL_SUMMON,
    DAMAGE_SHIELD_MISSED,
    SPELL_PERIODIC_LEECH
};

#if 0
enum UnitFlags
{
    COMBATLOG_OBJECT_AFFILIATION_MINE     =  0x00000001	,
    COMBATLOG_OBJECT_AFFILIATION_PARTY   =   0x00000002	,
    COMBATLOG_OBJECT_AFFILIATION_RAID     =  0x00000004	,
    COMBATLOG_OBJECT_AFFILIATION_OUTSIDER =	 0x00000008	,
    COMBATLOG_OBJECT_AFFILIATION_MASK    =   0x0000000F	,
    COMBATLOG_OBJECT_REACTION_FRIENDLY   =   0x00000010	,
    COMBATLOG_OBJECT_REACTION_NEUTRAL   =    0x00000020	,
    COMBATLOG_OBJECT_REACTION_HOSTILE    =   0x00000040	,
    COMBATLOG_OBJECT_REACTION_MASK      =    0x000000F0	,
    COMBATLOG_OBJECT_CONTROL_PLAYER     =    0x00000100	,
    COMBATLOG_OBJECT_CONTROL_NPC       =     0x00000200	,
    COMBATLOG_OBJECT_CONTROL_MASK      =     0x00000300	,
    COMBATLOG_OBJECT_TYPE_PLAYER       =     0x00000400	,//Units directly controlled by players.
    COMBATLOG_OBJECT_TYPE_NPC          =     0x00000800	,//Units controlled by the server.
    COMBATLOG_OBJECT_TYPE_PET         =      0x00001000	,//Pets are units controlled by a player or NPC, including via mind control.
    COMBATLOG_OBJECT_TYPE_GUARDIAN    =      0x00002000	,//Units that are not controlled, but automatically defend their master.
    COMBATLOG_OBJECT_TYPE_OBJECT       =     0x00004000	,//Objects are everything else, such as traps and totems.
    COMBATLOG_OBJECT_TYPE_MASK        =      0x0000FC00	,
    COMBATLOG_OBJECT_TARGET           =      0x00010000	,
    COMBATLOG_OBJECT_FOCUS            =      0x00020000	,
    COMBATLOG_OBJECT_MAINTANK         =      0x00040000	,
    COMBATLOG_OBJECT_MAINASSIST       =      0x00080000	,
    COMBATLOG_OBJECT_NONE             =      0x80000000	,//Whether the unit does not exist.
    COMBATLOG_OBJECT_SPECIAL_MASK     =      0xFFFF0000
};
#endif

enum RaidFlags
{
    COMBATLOG_OBJECT_RAIDTARGET1   =     0x00000001,
    COMBATLOG_OBJECT_RAIDTARGET2    =    0x00000002,
    COMBATLOG_OBJECT_RAIDTARGET3   =     0x00000004,
    COMBATLOG_OBJECT_RAIDTARGET4   =     0x00000008,
    COMBATLOG_OBJECT_RAIDTARGET5   =     0x00000010,
    COMBATLOG_OBJECT_RAIDTARGET6   =     0x00000020,
    COMBATLOG_OBJECT_RAIDTARGET7   =     0x00000040,
    COMBATLOG_OBJECT_RAIDTARGET8   =     0x00000080,
    COMBATLOG_OBJECT_RAIDTARGET_MASK=	0x000000FF
};


enum class SpellSchool
{
    Physical = 1,
    Holy    =2,
    Fire   =4,
    Nature =8,
    Frost =16,
    Shadow =32  ,
    Arcane  =64 ,

    Holystrike	 = Holy | Physical,
    Flamestrike	= Fire | Physical,
    Radiant        = Fire | Holy ,
    Holyfire = Radiant,
    Stormstrike	 = Nature | Physical,
    Holystorm	= Nature | Holy,
    Volcanic	= Nature | Fire,
    Froststrike	= Frost | Physical,
    Holyfrost	= Frost | Holy,
    Frostfire	= Frost | Fire,
    Froststorm	= Frost | Nature,
    Shadowstrike	= Shadow | Physical,
    Twilight        = Shadow | Holy,
    Shadowlight        = Shadow | Holy,
    Shadowflame	= Shadow | Fire,
    Plague          = Shadow | Nature,
    Shadowstorm          = Shadow | Nature,
    Shadowfrost	= Shadow | Frost,
    Spellstrike	= Arcane | Physical,
    Divine          = Arcane | Holy,
    Spellfire	= Arcane | Fire,
    Astral          = Arcane | Nature,
    Spellstorm          = Arcane | Nature,
    Spellfrost	= Arcane | Frost,
    Spellshadow	= Arcane |Shadow,


};



enum class PowerType
{
    HealthCost	= -2,
    None	= -1,
    Mana         =0           ,
    Rage             =1       ,
    Focus            =2       ,
    Energy            =3      ,
    ComboPoints       =4      ,
    Runes             =5      ,
    RunicPower        =6      ,
    SoulShards        =7      ,
    LunarPower         =8     ,
    HolyPower          =9     ,
    Alternate          =10     ,
    Maelstrom          =11     ,
    Chi                =12     ,
    Insanity           =13     ,
    Obsolete           =14     ,
    Obsolete2          =15     ,
    ArcaneCharges      =16     ,
    Fury               =17     ,
    Pain               =18     ,
    Essence            =19     ,
    RuneBlood          =20     ,
    RuneFrost          =21     ,
    RuneUnholy         =22     ,
    AlternateQuest     =23     ,
    AlternateEncounter =24	,
    AlternateMount      =25    ,
    NumPowerTypes	=26
};

enum class MissType
{
    ABSORB,
    BLOCK,
    DEFLECT,
    DODGE,
    EVADE,
    IMMUNE,
    MISS,
    PARRY,
    REFLECT,
    RESIST
};

enum class AuraType
{
    BUFF,
    DEBUFF
};

enum class EnvironmentalType
{
    DROWNING,
    FALLING,
    FATIGUE,
    FIRE,
    LAVA,
    SLIME
};


#endif // DEFS_H

#ifndef SUBEVENTS_H
#define SUBEVENTS_H

#include <cstdint>
#include <QString>
#include "defs.h"
#include <variant>


struct UnitFlags
{
    struct Affiliation
    {
        static constexpr uint8_t mine{0x1};
        static constexpr uint8_t party{0x2};
        static constexpr uint8_t raid{0x4};
        static constexpr uint8_t outsider{0x8};
        static constexpr uint8_t mask{0xf};
    };

    struct Reaction
    {
        static constexpr uint32_t friendly{0x10};
        static constexpr uint32_t neutral{0x20};
        static constexpr uint32_t hostile{0x40};
        static constexpr uint32_t mask{0xF0};
        static constexpr uint32_t unused{0x80};
    };

    struct Control
    {
        static constexpr uint32_t player{0x100};
        static constexpr uint32_t npc{0x200};
        static constexpr uint32_t mask{0x300};
    };

    struct Type
    {
        static constexpr uint32_t player{0x400};
        static constexpr uint32_t npc{0x800};
        static constexpr uint32_t pet{0x1000};
        static constexpr uint32_t guardian{0x2000};
        static constexpr uint32_t object{0x4000};
        static constexpr uint32_t mask{0xFC00};
        static constexpr uint32_t unused{0x8000};
    };

    struct Special
    {
        static constexpr uint32_t target{0x10000};
        static constexpr uint32_t focus{0x20000};
        static constexpr uint32_t mainTank{0x40000};
        static constexpr uint32_t mainAssist{0x80000};
        static constexpr uint32_t none{0x80000000};

        static constexpr uint32_t mask{0xFFFF0000};
    };


    [[nodiscard]] static UnitFlags fromNum(uint32_t n);

    uint32_t value{0};

};





struct Object
{
    [[nodiscard]] static Object fromString(QStringList list);

    uint64_t guid;
    QString name;
    UnitFlags flags;
    RaidFlags raidFlags;

private:

};

struct Spell
{
    Spell(QStringList list);
    uint32_t id;
    QString name;
    uint8_t school;
};

struct Item
{
    Item(QStringList list);
    uint32_t id;
    QString name;
};



namespace detail
{






namespace prefix
{
struct Swing{};

struct Range
{
    Range(QStringList list) : spell{list} {}
    Spell spell;
};

using Spell = Range;
using SpellPeriodic = Range;
using SpellBuilding = Range;

struct Environmental
{
    EnvironmentalType type;
};

}

namespace suffix
{

struct Damage
{
    Damage(QStringList list);
    uint32_t amount;
    uint32_t overkill;
    SpellSchool spelschool;
    uint32_t resisted;
    uint32_t blocked;
    uint32_t absorbed;
    bool critical;
    bool glancing;
    bool crushing;
    //bool isOffHand;
};

struct Missed
{
    Missed(QStringList list);
    MissType type;
    //bool isOffHand;
    uint32_t amountMissed{0};
    //bool critical;
};

struct Heal
{
    Heal(QStringList list);
    uint32_t amount;
    uint32_t overhealing;
    uint32_t absorbed;
    bool critical;
};

struct HealAbsorbed
{
    Object extra;
    Spell extraSpell;
    uint32_t absorbedAmount;
    uint32_t totalAmount;
};

struct Absorbed{};

struct Energize
{
    Energize(QStringList list);
    uint32_t amount;
    //uint32_t overEnergize;
    PowerType type;
    //uint32_t maxPower;
};

struct Drain
{
    Drain(QStringList list);
    uint32_t amount;
    PowerType type;
    uint32_t extraAmount;
    //uint32_t maxPower;
};

using Leech = Drain;

struct Interrupt
{
    Interrupt(QStringList list);
    Spell extraSpell;
};

struct Dispell
{
    Dispell(QStringList list);
    Spell extraSpell;
    AuraType aura;
};

using DispellFailed = Interrupt;
using Stolen = Dispell;

struct ExtraAttacks
{
    uint32_t amount;
};

struct AuraApplied
{
    AuraApplied(QStringList list);
    AuraType type;
    //int amount;
};

using AuraRemoved = AuraApplied;
struct AuraAppliedDose : public AuraApplied
{
    AuraAppliedDose(QStringList list);
    uint32_t amount;
};

using AuraRemovedDose = AuraAppliedDose;

using AuraRefresh = AuraApplied;

#if 0
class AuraRefresh
{
    AuraType type;
};
#endif

using AuraBroken = AuraRefresh;

struct AuraBrokenSpell
{
    Spell extra;
    AuraType type;
};

using CastStart = Absorbed;
using CastSucces = Absorbed;
struct CastFailed
{
    CastFailed(QStringList list);
    QString type;
};

using Instakill = Absorbed;
using DurabilityDamage = Absorbed;
using DurabilityDamageAll = Absorbed;
using Create = Absorbed;
using Summon = Absorbed;
using Resurrect = Absorbed;
using Instakill = Absorbed;


}






}




struct SpellCastSucces : public detail::prefix::Spell, public detail::suffix::CastSucces
{
    SpellCastSucces(QStringList list) : detail::prefix::Spell{list} {}
};

struct SpellDamage : public detail::prefix::Spell, public detail::suffix::Damage
{
    SpellDamage(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::Damage{list.mid(3)}
    {}
};

struct SpellPeriodicDamage : public detail::prefix::SpellPeriodic, public detail::suffix::Damage
{
    SpellPeriodicDamage(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Damage{list.mid(3)}
    {}
};

struct SpellAuraApplied : public detail::prefix::Spell , public detail::suffix::AuraApplied
{
    SpellAuraApplied(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraApplied{list.mid(3)}
    {}
};

struct SpellAuraRemoved : public detail::prefix::Spell , public detail::suffix::AuraRemoved
{
    SpellAuraRemoved(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraRemoved{list.mid(3)}
    {}
};

struct SpellAuraRefresh : public detail::prefix::Spell , public detail::suffix::AuraRefresh
{
    SpellAuraRefresh(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraRefresh{list.mid(3)}
    {}
};

struct SpellEnergize : public detail::prefix::Spell, public detail::suffix::Energize
{
    SpellEnergize(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::Energize{list.mid(3)}
    {}
};

struct SwingDamage : public detail::prefix::Swing, public detail::suffix::Damage
{
    SwingDamage(QStringList list) : detail::prefix::Swing{} , detail::suffix::Damage{list}
    {}
};

struct SpellPeriodicHeal : public detail::prefix::SpellPeriodic, public detail::suffix::Heal
{
    SpellPeriodicHeal(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Heal{list.mid(3)}
    {}
};

struct SpellAuraAppliedDose : public detail::prefix::Spell , public detail::suffix::AuraAppliedDose
{
    SpellAuraAppliedDose(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraAppliedDose{list.mid(3)}
    {}
};

struct SpellCastStart : public detail::prefix::Spell, public detail::suffix::CastStart
{
    SpellCastStart(QStringList list) : detail::prefix::Spell{list}
    {

    }
};


struct SpellPeriodicEnergize : public detail::prefix::SpellPeriodic, public detail::suffix::Energize
{
    SpellPeriodicEnergize(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Energize{list.mid(3)}
    {}
};

struct SpellHeal : public detail::prefix::Spell, public detail::suffix::Heal
{
    SpellHeal(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::Heal{list.mid(3)}
    {}
};

struct SwingMissed : public detail::prefix::Swing, public detail::suffix::Missed
{
    SwingMissed(QStringList list) : detail::prefix::Swing{}, detail::suffix::Missed{list}
    {

    }
};

struct PartyKill
{
    PartyKill(QStringList list);
};

struct SpellCastFailed : public detail::prefix::Spell, public detail::suffix::CastFailed
{
    SpellCastFailed(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::CastFailed{list.mid(3)}
    {

    }

};

struct EnchantApplied
{
    EnchantApplied(QStringList list);
    QString name;
    Item item;
};

struct SpellMissed : public detail::prefix::Spell, public detail::suffix::Missed
{
    SpellMissed(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::Missed{list.mid(3)}
    {

    }
};

struct SpellInterrupt : public detail::prefix::Spell, public detail::suffix::Interrupt
{
    SpellInterrupt(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::Interrupt{list.mid(3)}
    {

    }
};

struct SpellPeriodicMissed : public detail::prefix::SpellPeriodic, public detail::suffix::Missed
{
    SpellPeriodicMissed(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Missed{list.mid(3)}
    {}
};

struct SpellDispel : public detail::prefix::Spell, public detail::suffix::Dispell
{
    SpellDispel(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::Dispell{list.mid(3)}
    {

    }

};

struct SpellSummon : public detail::prefix::Spell, public detail::suffix::Summon
{
    SpellSummon(QStringList list) : detail::prefix::Spell{list} , detail::suffix::Summon{}
    {

    }
};

struct SpellPeriodicLeech : public detail::prefix::SpellPeriodic, public detail::suffix::Leech
{
    SpellPeriodicLeech(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Leech{list.mid(3)}
    {

    }
};


struct SpellResurrect : public detail::prefix::Spell, public detail::suffix::Resurrect
{
    SpellResurrect(QStringList list) : detail::prefix::Spell{list} , detail::suffix::Resurrect{}
    {

    }
};



using variant_t = std::variant<std::monostate,SpellCastSucces,SpellDamage,SpellPeriodicDamage,
                               SpellAuraApplied,SpellAuraRemoved, SpellAuraRefresh,SpellEnergize,
                               SwingDamage,SpellPeriodicHeal,SpellAuraAppliedDose,SpellCastStart,
                               SpellPeriodicEnergize, SpellHeal, SwingMissed, PartyKill,SpellCastFailed,
                               EnchantApplied,SpellMissed, SpellInterrupt,SpellPeriodicMissed,
                               SpellDispel,SpellSummon,SpellPeriodicLeech, SpellResurrect>;

using DamageShield = SpellDamage;
using UnitDied = PartyKill;
using DamageShieldMissed = SpellMissed;
using SpellCreate = SpellSummon;
using RangeDamage = SpellDamage;
using SpellInstakill = SpellSummon;
using EnchantRemoved = EnchantApplied;


#endif // SUBEVENTS_H

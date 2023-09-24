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

        static constexpr uint32_t unknownArthasCullongOfStratholme{0x400000};

        static constexpr uint32_t unused{0x7FB00000};
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
class Swing{};

class Range
{
public:
    Range(QStringList list) : spell{list} {}
private:
    Spell spell;
};

using Spell = Range;
using SpellPeriodic = Range;
using SpellBuilding = Range;

class Environmental
{
    EnvironmentalType type;
};

}

namespace suffix
{

class Damage
{
public:
    Damage(QStringList list);
private:
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

class Missed
{
public:
    Missed(QStringList list);
private:
    MissType type;
    //bool isOffHand;
    uint32_t amountMissed{0};
    //bool critical;
};

class Heal
{
public:
    Heal(QStringList list);
private:
    uint32_t amount;
    uint32_t overhealing;
    uint32_t absorbed;
    bool critical;
};

class HealAbsorbed
{
    Object extra;
    Spell extraSpell;
    uint32_t absorbedAmount;
    uint32_t totalAmount;
};

class Absorbed{};

class Energize
{
public:
    Energize(QStringList list);
private:
    uint32_t amount;
    //uint32_t overEnergize;
    PowerType type;
    //uint32_t maxPower;
};

class Drain
{
public:
    Drain(QStringList list);
private:
    uint32_t amount;
    PowerType type;
    uint32_t extraAmount;
    //uint32_t maxPower;
};

using Leech = Drain;

class Interrupt
{
public:
    Interrupt(QStringList list);
private:
    Spell extraSpell;
};

class Dispell
{
public:
    Dispell(QStringList list);
private:
    Spell extraSpell;
    AuraType aura;
};

using DispellFailed = Interrupt;
using Stolen = Dispell;

class ExtraAttacks
{
    uint32_t amount;
};

class AuraApplied
{
public:
    AuraApplied(QStringList list);
private:
    AuraType type;
    //int amount;
};

using AuraRemoved = AuraApplied;
class AuraAppliedDose : public AuraApplied
{
public:
    AuraAppliedDose(QStringList list);
private:
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

class AuraBrokenSpell
{
    Spell extra;
    AuraType type;
};

using CastStart = Absorbed;
using CastSucces = Absorbed;
class CastFailed
{
public:
    CastFailed(QStringList list);
private:
    QString type;
};

using Instakill = Absorbed;
using DurabilityDamage = Absorbed;
using DurabilityDamageAll = Absorbed;
using Create = Absorbed;
using Summon = Absorbed;
using Resurrect = Absorbed;

}






}




class SpellCastSucces : public detail::prefix::Spell, public detail::suffix::CastSucces
{
public:
    SpellCastSucces(QStringList list) : detail::prefix::Spell{list} {}
};

class SpellDamage : public detail::prefix::Spell, public detail::suffix::Damage
{
public:
    SpellDamage(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::Damage{list.mid(3)}
    {}
};

class SpellPeriodicDamage : public detail::prefix::SpellPeriodic, public detail::suffix::Damage
{
public:
    SpellPeriodicDamage(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Damage{list.mid(3)}
    {}
};

class SpellAuraApplied : public detail::prefix::Spell , public detail::suffix::AuraApplied
{
public:
    SpellAuraApplied(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraApplied{list.mid(3)}
    {}
};

class SpellAuraRemoved : public detail::prefix::Spell , public detail::suffix::AuraRemoved
{
public:
    SpellAuraRemoved(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraRemoved{list.mid(3)}
    {}
};

class SpellAuraRefresh : public detail::prefix::Spell , public detail::suffix::AuraRefresh
{
public:
    SpellAuraRefresh(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraRefresh{list.mid(3)}
    {}
};

class SpellEnergize : public detail::prefix::Spell, public detail::suffix::Energize
{
public:
    SpellEnergize(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::Energize{list.mid(3)}
    {}
};

class SwingDamage : public detail::prefix::Swing, public detail::suffix::Damage
{
public:
    SwingDamage(QStringList list) : detail::prefix::Swing{} , detail::suffix::Damage{list}
    {}
};

class SpellPeriodicHeal : public detail::prefix::SpellPeriodic, public detail::suffix::Heal
{
public:
    SpellPeriodicHeal(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Heal{list.mid(3)}
    {}
};

class SpellAuraAppliedDose : public detail::prefix::Spell , public detail::suffix::AuraAppliedDose
{
public:
    SpellAuraAppliedDose(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::AuraAppliedDose{list.mid(3)}
    {}
};

class SpellCastStart : public detail::prefix::Spell, public detail::suffix::CastStart
{
public:
    SpellCastStart(QStringList list) : detail::prefix::Spell{list}
    {

    }
};


class SpellPeriodicEnergize : public detail::prefix::SpellPeriodic, public detail::suffix::Energize
{
public:
    SpellPeriodicEnergize(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Energize{list.mid(3)}
    {}
};

class SpellHeal : public detail::prefix::Spell, public detail::suffix::Heal
{
public:
    SpellHeal(QStringList list) : detail::prefix::Spell{list.mid(0,3)} , detail::suffix::Heal{list.mid(3)}
    {}
};

class SwingMissed : public detail::prefix::Swing, public detail::suffix::Missed
{
public:
    SwingMissed(QStringList list) : detail::prefix::Swing{}, detail::suffix::Missed{list}
    {

    }
};

class PartyKill
{
public:
    PartyKill(QStringList list);
};

class SpellCastFailed : public detail::prefix::Spell, public detail::suffix::CastFailed
{
public:
    SpellCastFailed(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::CastFailed{list.mid(3)}
    {

    }

};

class EnchantApplied
{
public:
    EnchantApplied(QStringList list);
private:
    QString name;
    Item item;
};

class SpellMissed : public detail::prefix::Spell, public detail::suffix::Missed
{
public:
    SpellMissed(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::Missed{list.mid(3)}
    {

    }
};

class SpellInterrupt : public detail::prefix::Spell, public detail::suffix::Interrupt
{
public:
    SpellInterrupt(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::Interrupt{list.mid(3)}
    {

    }
};

class SpellPeriodicMissed : public detail::prefix::SpellPeriodic, public detail::suffix::Missed
{
public:
    SpellPeriodicMissed(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Missed{list.mid(3)}
    {}
};

class SpellDispel : public detail::prefix::Spell, public detail::suffix::Dispell
{
public:
    SpellDispel(QStringList list) : detail::prefix::Spell{list.mid(0,3)}, detail::suffix::Dispell{list.mid(3)}
    {

    }

};

class SpellSummon : public detail::prefix::Spell, public detail::suffix::Summon
{
public:
    SpellSummon(QStringList list) : detail::prefix::Spell{list} , detail::suffix::Summon{}
    {

    }
};

class SpellPeriodicLeech : public detail::prefix::SpellPeriodic, public detail::suffix::Leech
{
public:
    SpellPeriodicLeech(QStringList list) : detail::prefix::SpellPeriodic{list.mid(0,3)} , detail::suffix::Leech{list.mid(3)}
    {

    }
};


using variant_t = std::variant<std::monostate,SpellCastSucces,SpellDamage,SpellPeriodicDamage,
                               SpellAuraApplied,SpellAuraRemoved, SpellAuraRefresh,SpellEnergize,
                               SwingDamage,SpellPeriodicHeal,SpellAuraAppliedDose,SpellCastStart,
                               SpellPeriodicEnergize, SpellHeal, SwingMissed, PartyKill,SpellCastFailed,
                               EnchantApplied,SpellMissed, SpellInterrupt,SpellPeriodicMissed,
                               SpellDispel,SpellSummon,SpellPeriodicLeech>;

using DamageShield = SpellDamage;
using UnitDied = PartyKill;
using DamageShieldMissed = SpellMissed;

#endif // SUBEVENTS_H

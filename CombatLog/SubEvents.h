#ifndef SUBEVENTS_H
#define SUBEVENTS_H

#include "defs.h"
#include <QString>
#include <cstdint>
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

    [[nodiscard]] bool operator==(UnitFlags o) const;

    uint32_t value{0};
};

struct Object
{
    [[nodiscard]] static Object fromString(const QStringList& list);

    [[nodiscard]] bool operator==(const Object &o) const;

    uint64_t guid;
    QString name;
    UnitFlags flags;
    // RaidFlags raidFlags;
};

struct SpellInfo
{
    SpellInfo(const QStringList& list);
    SpellInfo(QString name, uint32_t id, uint8_t school);

    [[nodiscard]] bool operator==(const SpellInfo &o) const;

    QString name;
    uint32_t id;
    uint8_t school; // same as SpellSchool?
};

struct Item
{
    Item(const QStringList& list);
    Item(uint32_t id, QString name);

    [[nodiscard]] bool operator==(const Item &o) const;

    uint32_t id;
    QString name;
};

namespace detail
{

namespace prefix
{
struct Swing
{
};

struct Range
{
    Range(const QStringList& list) : spell {list} {}
    Range(SpellInfo spell);
    SpellInfo spell;

    [[nodiscard]] bool operator==(const Range &o) const;
};

using Spell         = Range;
using SpellPeriodic = Range;
using SpellBuilding = Range;

struct Environmental
{
    Environmental(const QStringList& list);
    EnvironmentalType type;
};

} // namespace prefix

namespace suffix
{

struct Damage
{
    Damage(const QStringList& list);

    [[nodiscard]] bool operator==(const Damage &o) const;

    uint32_t amount;
    uint32_t overkill;
    SpellSchool spellschool; // not initialized
    uint32_t resisted;
    uint32_t blocked;
    uint32_t absorbed;
    bool critical;
    bool glancing;
    bool crushing;
    // bool isOffHand;
};

struct Missed
{
    Missed(const QStringList& list);

    [[nodiscard]] bool operator==(Missed o) const;

    MissType type;
    // bool isOffHand;
    uint32_t amountMissed{0};
    // bool critical;
};

struct Heal
{
    Heal(const QStringList& list);
    uint32_t amount;
    uint32_t overhealing;
    uint32_t absorbed;
    bool critical;
};

struct HealAbsorbed
{
    Object extra;
    SpellInfo extraSpell;
    uint32_t absorbedAmount {0};
    uint32_t totalAmount {0};
};

struct Absorbed
{
};

struct Energize
{
    Energize(const QStringList& list);
    uint32_t amount;
    // uint32_t overEnergize;
    PowerType type;
    // uint32_t maxPower;
};

struct Drain
{
    Drain(const QStringList& list);
    uint32_t amount;
    PowerType type;
    uint32_t extraAmount;
    // uint32_t maxPower;
};

using Leech = Drain;

struct Interrupt
{
    Interrupt(const QStringList& list);
    SpellInfo extraSpell;
};

struct Dispell
{
    Dispell(const QStringList& list);
    SpellInfo extraSpell;
    AuraType aura;
};

using DispellFailed = Interrupt;
using Stolen        = Dispell;

struct ExtraAttacks
{
    ExtraAttacks(const QStringList& list);
    uint32_t amount;
};

struct AuraApplied
{
    AuraApplied(const QStringList& list);
    AuraType type;
    // int amount;
};

using AuraRemoved = AuraApplied;
struct AuraAppliedDose : public AuraApplied
{
    AuraAppliedDose(const QStringList& list);
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
    SpellInfo extra;
    AuraType type;
};

using CastStart  = Absorbed;
using CastSucces = Absorbed;
struct CastFailed
{
    CastFailed(const QStringList& list);
    QString type;
};

using Instakill           = Absorbed;
using DurabilityDamage    = Absorbed;
using DurabilityDamageAll = Absorbed;
using Create              = Absorbed;
using Summon              = Absorbed;
using Resurrect           = Absorbed;
using Instakill           = Absorbed;

} // namespace suffix

} // namespace detail

struct SpellCastSucces : public detail::prefix::Spell,
                         public detail::suffix::CastSucces
{
    SpellCastSucces(const QStringList& list) : detail::prefix::Spell {list} {}
    SpellCastSucces(const detail::prefix::Spell& prefix,
                    detail::suffix::CastSucces suffix);

    [[nodiscard]] bool operator==(const SpellCastSucces &o) const;
};

struct SpellDamage : public detail::prefix::Spell, public detail::suffix::Damage
{
    SpellDamage(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::Damage {list.mid(3)}
    {
    }

    [[nodiscard]] bool operator==(const SpellDamage &o) const;
};

struct SpellPeriodicDamage : public detail::prefix::SpellPeriodic,
                             public detail::suffix::Damage
{
    SpellPeriodicDamage(const QStringList& list)
        : detail::prefix::SpellPeriodic {list.mid(0, 3)},
          detail::suffix::Damage {list.mid(3)}
    {
    }

    [[nodiscard]] bool operator==(const SpellPeriodicDamage &o) const;
};

struct SpellAuraApplied : public detail::prefix::Spell,
                          public detail::suffix::AuraApplied
{
    SpellAuraApplied(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::AuraApplied {list.mid(3)}
    {
    }
};

struct SpellAuraRemoved : public detail::prefix::Spell,
                          public detail::suffix::AuraRemoved
{
    SpellAuraRemoved(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::AuraRemoved {list.mid(3)}
    {
    }
};

struct SpellAuraRefresh : public detail::prefix::Spell,
                          public detail::suffix::AuraRefresh
{
    SpellAuraRefresh(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::AuraRefresh {list.mid(3)}
    {
    }
};

struct SpellEnergize : public detail::prefix::Spell,
                       public detail::suffix::Energize
{
    SpellEnergize(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::Energize {list.mid(3)}
    {
    }
};

struct SwingDamage : public detail::prefix::Swing, public detail::suffix::Damage
{
    SwingDamage(const QStringList& list)
        : detail::prefix::Swing {}, detail::suffix::Damage {list}
    {
    }

    [[nodiscard]] bool operator==(const SwingDamage &o) const;
};

struct SpellPeriodicHeal : public detail::prefix::SpellPeriodic,
                           public detail::suffix::Heal
{
    SpellPeriodicHeal(const QStringList& list)
        : detail::prefix::SpellPeriodic {list.mid(0, 3)},
          detail::suffix::Heal {list.mid(3)}
    {
    }
};

struct SpellAuraAppliedDose : public detail::prefix::Spell,
                              public detail::suffix::AuraAppliedDose
{
    SpellAuraAppliedDose(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::AuraAppliedDose {list.mid(3)}
    {
    }
};

struct SpellCastStart : public detail::prefix::Spell,
                        public detail::suffix::CastStart
{
    SpellCastStart(const QStringList& list) : detail::prefix::Spell {list} {}
};

struct SpellPeriodicEnergize : public detail::prefix::SpellPeriodic,
                               public detail::suffix::Energize
{
    SpellPeriodicEnergize(const QStringList& list)
        : detail::prefix::SpellPeriodic {list.mid(0, 3)},
          detail::suffix::Energize {list.mid(3)}
    {
    }
};

struct SpellHeal : public detail::prefix::Spell, public detail::suffix::Heal
{
    SpellHeal(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::Heal {list.mid(3)}
    {
    }
};

struct SwingMissed : public detail::prefix::Swing, public detail::suffix::Missed
{
    SwingMissed(const QStringList& list)
        : detail::prefix::Swing {}, detail::suffix::Missed {list}
    {
    }

    [[nodiscard]] bool operator==(SwingMissed o) const;
};

struct PartyKill
{
    PartyKill(const QStringList& list);

    [[nodiscard]] bool operator==([[maybe_unused]] PartyKill o) const
    {
        return true;
    }
};

struct SpellCastFailed : public detail::prefix::Spell,
                         public detail::suffix::CastFailed
{
    SpellCastFailed(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::CastFailed {list.mid(3)}
    {
    }
};

struct EnchantApplied
{
    EnchantApplied(const QStringList& list);
    EnchantApplied(QString name, Item item);

    [[nodiscard]] bool operator==(const EnchantApplied& o) const;

    QString name;
    Item item;
};

struct SpellMissed : public detail::prefix::Spell, public detail::suffix::Missed
{
    SpellMissed(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::Missed {list.mid(3)}
    {
    }

    [[nodiscard]] bool operator==(const SpellMissed &o) const;
};

struct SpellInterrupt : public detail::prefix::Spell,
                        public detail::suffix::Interrupt
{
    SpellInterrupt(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::Interrupt {list.mid(3)}
    {
    }
};

struct SpellPeriodicMissed : public detail::prefix::SpellPeriodic,
                             public detail::suffix::Missed
{
    SpellPeriodicMissed(const QStringList& list)
        : detail::prefix::SpellPeriodic {list.mid(0, 3)},
          detail::suffix::Missed {list.mid(3)}
    {
    }

    [[nodiscard]] bool operator==(const SpellPeriodicMissed &o) const;
};

struct SpellDispel : public detail::prefix::Spell,
                     public detail::suffix::Dispell
{
    SpellDispel(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::Dispell {list.mid(3)}
    {
    }
};

struct SpellSummon : public detail::prefix::Spell, public detail::suffix::Summon
{
    SpellSummon(const QStringList& list)
        : detail::prefix::Spell {list}, detail::suffix::Summon {}
    {
    }
};

struct SpellPeriodicLeech : public detail::prefix::SpellPeriodic,
                            public detail::suffix::Leech
{
    SpellPeriodicLeech(const QStringList& list)
        : detail::prefix::SpellPeriodic {list.mid(0, 3)},
          detail::suffix::Leech {list.mid(3)}
    {
    }
};

struct SpellResurrect : public detail::prefix::Spell,
                        public detail::suffix::Resurrect
{
    SpellResurrect(const QStringList& list)
        : detail::prefix::Spell {list}, detail::suffix::Resurrect {}
    {
    }
};

struct SpellExtraAttacks : public detail::prefix::Spell,
                           public detail::suffix::ExtraAttacks
{
    SpellExtraAttacks(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::ExtraAttacks {list.mid(3)}
    {
    }
};

struct EnvironmentalDamage : public detail::prefix::Environmental,
                             public detail::suffix::Damage
{
    EnvironmentalDamage(const QStringList& list)
        : detail::prefix::Environmental {list.mid(0, 1)},
          detail::suffix::Damage {list.mid(1)}
    {
    }
};

struct SpellAuraRemovedDose : public detail::prefix::Spell,
                              public detail::suffix::AuraAppliedDose
{
    SpellAuraRemovedDose(const QStringList& list)
        : detail::prefix::Spell {list.mid(0, 3)},
          detail::suffix::AuraRemovedDose {list.mid(3)}
    {
    }
};

using variant_t = std::variant<
    std::monostate, SpellCastSucces, SpellDamage, SpellPeriodicDamage,
    SpellAuraApplied, SpellAuraRemoved, SpellAuraRefresh, SpellEnergize,
    SwingDamage, SpellPeriodicHeal, SpellAuraAppliedDose, SpellCastStart,
    SpellPeriodicEnergize, SpellHeal, SwingMissed, PartyKill, SpellCastFailed,
    EnchantApplied, SpellMissed, SpellInterrupt, SpellPeriodicMissed,
    SpellDispel, SpellSummon, SpellPeriodicLeech, SpellResurrect,
    SpellExtraAttacks, EnvironmentalDamage, SpellAuraRemovedDose>;

using DamageShield       = SpellDamage;
using UnitDied           = PartyKill;
using DamageShieldMissed = SpellMissed;
using SpellCreate        = SpellSummon;
using RangeDamage        = SpellDamage;
using SpellInstakill     = SpellSummon;
using EnchantRemoved     = EnchantApplied;
using DamageSplit        = SpellDamage;
using SpellStolen        = SpellDispel;
using RangeMissed        = SpellMissed;

#endif // SUBEVENTS_H

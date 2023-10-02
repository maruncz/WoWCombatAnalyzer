#include "SubEvents.h"
#include "LineParser.h"
#include "exceptions.h"

UnitFlags UnitFlags::fromNum(uint32_t n)
{
    static constexpr uint32_t unusedBits{Reaction::unused | Type::unused};
    if (n & unusedBits)
    {
        throw CombatLogParserException(
            QStringLiteral("undefined bits set: 0x%1")
                .arg(n & unusedBits, 8, 16)
                .toStdString());
    }

    UnitFlags ret;
    ret.value = n;
    return ret;
}

bool UnitFlags::operator==(UnitFlags o) const
{
    return value == o.value;
}

Object Object::fromString(const QStringList& list)
{
    if (list.size() != 3)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 3, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }
    Object ret;
    ret.guid  = LineParser::parseGUID(list.at(0));
    ret.flags = LineParser::parseUnitFlags(list.at(2));
    if (ret.flags.value == UnitFlags::Special::none)
    {
        ret.name.clear();
    }
    else
    {
        ret.name = LineParser::removequotes(list.at(1));
    }
    return ret;
}

bool Object::operator==(const Object &o) const
{
    if (guid != o.guid)
    {
        return false;
    }

    if (name != o.name)
    {
        return false;
    }

    if (flags != o.flags)
    {
        return false;
    }

    return true;
}

SpellInfo::SpellInfo(const QStringList& list)
{
    if (list.size() != 3)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 3, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }
    id     = LineParser::parseSpellId(list.at(0));
    name   = LineParser::removequotes(list.at(1));
    school = LineParser::parseSpellSchool(list.at(2));
}

SpellInfo::SpellInfo(const QString &name, uint32_t id, uint8_t school)
    : name{name}, id{id}, school{school}
{
}

bool SpellInfo::operator==(const SpellInfo &o) const
{
    if (name != o.name)
    {
        return false;
    }

    if (id != o.id)
    {
        return false;
    }

    if (school != o.school)
    {
        return false;
    }

    return true;
}

Item::Item(const QStringList& list)
{
    if (list.size() != 2)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 2, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }
    id   = LineParser::parseSpellId(list.at(0));
    name = LineParser::removequotes(list.at(1));
}

Item::Item(uint32_t id, const QString &name) : id{id}, name{name} {}

bool Item::operator==(const Item &o) const
{
    if (id != o.id)
    {
        return false;
    }

    if (name != o.name)
    {
        return false;
    }

    return true;
}

detail::suffix::Damage::Damage(const QStringList& list)
{
    if (list.size() != 9)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 9, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    amount   = LineParser::parseDamageAmount(list.at(0));
    overkill = LineParser::parseDamageAmount(list.at(1));

    resisted = LineParser::parseDamageAmount(list.at(3));
    blocked  = LineParser::parseDamageAmount(list.at(4));
    absorbed = LineParser::parseDamageAmount(list.at(5));
    critical = LineParser::parseBool(list.at(6));
    glancing = LineParser::parseBool(list.at(7));
    crushing = LineParser::parseBool(list.at(8));
    // isOffHand = LineParser::parseBool(list.at(9));
}

bool detail::suffix::Damage::operator==(const Damage &o) const
{
    if (amount != o.amount)
    {
        return false;
    }
    if (overkill != o.overkill)
    {
        return false;
    }
    if (spellschool != o.spellschool)
    {
        return false;
    }
    if (resisted != o.resisted)
    {
        return false;
    }
    if (absorbed != o.absorbed)
    {
        return false;
    }
    if (critical != o.critical)
    {
        return false;
    }
    if (glancing != o.glancing)
    {
        return false;
    }
    if (crushing != o.crushing)
    {
        return false;
    }
    return true;
}

detail::suffix::AuraApplied::AuraApplied(const QStringList& list)
{
    if (list.size() != 1)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 1, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    type = LineParser::parseAuraType(list.at(0));
}

detail::suffix::Energize::Energize(const QStringList& list)
{
    if (list.size() != 2)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 2, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(0));
    type   = LineParser::parsePowerType(list.at(1));
}

detail::suffix::Heal::Heal(const QStringList& list)
{
    if (list.size() != 4)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 4, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    amount      = LineParser::parseDamageAmount(list.at(0));
    overhealing = LineParser::parseDamageAmount(list.at(1));
    absorbed    = LineParser::parseDamageAmount(list.at(2));
    critical    = LineParser::parseBool(list.at(3));
}

detail::suffix::AuraAppliedDose::AuraAppliedDose(const QStringList& list)
    : AuraApplied(list.mid(0, 1))
{
    if (list.size() != 2)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 2, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(1));
}

detail::suffix::Missed::Missed(const QStringList& list)
{
    if (list.size() < 1)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 1, got %1")
                .arg(list.size())
                .toStdString());
    }

    type = LineParser::parseMissType(list.at(0));

    if (type == MissType::DODGE)
    {
        amountMissed = 0;
        if (list.size() != 1)
        {
            throw CombatLogParserException(
                QStringLiteral("wrong list size, expected 1, got %1 : %2")
                    .arg(list.size())
                    .arg(list.join(','))
                    .toStdString());
        }
    }
    else if (type == MissType::ABSORB)
    {
        amountMissed = LineParser::parseDamageAmount(list.at(1));
        if (list.size() != 2)
        {
            throw CombatLogParserException(
                QStringLiteral("wrong list size, expected 2, got %1 : %2")
                    .arg(list.size())
                    .arg(list.join(','))
                    .toStdString());
        }
    }
}

bool detail::suffix::Missed::operator==(Missed o) const
{
    if (type != o.type)
    {
        return false;
    }
    if (amountMissed != o.amountMissed)
    {
        return false;
    }
    return true;
}

PartyKill::PartyKill(const QStringList& list)
{
    if (list.size() != 0)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 0, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }
}

detail::suffix::CastFailed::CastFailed(const QStringList& list)
{
    if (list.size() != 1)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 1, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    type = LineParser::removequotes(list.at(0));
}

EnchantApplied::EnchantApplied(const QStringList& list) : item {list.mid(1)}
{
    if (list.size() != 3)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 3, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    name = LineParser::removequotes(list.at(0));
}

EnchantApplied::EnchantApplied(const QString &name, const Item &item)
    : name{name}, item{item}
{
}

bool EnchantApplied::operator==(const EnchantApplied &o) const
{
    if (name != o.name)
    {
        return false;
    }

    if (item != o.item)
    {
        return false;
    }

    return true;
}

detail::suffix::Interrupt::Interrupt(const QStringList& list)
    : extraSpell {list}
{
}

detail::suffix::Dispell::Dispell(const QStringList& list)
    : extraSpell {list.mid(0, 3)}
{
    if (list.size() != 4)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 4, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }
    aura = LineParser::parseAuraType(list.at(3));
}

detail::suffix::Drain::Drain(const QStringList& list)
{
    if (list.size() != 3)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 3, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    amount      = LineParser::parseDamageAmount(list.at(0));
    type        = LineParser::parsePowerType(list.at(1));
    extraAmount = LineParser::parseDamageAmount(list.at(2));
}

detail::prefix::Range::Range(const SpellInfo &spell) : spell{spell} {}

bool detail::prefix::Range::operator==(const Range &o) const
{
    return spell == o.spell;
}

SpellCastSucces::SpellCastSucces(const detail::prefix::Spell& prefix,
                                 detail::suffix::CastSucces suffix)
    : detail::prefix::Spell {prefix}, detail::suffix::CastSucces {suffix}
{
}

bool SpellCastSucces::operator==(const SpellCastSucces &o) const
{
    return detail::prefix::Spell::operator==(o);
}

bool SwingDamage::operator==(const SwingDamage &o) const
{
    return detail::suffix::Damage::operator==(o);
}

bool SpellDamage::operator==(const SpellDamage &o) const
{
    return detail::prefix::Spell::operator==(o) &&
           detail::suffix::Damage::operator==(o);
}

bool SpellPeriodicDamage::operator==(const SpellPeriodicDamage &o) const
{
    return detail::prefix::SpellPeriodic::operator==(o) &&
           detail::suffix::Damage::operator==(o);
}

bool SwingMissed::operator==(SwingMissed o) const
{
    return detail::suffix::Missed::operator==(o);
}

bool SpellMissed::operator==(const SpellMissed &o) const
{
    return detail::prefix::Spell::operator==(o) &&
           detail::suffix::Missed::operator==(o);
}

bool SpellPeriodicMissed::operator==(const SpellPeriodicMissed &o) const
{
    return detail::prefix::SpellPeriodic::operator==(o) &&
           detail::suffix::Missed::operator==(o);
}

detail::suffix::ExtraAttacks::ExtraAttacks(const QStringList& list)
{
    if (list.size() != 1)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 3, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(0));
}

detail::prefix::Environmental::Environmental(const QStringList& list)
{
    if (list.size() != 1)
    {
        throw CombatLogParserException(
            QStringLiteral("wrong list size, expected 3, got %1 : %2")
                .arg(list.size())
                .arg(list.join(','))
                .toStdString());
    }

    type = [](const QString& s) -> EnvironmentalType
    {
        if (s == QLatin1String("DROWNING"))
        {
            return EnvironmentalType::DROWNING;
        }
        else if (s == QLatin1String("FALLING"))
        {
            return EnvironmentalType::FALLING;
        }
        else if (s == QLatin1String("FATIGUE"))
        {
            return EnvironmentalType::FATIGUE;
        }
        else if (s == QLatin1String("FIRE"))
        {
            return EnvironmentalType::FIRE;
        }
        else if (s == QLatin1String("LAVA"))
        {
            return EnvironmentalType::LAVA;
        }
        else if (s == QLatin1String("SLIME"))
        {
            return EnvironmentalType::SLIME;
        }

        throw CombatLogParserException(
            QStringLiteral("unexpected EnvironmentalType: %1")
                .arg(s)
                .toStdString());
    }(list.at(0));
}

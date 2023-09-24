#include "SubEvents.h"
#include "LineParser.h"
#include "exceptions.h"

UnitFlags UnitFlags::fromNum(uint32_t n)
{
    static constexpr uint32_t unusedBits{Reaction::unused | Type::unused | Special::unused};
    if(n & unusedBits)
    {
        throw CombatLogParserException(QString("undefined bits set: %1").arg(n & unusedBits, 8,16).toStdString());
    }

    UnitFlags ret;
    ret.value = n;
    return ret;
}

Object Object::fromString(QStringList list)
{
    if(list.size() != 3)
    {
        throw CombatLogParserException(QString("wrong list size, expected 3, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }
    Object ret;
    ret.guid = LineParser::parseGUID(list.at(0));
    ret.flags = LineParser::parseUnitFlags(list.at(2));
    if(ret.flags.value == UnitFlags::Special::none)
    {
        ret.name.clear();
    }
    else
    {
        ret.name = LineParser::removequotes(list.at(1));
    }
    return ret;
}

Spell::Spell(QStringList list)
{
    if(list.size() != 3)
    {
        throw CombatLogParserException(QString("wrong list size, expected 3, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }
    id = LineParser::parseSpellId(list.at(0));
    name = LineParser::removequotes(list.at(1));
    school = LineParser::parseSpellSchool(list.at(2));
}

Item::Item(QStringList list)
{
    if(list.size() != 2)
    {
        throw CombatLogParserException(QString("wrong list size, expected 2, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }
    id = LineParser::parseSpellId(list.at(0));
    name = LineParser::removequotes(list.at(1));
}

detail::suffix::Damage::Damage(QStringList list)
{
    if(list.size() != 9)
    {
        throw CombatLogParserException(QString("wrong list size, expected 9, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(0));
    overkill = LineParser::parseDamageAmount(list.at(1));

    resisted  = LineParser::parseDamageAmount(list.at(3));
    blocked = LineParser::parseDamageAmount(list.at(4));
    absorbed = LineParser::parseDamageAmount(list.at(5));
    critical = LineParser::parseBool(list.at(6));
    glancing = LineParser::parseBool(list.at(7));
    crushing = LineParser::parseBool(list.at(8));
    //isOffHand = LineParser::parseBool(list.at(9));
}

detail::suffix::AuraApplied::AuraApplied(QStringList list)
{
    if(list.size() != 1)
    {
        throw CombatLogParserException(QString("wrong list size, expected 1, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    type = LineParser::parseAuraType(list.at(0));
}

detail::suffix::Energize::Energize(QStringList list)
{
    if(list.size() != 2)
    {
        throw CombatLogParserException(QString("wrong list size, expected 2, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(0));
    type = LineParser::parsePowerType(list.at(1));
}

detail::suffix::Heal::Heal(QStringList list)
{
    if(list.size() != 4)
    {
        throw CombatLogParserException(QString("wrong list size, expected 4, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(0));
    overhealing = LineParser::parseDamageAmount(list.at(1));
    absorbed = LineParser::parseDamageAmount(list.at(2));
    critical = LineParser::parseBool(list.at(3));
}

detail::suffix::AuraAppliedDose::AuraAppliedDose(QStringList list) : AuraApplied(list.mid(0,1))
{
    if(list.size() != 2)
    {
        throw CombatLogParserException(QString("wrong list size, expected 2, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(1));
}

detail::suffix::Missed::Missed(QStringList list)
{
    if(list.size() < 1)
    {
        throw CombatLogParserException(QString("wrong list size, expected 1, got %1").arg(list.size()).toStdString());
    }


    type = LineParser::parseMissType(list.at(0));

    if(type == MissType::DODGE)
    {
        amountMissed = 0;
        if(list.size() != 1)
        {
            throw CombatLogParserException(QString("wrong list size, expected 1, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
        }
    }
    else if(type == MissType::ABSORB)
    {
        amountMissed = LineParser::parseDamageAmount(list.at(1));
        if(list.size() != 2)
        {
            throw CombatLogParserException(QString("wrong list size, expected 2, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
        }
    }

}

PartyKill::PartyKill(QStringList list)
{
    if(list.size() != 0)
    {
        throw CombatLogParserException(QString("wrong list size, expected 0, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }
}


detail::suffix::CastFailed::CastFailed(QStringList list)
{
    if(list.size() != 1)
    {
        throw CombatLogParserException(QString("wrong list size, expected 1, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    type = LineParser::removequotes(list.at(0));
}



EnchantApplied::EnchantApplied(QStringList list) : item{list.mid(1)}
{
    if(list.size() != 3)
    {
        throw CombatLogParserException(QString("wrong list size, expected 3, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    name = LineParser::removequotes(list.at(0));
}

detail::suffix::Interrupt::Interrupt(QStringList list) : extraSpell{list}
{

}

detail::suffix::Dispell::Dispell(QStringList list) : extraSpell{list.mid(0,3)}
{
    if(list.size() != 4)
    {
        throw CombatLogParserException(QString("wrong list size, expected 4, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }
    aura = LineParser::parseAuraType(list.at(3));
}

detail::suffix::Drain::Drain(QStringList list)
{
    if(list.size() != 3)
    {
        throw CombatLogParserException(QString("wrong list size, expected 3, got %1 : %2").arg(list.size()).arg(list.join(',')).toStdString());
    }

    amount = LineParser::parseDamageAmount(list.at(0));
    type = LineParser::parsePowerType(list.at(1));
    extraAmount = LineParser::parseDamageAmount(list.at(2));
}

#include "LogLine.h"
#include "defs.h"
#include "exceptions.h"
#include <QDebug>

LogLine::LogLine(const QDateTime &timestamp, const Object &sourceObject,
                 const Object &destObject, SubEvent subeventType,
                 const variant_t &subEventValue)
    : timestamp{timestamp}, sourceObject{sourceObject}, destObject{destObject},
      subeventType{subeventType}, subEventValue{subEventValue}
{
}

LogLine LogLine::fromRawData(QString s)
{
    auto line = parseTokens(s);
    LogLine ret;

    ret.timestamp     = parseTimestamp(line.at(0));
    ret.subeventType  = subeventTypeFromString(line.at(1));
    ret.sourceObject  = Object::fromString(line.mid(2, 3));
    ret.destObject    = Object::fromString(line.mid(5, 3));
    ret.subEventValue = subeventValueFromString(ret.subeventType, line.mid(8));

    return ret;
}

QStringList LogLine::parseTokens(QString s)
{
    QStringList ret;

    auto time_sep = s.indexOf("  ");
    if (time_sep == -1)
    {
        throw CombatLogParserException("cannot find timestamp");
    }

    ret.append(s.mid(0, time_sep));
    s.remove(0, time_sep + 2);

    QStringList list;
    {
        bool inQuotes{false};
        QString::size_type lastComma{0};
        for (QString::size_type i = 0; i < s.size(); ++i)
        {
            if ((s.at(i) == ',') && (!inQuotes))
            {
                list.append(s.mid(lastComma, i - lastComma));
                lastComma = i + 1;
            }

            if (s.at(i) == '"')
            {
                inQuotes = !inQuotes;
            }

            if (s.at(i) == '\n')
            {
                list.append(s.mid(lastComma, i - lastComma));
                break;
            }
        }
    }

    for (auto &&e : list)
    {
        ret.append(e.trimmed());
    }

    return ret;
}

QDateTime LogLine::parseTimestamp(QString s)
{
    auto ret = QDateTime::fromString(s, "M/dd hh:mm:ss.zzz");
    ret      = ret.addYears(2023 - 1900);
    return ret;
}

SubEvent LogLine::subeventTypeFromString(QString s)
{
    if (s == "SPELL_CAST_SUCCESS")
    {
        return SubEvent::SPELL_CAST_SUCCESS;
    }
    else if (s == "SPELL_AURA_APPLIED")
    {
        return SubEvent::SPELL_AURA_APPLIED;
    }
    else if (s == "SPELL_AURA_APPLIED_DOSE")
    {
        return SubEvent::SPELL_AURA_APPLIED_DOSE;
    }
    else if (s == "SPELL_AURA_REMOVED")
    {
        return SubEvent::SPELL_AURA_REMOVED;
    }
    else if (s == "SPELL_AURA_REFRESH")
    {
        return SubEvent::SPELL_AURA_REFRESH;
    }
    else if (s == "SPELL_DAMAGE")
    {
        return SubEvent::SPELL_DAMAGE;
    }
    else if (s == "SPELL_ENERGIZE")
    {
        return SubEvent::SPELL_ENERGIZE;
    }
    else if (s == "SPELL_PERIODIC_DAMAGE")
    {
        return SubEvent::SPELL_PERIODIC_DAMAGE;
    }
    else if (s == "SWING_DAMAGE")
    {
        return SubEvent::SWING_DAMAGE;
    }
    else if (s == "DAMAGE_SHIELD")
    {
        return SubEvent::DAMAGE_SHIELD;
    }
    else if (s == "SPELL_PERIODIC_HEAL")
    {
        return SubEvent::SPELL_PERIODIC_HEAL;
    }
    else if (s == "SPELL_CAST_START")
    {
        return SubEvent::SPELL_CAST_START;
    }
    else if (s == "SPELL_PERIODIC_ENERGIZE")
    {
        return SubEvent::SPELL_PERIODIC_ENERGIZE;
    }
    else if (s == "SPELL_HEAL")
    {
        return SubEvent::SPELL_HEAL;
    }
    else if (s == "SWING_MISSED")
    {
        return SubEvent::SWING_MISSED;
    }
    else if (s == "PARTY_KILL")
    {
        return SubEvent::PARTY_KILL;
    }
    else if (s == "UNIT_DIED")
    {
        return SubEvent::UNIT_DIED;
    }
    else if (s == "SPELL_CAST_FAILED")
    {
        return SubEvent::SPELL_CAST_FAILED;
    }
    else if (s == "ENCHANT_APPLIED")
    {
        return SubEvent::ENCHANT_APPLIED;
    }
    else if (s == "SPELL_MISSED")
    {
        return SubEvent::SPELL_MISSED;
    }
    else if (s == "SPELL_INTERRUPT")
    {
        return SubEvent::SPELL_INTERRUPT;
    }
    else if (s == "SPELL_PERIODIC_MISSED")
    {
        return SubEvent::SPELL_PERIODIC_MISSED;
    }
    else if (s == "SPELL_DISPEL")
    {
        return SubEvent::SPELL_DISPEL;
    }
    else if (s == "SPELL_SUMMON")
    {
        return SubEvent::SPELL_SUMMON;
    }
    else if (s == "DAMAGE_SHIELD_MISSED")
    {
        return SubEvent::DAMAGE_SHIELD_MISSED;
    }
    else if (s == "SPELL_PERIODIC_LEECH")
    {
        return SubEvent::SPELL_PERIODIC_LEECH;
    }
    else if (s == "SPELL_CREATE")
    {
        return SubEvent::SPELL_CREATE;
    }
    else if (s == "RANGE_DAMAGE")
    {
        return SubEvent::RANGE_DAMAGE;
    }
    else if (s == "SPELL_INSTAKILL")
    {
        return SubEvent::SPELL_INSTAKILL;
    }
    else if (s == "SPELL_RESURRECT")
    {
        return SubEvent::SPELL_RESURRECT;
    }
    else if (s == "ENCHANT_REMOVED")
    {
        return SubEvent::ENCHANT_REMOVED;
    }

    throw CombatLogParserException(
        QString("unknown subEvent: %1").arg(s).toStdString());
}

variant_t LogLine::subeventValueFromString(SubEvent type, QStringList list)
{
    switch (type)
    {
        case SubEvent::SPELL_CAST_SUCCESS: return {SpellCastSucces{list}};
        case SubEvent::SPELL_AURA_REMOVED: return {SpellAuraRemoved{list}};
        case SubEvent::SPELL_AURA_REFRESH: return {SpellAuraRefresh{list}};
        case SubEvent::SPELL_AURA_APPLIED: return {SpellAuraApplied{list}};
        case SubEvent::SPELL_AURA_APPLIED_DOSE:
            return {SpellAuraAppliedDose{list}};
        case SubEvent::SPELL_DAMAGE: return {SpellDamage{list}};
        case SubEvent::SPELL_ENERGIZE: return {SpellEnergize{list}};
        case SubEvent::SPELL_PERIODIC_DAMAGE:
            return {SpellPeriodicDamage{list}};
        case SubEvent::SWING_DAMAGE: return {SwingDamage{list}};
        case SubEvent::DAMAGE_SHIELD: return {DamageShield{list}};
        case SubEvent::SPELL_PERIODIC_HEAL: return {SpellPeriodicHeal{list}};
        case SubEvent::SPELL_CAST_START: return {SpellCastStart{list}};
        case SubEvent::SPELL_PERIODIC_ENERGIZE:
            return {SpellPeriodicEnergize{list}};
        case SubEvent::SPELL_HEAL: return {SpellHeal{list}};
        case SubEvent::SWING_MISSED: return {SwingMissed{list}};
        case SubEvent::PARTY_KILL: return {PartyKill{list}};
        case SubEvent::UNIT_DIED: return {UnitDied{list}};
        case SubEvent::SPELL_CAST_FAILED: return {SpellCastFailed{list}};
        case SubEvent::ENCHANT_APPLIED: return {EnchantApplied{list}};
        case SubEvent::SPELL_MISSED: return {SpellMissed{list}};
        case SubEvent::SPELL_INTERRUPT: return {SpellInterrupt{list}};
        case SubEvent::SPELL_PERIODIC_MISSED:
            return {SpellPeriodicMissed{list}};
        case SubEvent::SPELL_DISPEL: return {SpellDispel{list}};
        case SubEvent::SPELL_SUMMON: return {SpellSummon{list}};
        case SubEvent::DAMAGE_SHIELD_MISSED: return {DamageShieldMissed{list}};
        case SubEvent::SPELL_PERIODIC_LEECH: return {SpellPeriodicLeech{list}};
        case SubEvent::SPELL_CREATE: return {SpellCreate{list}};
        case SubEvent::RANGE_DAMAGE: return {RangeDamage{list}};
        case SubEvent::SPELL_INSTAKILL: return {SpellInstakill{list}};
        case SubEvent::SPELL_RESURRECT: return {SpellResurrect{list}};
        case SubEvent::ENCHANT_REMOVED: return {EnchantRemoved{list}};
    }
    throw CombatLogParserException(
        QString("unhandled subevent: %1").arg(list.at(0)).toStdString());
}

variant_t LogLine::getSubEventValue() const
{
    return subEventValue;
}

bool LogLine::operator==(const LogLine &o) const
{
    if (timestamp != o.timestamp)
    {
        return false;
    }

    if (sourceObject != o.sourceObject)
    {
        return false;
    }

    if (destObject != o.destObject)
    {
        return false;
    }

    if (subeventType != o.subeventType)
    {
        return false;
    }

    if (subEventValue != subEventValue)
    {
        return false;
    }

    return true;
}

SubEvent LogLine::getSubeventType() const
{
    return subeventType;
}

Object LogLine::getDestObject() const
{
    return destObject;
}

Object LogLine::getSourceObject() const
{
    return sourceObject;
}

QDateTime LogLine::getTimestamp() const
{
    return timestamp;
}

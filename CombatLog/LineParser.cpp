#include "LineParser.h"
#include "exceptions.h"

QStringList LineParser::splitLine(QString s)
{
    QStringList ret;
    auto time_sep = s.indexOf("  ");
    if (time_sep == -1)
    {
        throw CombatLogParserException("cannot find timestamp");
    }

    ret.append(s.mid(0, time_sep));

    s.remove(0, time_sep);
    auto list = s.split(',');

    for (auto &&e : list)
    {
        ret.append(e);
    }
    return ret;
}

uint64_t LineParser::parseGUID(QString s)
{
    return parseNumber<uint64_t>(s, 16);
}

QString LineParser::removequotes(QString s)
{
    if ((!s.endsWith('"')) || (!s.startsWith('"')))
    {
        throw CombatLogParserException(
            QString("bad string format: %1").arg(s).toStdString());
    }
    s.remove('"');
    return s;
}

UnitFlags LineParser::parseUnitFlags(QString s)
{
    UnitFlags ret;
    ret = UnitFlags::fromNum(parseNumber<uint32_t>(s, 16));
    return ret;
}

uint32_t LineParser::parseSpellId(QString s)
{
    return parseNumber<uint32_t>(s, 10);
}

uint8_t LineParser::parseSpellSchool(QString s)
{
    return parseNumber<uint8_t>(s, 16);
}

uint32_t LineParser::parseDamageAmount(QString s)
{
    return parseNumber<uint32_t>(s, 10);
}

bool LineParser::parseBool(QString s)
{
    if (s == "1")
    {
        return true;
    }
    else if (s == "nil")
    {
        return false;
    }
    else
    {
        throw CombatLogParserException(
            QString("unexpected bool value: %1").arg(s).toStdString());
    }
}

AuraType LineParser::parseAuraType(QString s)
{
    if (s == "BUFF")
    {
        return AuraType::BUFF;
    }
    else if (s == "DEBUFF")
    {
        return AuraType::DEBUFF;
    }
    else
    {
        throw CombatLogParserException(
            QString("unexpected aura type value: %1").arg(s).toStdString());
    }
}

PowerType LineParser::parsePowerType(QString s)
{
    auto val = parseNumber<uint8_t>(s, 10);
    if (val >= 26)
    {
        throw CombatLogParserException(
            QString("unexpected power type: %1").arg(val).toStdString());
    }

    return static_cast<PowerType>(val);
}

MissType LineParser::parseMissType(QString s)
{
    if (s == "ABSORB")
    {
        return MissType::ABSORB;
    }
    else if (s == "BLOCK")
    {
        return MissType::BLOCK;
    }
    else if (s == "DEFLECT")
    {
        return MissType::DEFLECT;
    }
    else if (s == "DODGE")
    {
        return MissType::DODGE;
    }
    else if (s == "EVADE")
    {
        return MissType::EVADE;
    }
    else if (s == "IMMUNE")
    {
        return MissType::IMMUNE;
    }
    else if (s == "MISS")
    {
        return MissType::MISS;
    }
    else if (s == "PARRY")
    {
        return MissType::PARRY;
    }
    else if (s == "REFLECT")
    {
        return MissType::REFLECT;
    }
    else if (s == "RESIST")
    {
        return MissType::RESIST;
    }
    else
    {
        throw CombatLogParserException(
            QString("unexpected miss type value: %1").arg(s).toStdString());
    }
}

template<typename T> T LineParser::parseNumber(QString s, uint8_t base)
{
    bool ok{false};
    T val{0};
    if constexpr (std::is_same_v<T, qulonglong>)
    {
        val = s.toULongLong(&ok, base);
    }
    else if constexpr (std::is_same_v<T, ulong>)
    {
        val = s.toULong(&ok, base);
    }
    else if constexpr (std::is_same_v<T, uint>)
    {
        val = s.toUInt(&ok, base);
    }
    else if constexpr (std::is_same_v<T, ushort>)
    {
        val = s.toUShort(&ok, base);
    }
    else if constexpr (std::is_same_v<T, uint8_t>)
    {
        auto tmp = s.toUShort(&ok, base);
        if (tmp > 255)
        {
            throw CombatLogParserException(
                QString("out of range: %1").arg(tmp).toStdString());
        }
        val = static_cast<T>(tmp);
    }
    else
    {
        throw CombatLogParserException("unimplemented parsing of number");
    }

    if (!ok)
    {
        throw CombatLogParserException(
            QString("error parsing number: %1, base: %2")
                .arg(s)
                .arg(base)
                .toStdString());
    }
    return val;
}

#include "LineParser.h"
#include "exceptions.h"

QStringList LineParser::splitLine(QString s)
{
    QStringList ret;
    auto time_sep = s.indexOf(QLatin1String("  "));
    if (time_sep == -1)
    {
        throw CombatLogParserException("cannot find timestamp");
    }

    ret.append(s.mid(0, time_sep));

    s.remove(0, time_sep);
    auto list = s.split(',');

    ret.reserve(ret.size() + list.size());

    for (auto &&e : list)
    {
        ret.append(e);
    }
    return ret;
}

uint64_t LineParser::parseGUID(const QString& s)
{
    return parseNumber<uint64_t>(s, 16);
}

QString LineParser::removequotes(QString s)
{
    if ((!s.endsWith('"')) || (!s.startsWith('"')))
    {
        throw CombatLogParserException(
            QStringLiteral("bad string format: %1").arg(s).toStdString());
    }
    s.remove('"');
    return s;
}

UnitFlags LineParser::parseUnitFlags(const QString& s)
{
    UnitFlags ret;
    ret = UnitFlags::fromNum(parseNumber<uint32_t>(s, 16));
    return ret;
}

uint32_t LineParser::parseSpellId(const QString& s)
{
    return parseNumber<uint32_t>(s, 10);
}

uint8_t LineParser::parseSpellSchool(const QString& s)
{
    return parseNumber<uint8_t>(s, 16);
}

uint32_t LineParser::parseDamageAmount(const QString& s)
{
    return parseNumber<uint32_t>(s, 10);
}

bool LineParser::parseBool(const QString& s)
{
    if (s == QLatin1String("1"))
    {
        return true;
    }
    else if (s == QLatin1String("nil"))
    {
        return false;
    }
    else
    {
        throw CombatLogParserException(
            QStringLiteral("unexpected bool value: %1").arg(s).toStdString());
    }
}

AuraType LineParser::parseAuraType(const QString& s)
{
    if (s == QLatin1String("BUFF"))
    {
        return AuraType::BUFF;
    }
    else if (s == QLatin1String("DEBUFF"))
    {
        return AuraType::DEBUFF;
    }
    else
    {
        throw CombatLogParserException(
            QStringLiteral("unexpected aura type value: %1")
                .arg(s)
                .toStdString());
    }
}

PowerType LineParser::parsePowerType(const QString& s)
{
    auto val = parseNumber<uint8_t>(s, 10);
    if (val >= 26)
    {
        throw CombatLogParserException(
            QStringLiteral("unexpected power type: %1").arg(val).toStdString());
    }

    return static_cast<PowerType>(val);
}

MissType LineParser::parseMissType(const QString& s)
{
    if (s == QLatin1String("ABSORB"))
    {
        return MissType::ABSORB;
    }
    else if (s == QLatin1String("BLOCK"))
    {
        return MissType::BLOCK;
    }
    else if (s == QLatin1String("DEFLECT"))
    {
        return MissType::DEFLECT;
    }
    else if (s == QLatin1String("DODGE"))
    {
        return MissType::DODGE;
    }
    else if (s == QLatin1String("EVADE"))
    {
        return MissType::EVADE;
    }
    else if (s == QLatin1String("IMMUNE"))
    {
        return MissType::IMMUNE;
    }
    else if (s == QLatin1String("MISS"))
    {
        return MissType::MISS;
    }
    else if (s == QLatin1String("PARRY"))
    {
        return MissType::PARRY;
    }
    else if (s == QLatin1String("REFLECT"))
    {
        return MissType::REFLECT;
    }
    else if (s == QLatin1String("RESIST"))
    {
        return MissType::RESIST;
    }
    else
    {
        throw CombatLogParserException(
            QStringLiteral("unexpected miss type value: %1")
                .arg(s)
                .toStdString());
    }
}

template<typename T>
T LineParser::parseNumber(const QString& s, uint8_t base)
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
                QStringLiteral("out of range: %1").arg(tmp).toStdString());
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
            QStringLiteral("error parsing number: %1, base: %2")
                .arg(s)
                .arg(base)
                .toStdString());
    }
    return val;
}

#ifndef LINEPARSER_H
#define LINEPARSER_H

#include "SubEvents.h"
#include "defs.h"
#include <QString>
#include <QStringList>

class LineParser
{
public:
    [[nodiscard]] static QStringList splitLine(QString s);
    [[nodiscard]] static uint64_t parseGUID(const QString& s);
    [[nodiscard]] static QString removequotes(QString s);
    [[nodiscard]] static UnitFlags parseUnitFlags(const QString& s);
    [[nodiscard]] static uint32_t parseSpellId(const QString& s);
    [[nodiscard]] static uint8_t parseSpellSchool(const QString& s);
    [[nodiscard]] static uint32_t parseDamageAmount(const QString& s);
    [[nodiscard]] static bool parseBool(const QString& s);
    [[nodiscard]] static AuraType parseAuraType(const QString& s);
    [[nodiscard]] static PowerType parsePowerType(const QString& s);
    [[nodiscard]] static MissType parseMissType(const QString& s);

private:
    template<typename T>
    [[nodiscard]] static T parseNumber(const QString& s, uint8_t base);
};

#endif // LINEPARSER_H

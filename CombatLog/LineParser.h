#ifndef LINEPARSER_H
#define LINEPARSER_H

#include "defs.h"
#include <QString>
#include <QStringList>
#include "SubEvents.h"

class LineParser
{
public:
    [[nodiscard]] static QStringList splitLine(QString s);
    [[nodiscard]] static uint64_t parseGUID(QString s);
    [[nodiscard]] static QString removequotes(QString s);
    [[nodiscard]] static UnitFlags parseUnitFlags(QString s);
    [[nodiscard]] static uint32_t parseSpellId(QString s);
    [[nodiscard]] static uint8_t parseSpellSchool(QString s);
    [[nodiscard]] static uint32_t parseDamageAmount(QString s);
    [[nodiscard]] static bool parseBool(QString s);
    [[nodiscard]] static AuraType parseAuraType(QString s);
    [[nodiscard]] static PowerType parsePowerType(QString s);
    [[nodiscard]] static MissType parseMissType(QString s);

private:

    template<typename T>
    [[nodiscard]] static T parseNumber(QString s, uint8_t base);
};

#endif // LINEPARSER_H

#ifndef LOGLINE_H
#define LOGLINE_H

#include <QDateTime>
#include <QString>
#include "SubEvents.h"
#include "defs.h"

class LogLine
{
public:
    [[nodiscard]] static LogLine fromRawData(QString s);

private:
    [[nodiscard]] static QStringList parseTokens(QString s);
    [[nodiscard]] static QDateTime parseTimestamp(QString s);
    [[nodiscard]] static SubEvent subeventTypeFromString(QString s);
    [[nodiscard]] static variant_t subeventValueFromString(SubEvent type, QStringList list);

    QDateTime timestamp;
    Object sourceObject;
    Object destObject;
    SubEvent subeventType;
    variant_t subEventValue;

};

#endif // LOGLINE_H

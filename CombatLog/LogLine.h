#ifndef LOGLINE_H
#define LOGLINE_H

#include "SubEvents.h"
#include "defs.h"
#include <QDateTime>
#include <QString>

class LogLine
{
public:
    LogLine(const QDateTime &timestamp, const Object &sourceObject,
            const Object &destObject, SubEvent subeventType,
            const variant_t &subEventValue);

    LogLine() = default;

    [[nodiscard]] static LogLine fromRawData(QString s);

    [[nodiscard]] bool filter() const;

    QDateTime getTimestamp() const;

    Object getSourceObject() const;

    Object getDestObject() const;

    SubEvent getSubeventType() const;

    variant_t getSubEventValue() const;

    [[nodiscard]] bool operator==(const LogLine &o) const;

private:
    [[nodiscard]] static QStringList parseTokens(QString s);
    [[nodiscard]] static QDateTime parseTimestamp(QString s);
    [[nodiscard]] static SubEvent subeventTypeFromString(QString s);
    [[nodiscard]] static variant_t subeventValueFromString(SubEvent type,
                                                           QStringList list);

    QDateTime timestamp;
    Object sourceObject;
    Object destObject;
    SubEvent subeventType;
    variant_t subEventValue;
};

#endif // LOGLINE_H

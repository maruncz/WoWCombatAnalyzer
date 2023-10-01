#ifndef COMBATLOG_H
#define COMBATLOG_H

#include "LogLine.h"
#include <QList>
#include <QStringList>
#include <optional>

class CombatLog
{
public:
    CombatLog() = default;
    CombatLog(const QList<LogLine> &lines, const QStringList &sourceNames,
              const QStringList &targetNames);

    [[nodiscard]] static CombatLog fromFile(QString filename);

    [[nodiscard]] const QList<LogLine> &getLines() const;

    [[nodiscard]] const QStringList &getSourceNames() const;

    [[nodiscard]] const QStringList &getTargetNames() const;

    template<typename T> [[nodiscard]] CombatLog filter(const T &filt) const
    {
        CombatLog ret;
        for (const auto &e : lines)
        {
            if (filt(e))
            {
                ret.append(e);
            }
        }
        ret.finalize();
        return ret;
    }

    [[nodiscard]] bool operator==(const CombatLog &o) const;

private:
    void append(LogLine line);
    void finalize();

    QList<LogLine> lines;
    QStringList sourceNames;
    QStringList targetNames;
};

#endif // COMBATLOG_H

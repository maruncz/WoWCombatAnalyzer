#ifndef COMBATLOG_H
#define COMBATLOG_H

#include <QList>
#include "LogLine.h"

class CombatLog
{
public:

    [[nodiscard]] static CombatLog fromFile(QString filename);

private:
    QList<LogLine> lines;
};

#endif // COMBATLOG_H

#include "CombatLog.h"
#include <QFile>
#include "exceptions.h"

CombatLog CombatLog::fromFile(QString filename)
{
    QFile file{filename};

    if(!file.open(QFile::ReadOnly))
    {
        throw CombatLogParserException(QString("Cannot open log file: %1, %2").arg(filename,file.errorString()).toStdString());
    }

    CombatLog ret;

    while(!file.atEnd())
    {
        auto line = file.readLine(512);

        if(line.size() >= 512)
        {
            throw CombatLogParserException(QString("Line too long: %1").arg(line.size()).toStdString());
        }

        ret.lines.append(LogLine::fromRawData(line));
    }


    return ret;
}

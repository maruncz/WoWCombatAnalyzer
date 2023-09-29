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

        ret.append(LogLine::fromRawData(line));
    }

    ret.finalize();
    return ret;
}

const QList<LogLine> &CombatLog::getLines() const
{
    return lines;
}

void CombatLog::append(LogLine line)
{
    lines.append(line);

    {
        auto sourceName = line.getSourceObject().name;
        if(!sourceNames.contains(sourceName))
        {
            sourceNames.append(sourceName);
        }
    }
    {
        auto targetName = line.getDestObject().name;
        if(!targetNames.contains(targetName))
        {
            targetNames.append(targetName);
        }
    }
}

void CombatLog::finalize()
{
    sourceNames.sort();
    targetNames.sort();
}



const QStringList& CombatLog::getTargetNames() const
{
    return targetNames;
}



const QStringList& CombatLog::getSourceNames() const
{
    return sourceNames;
}

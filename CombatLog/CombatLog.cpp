#include "CombatLog.h"
#include "exceptions.h"
#include <QFile>

CombatLog::CombatLog(const QList<LogLine> &lines,
                     const QStringList &sourceNames,
                     const QStringList &targetNames)
    : lines{lines}, sourceNames{sourceNames}, targetNames{targetNames}
{
}

CombatLog CombatLog::fromFile(QString filename)
{
    QFile file{filename};

    if (!file.open(QFile::ReadOnly))
    {
        throw CombatLogParserException(
            QStringLiteral("Cannot open log file: %1, %2")
                .arg(filename, file.errorString())
                .toStdString());
    }

    CombatLog ret;

    while (!file.atEnd())
    {
        auto line = file.readLine(512);

        if (line.size() >= 512)
        {
            throw CombatLogParserException(QStringLiteral("Line too long: %1")
                                               .arg(line.size())
                                               .toStdString());
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

void CombatLog::append(const LogLine& line)
{
    lines.append(line);

    {
        auto sourceName = line.getSourceObject().name;
        if (!sourceNames.contains(sourceName))
        {
            sourceNames.append(sourceName);
        }
    }
    {
        auto targetName = line.getDestObject().name;
        if (!targetNames.contains(targetName))
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

const QStringList &CombatLog::getTargetNames() const
{
    return targetNames;
}

bool CombatLog::operator==(const CombatLog &o) const
{
    if (sourceNames != o.sourceNames)
    {
        return false;
    }

    if (targetNames != o.targetNames)
    {
        return false;
    }

    if (lines != o.lines)
    {
        return false;
    }

    return true;
}

const QStringList &CombatLog::getSourceNames() const
{
    return sourceNames;
}

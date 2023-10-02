#include "healdatamodel.h"
#include "../CombatLog/CombatLog.h"

HealDataModel::HealDataModel(CombatLog *log, QObject *parent)
    : QAbstractTableModel(parent), combatlog{log}
{
}

QVariant HealDataModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        return QString::fromStdString(std::string{header.at(section)});
    }

    return {};
}

int HealDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return combatlog->getLines().size();
}

int HealDataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return header.size();
}

QVariant HealDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    if (role == Qt::DisplayRole)
    {
        const auto &line = combatlog->getLines().at(index.row());
        switch (index.column())
        {
            case 0:
            {
                return line.getTimestamp();
            }
            case 1:
            {
                return line.getSourceObject().name;
            }
            case 2:
            {
                return line.getDestObject().name;
            }
            case 3:
            {
                return std::visit(
                    [](auto &&arg) -> QString
                    {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_base_of_v<detail::prefix::Spell,
                                                        T>)
                        {
                            return arg.spell.name;
                        }
                        else
                        {
                            throw std::runtime_error("unhandled case");
                        }
                    },
                    line.getSubEventValue());
            }
            case 4:
            {
                return std::visit(
                    [](auto &&arg) -> uint32_t
                    {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_base_of_v<detail::suffix::Heal,
                                                        T>)
                        {
                            return arg.amount;
                        }
                        else
                        {
                            throw std::runtime_error("unhandled case");
                        }
                    },
                    line.getSubEventValue());
            }
        }
    }

    return {};
}

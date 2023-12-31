#ifndef HEALDATAMODEL_H
#define HEALDATAMODEL_H

#include <QAbstractTableModel>
#include <array>
#include <string_view>
#include <vector>

class LogLine;
class CombatLog;

class HealDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit HealDataModel(CombatLog * log, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    static constexpr auto header = std::to_array<std::string_view>({"Timestamp"
                                                                 , "Source"
                                                                 , "Target"
                                                                 , "Spell",
                                                                 "Amount"});

    CombatLog *combatlog{nullptr};
};

#endif // HEALDATAMODEL_H

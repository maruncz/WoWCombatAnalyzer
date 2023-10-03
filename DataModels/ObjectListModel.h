#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <utility>

class ObjectListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ObjectListModel(QObject *parent = nullptr);

    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex& parent
                               = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role = Qt::DisplayRole) const override;

    void setList(const QStringList *newList);

private:
    const QStringList *list{nullptr};
};

#endif // OBJECTLISTMODEL_H

#include "ObjectListModel.h"

ObjectListModel::ObjectListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int ObjectListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    if(!list)
    {
        return 0;
    }

    return list->size();
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(!list)
    {
        return {};
    }


    if(role == Qt::DisplayRole)
    {
        return list->at(index.row());
    }




    return {};
}

void ObjectListModel::setList(const QStringList *newList)
{
    auto oldRows{0};
    if(list)
    {
        oldRows = list->size();
    }
    list = newList;
    emit dataChanged(QAbstractItemModel::createIndex(0,0), QAbstractItemModel::createIndex(oldRows,0));
}

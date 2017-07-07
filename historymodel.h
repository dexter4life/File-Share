#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QStandardItemModel>

class HistoryModel : public QStandardItemModel
{
       Q_OBJECT
public:
    HistoryModel( QObject *parent = 0);
    ~HistoryModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // HISTORYMODEL_H

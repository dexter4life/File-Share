#ifndef CLIENTVIEWMODEL_H
#define CLIENTVIEWMODEL_H

#include <QStandardItemModel>
#include <QtCore>

class ClientViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ClientViewModel(QObject *parent = 0);
    ClientViewModel(int rows, int columns, QObject *parent = 0):
        QStandardItemModel(rows,columns,parent){}

    ~ClientViewModel();

    Qt::ItemFlags flags( const QModelIndex &index ) const;

};

#endif // CLIENTVIEWMODEL_H

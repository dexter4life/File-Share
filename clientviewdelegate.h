#ifndef CLIENTVIEWDELEGATE_H
#define CLIENTVIEWDELEGATE_H

#include <QAbstractItemDelegate>

class ClientViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    ClientViewDelegate(QObject *parent = 0);
    ~ClientViewDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CLIENTVIEWDELEGATE_H

#include "clientviewdelegate.h"
#include <QtWidgets>

ClientViewDelegate::ClientViewDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{

}

ClientViewDelegate::~ClientViewDelegate()
{

}

void ClientViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( option.state & QStyle::State_Selected)
        painter->fillRect(rect(), option.palette.highlight());

    option.styleObject();
}


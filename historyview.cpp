/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "historyview.h"
#include <QtWidgets>
#include <QtCore>

historyView::historyView(QWidget *parent)
    : QTableWidget(parent)
{
    setRowCount(0);

    menu = new QMenu(this);
    QList< QAction * > actions;

    send = new QAction(QIcon(":/image/send-file-128.png"),"Send",menu);
    del = new QAction(QIcon(":/image/delete-512.png"),"Delete", menu);

    // connections here

   // connect(del, SIGNAL(triggered()), this, SLOT(deleteRow()));
    connect(send, SIGNAL(triggered()), this, SIGNAL(sendFile()));

    actions << send << del;
    menu->addActions(actions);
}

historyView::~historyView(){}

void historyView::contextMenuEvent(QContextMenuEvent *event)
{
    if( event->isAccepted())
    {
        menu->exec(event->globalPos());
    }
}



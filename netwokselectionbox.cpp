/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include <QtWidgets>
#include "netwokselectionbox.h"

#include <QSignalMapper>

NetwokSelectionBox::NetwokSelectionBox(QWidget *parent)
    : QComboBox(parent)
{
    connect(this, SIGNAL(activated(int)), this, SLOT(remit(int)));
}

NetwokSelectionBox::~NetwokSelectionBox()
{

}

void NetwokSelectionBox::mousePressEvent(QMouseEvent *e)
{


    beforeIndex  = currentIndex();

    QComboBox::mousePressEvent(e);
}

void NetwokSelectionBox::remit(int item)
{
    if( beforeIndex != item)
    {
        emit beforeCurrent(beforeIndex);
    }
}

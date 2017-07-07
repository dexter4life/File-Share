/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "sendfilebutton.h"
#include <QtWidgets>

SendFileButton::SendFileButton(QWidget *parent)
    : QPushButton(parent)
{

}

SendFileButton::~SendFileButton()
{

}

void SendFileButton::mousePressEvent(QMouseEvent *event)
{

   if( event->isAccepted() )
    {
        if( event->button() == Qt::LeftButton )
        {
           if( event->modifiers().testFlag(Qt::ControlModifier) )
               emit ctrlOnClick(); /// control key pressed
        }

        QPushButton::mousePressEvent(event);
    }
}


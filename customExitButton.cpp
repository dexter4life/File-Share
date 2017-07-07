/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "customExitButton.h"
#include <QtWidgets>
#include <QPointer>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
CustomExitButton::CustomExitButton(QWidget *parent)
    :QAbstractButton(parent)
{
    imageList  << QPixmap(":/background/image/exitButton01.png")
               << QPixmap(":/background/image/exitButton00.png")
               << QPixmap(":/background/image/exitButtonBark.png");
    m_Catche = &imageList[0];

    this->setAcceptDrops(false);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
CustomExitButton::~CustomExitButton(){}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomExitButton::enterEvent(QEvent *event)
{
    if( event->isAccepted() )
    {
        m_Catche = &imageList[1];
    }
    QAbstractButton::enterEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomExitButton::leaveEvent(QEvent *event)
{
    if( event->isAccepted())
    {
        m_Catche = &imageList[0];
    }
    QAbstractButton::leaveEvent(event);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomExitButton::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        m_Catche = &imageList[2];
    }
    QAbstractButton::mousePressEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomExitButton::mouseReleaseEvent(QMouseEvent *event)
{
     if( event->isAccepted())
     {
        m_Catche = &imageList[1];
     }
     QAbstractButton::mouseReleaseEvent(event);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CustomExitButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.drawPixmap(rect(), *m_Catche);
    painter.restore();

}



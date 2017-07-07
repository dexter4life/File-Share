/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "minimizebutton.h"
#include <QtWidgets>
#include <Windows.h>

MinimizeButton::MinimizeButton(QWidget *parent)
    :QAbstractButton(parent)
{

    imageList <<QPixmap(":/image/minimizeButtonDark.png")
              << QPixmap(":/image/minimizeButton01.png")
              << QPixmap(":/image/minimizeButton.png");

    m_Catche = &imageList[1];

    setAcceptDrops(false);
}

MinimizeButton::~MinimizeButton()
{
}

void MinimizeButton::enterEvent(QEvent *event)
{
    m_Catche = &imageList[2];
    QAbstractButton::enterEvent(event);
}

void MinimizeButton::leaveEvent(QEvent *event)
{
    m_Catche = &imageList[1];
    QAbstractButton::enterEvent(event);
}

void MinimizeButton::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton ){
        m_Catche = &imageList[2];
        QAbstractButton::mousePressEvent(event);
    }
}

void MinimizeButton::mouseReleaseEvent(QMouseEvent *event)
{
     if( event->isAccepted())
     {
      m_Catche = &imageList[0];
      QAbstractButton::mouseReleaseEvent(event);
     }
}

void MinimizeButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.drawPixmap(rect(), *m_Catche);
    painter.restore();

}

//delete later
void MinimizeButton::initButton(QPixmap *image)
{
    this->m_Catche = image;
}

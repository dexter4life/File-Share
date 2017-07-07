/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "hotspotbutton.h"
#include <QtWidgets>


HotspotButton::HotspotButton(QWidget *parent)
    : QAbstractButton(parent)
{
    imageList  << QPixmap(":/image/hotspot_1.png")
               << QPixmap(":/image/hotspot_2.png")
               << QPixmap(":/image/hotspot_3.png");
    m_Catche = &imageList[0];
    wifiOn = false;

    this->setAcceptDrops(false);

    step=0;
}

HotspotButton::~HotspotButton()
{
}

void HotspotButton::enterEvent(QEvent *event)
{
    m_Catche = &imageList[1];
    QAbstractButton::enterEvent(event);
}

void HotspotButton::leaveEvent(QEvent *event)
{
    m_Catche = &imageList[0];
    QAbstractButton::leaveEvent(event);
}

void HotspotButton::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        m_Catche = &imageList[2];
        QAbstractButton::mousePressEvent(event);
        repaint();
    }
}

void HotspotButton::mouseReleaseEvent(QMouseEvent *event)
{
     if( event->isAccepted())
     {
        m_Catche = &imageList[1];
        QAbstractButton::mouseReleaseEvent(event);
     }
}

void HotspotButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    if(this->isChecked())
    {
        timer.start(250, this);
        //if( timer.isActive() )
        m_Catche = &imageList[step % 2];
    }
    else
    {

        timer.stop();
    }
    painter.drawPixmap(rect(), *m_Catche);
    painter.restore();
}

void HotspotButton::timerEvent(QTimerEvent *event)
{
    if( event->timerId() == timer.timerId()){
        step++;
        update();
    }
}



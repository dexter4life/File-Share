/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "profilepicture.h"
#include <QtWidgets>

ProfilePicture::ProfilePicture(QWidget *parent, QPixmap pixmap) : QWidget(parent),
    pix( pixmap )
{
    setAcceptDrops(false);
}

ProfilePicture::~ProfilePicture()
{

}

void ProfilePicture::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 5, Qt::SolidLine);
    painter.setPen(pen);

    pix.setDevicePixelRatio(10);

    QRegion region(rect(), QRegion::Ellipse);
    painter.setClipRegion(region);
    if( !pix ){
        painter.fillRect(rect(),Qt::blue);
    }
    else
       painter.drawPixmap(rect(), pix);

    painter.drawEllipse(rect());

    painter.restore();
    QWidget::paintEvent(event);
}

void ProfilePicture::setPixmap(QPixmap &pixmap)
{
    pix = pixmap;

    repaint();
}


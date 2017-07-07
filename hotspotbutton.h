#ifndef HOTSPOTBUTTON_H
#define HOTSPOTBUTTON_H

#include <QAbstractButton>
#include <QBasicTimer>

class HotspotButton : public QAbstractButton
{
    Q_OBJECT
public:
    HotspotButton(QWidget *parent = 0);
    ~HotspotButton();


    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event );
    virtual void timerEvent(QTimerEvent *event);
    inline void setActive(bool state){ wifiOn = state;}

private:
    QBasicTimer timer;
    bool wifiOn;
    QList< QPixmap> imageList;
    QPixmap *m_Catche;
    int step;
};

#endif // HOTSPOTBUTTON_H

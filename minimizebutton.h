#ifndef MINIMIZEBUTTON_H
#define MINIMIZEBUTTON_H

#include <QAbstractButton>
#include <QList>

class MinimizeButton : public QAbstractButton
{
    Q_OBJECT
public:
    MinimizeButton(QWidget *parent = 0);
    ~MinimizeButton();

    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event );


private:
    QPixmap *m_Catche;
    QList< QPixmap > imageList;

    void initButton(QPixmap *image);
};

#endif // MINIMIZEBUTTON_H

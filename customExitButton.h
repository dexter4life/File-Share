#ifndef TOPLEVELBUTTONS_H
#define TOPLEVELBUTTONS_H

#include <QAbstractButton>
#include <QList>

class CustomExitButton : public QAbstractButton
{
    Q_OBJECT
public:
    CustomExitButton(QWidget *parent = 0);
    ~CustomExitButton();

    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event );

private:
    QList< QPixmap > imageList;
    QPixmap *m_Catche;
};

#endif // TOPLEVELBUTTONS_H

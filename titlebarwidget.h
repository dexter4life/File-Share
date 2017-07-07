#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class MinimizeButton;
class QHBoxLayout;
class CustomExitButton;
class ButtonSet;
QT_END_NAMESPACE

class TitleBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBarWidget(QWidget *parent = 0);
    ~TitleBarWidget();
    virtual void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
public slots:

private:
    ButtonSet *buttons;
    bool m_draged;
    QPoint m_dragPos;
    MinimizeButton *m_Minimize;
    QHBoxLayout *m_titleBarLayout;
    CustomExitButton  *exitButton;
signals:
    void minimizeWindow();
    void moveWindow(QPoint pos);
    void closeWindow();
    void displaySetting();
    void displayRecent();
    void displayHome();
};

#endif // TITLEBARWIDGET_H

#ifndef BUTTONSET_H
#define BUTTONSET_H

#include <QWidget>

class QAction;
class QToolBar;

class ButtonSet : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonSet(QWidget *parent = 0);
    ~ButtonSet();

signals:
    void settingClicked();
    void homeButtonClicked();
    void recentButtonClicked();
private:
    QAction *viewHistory, *settingView, *homeView;
    QToolBar *toolbar;
};

#endif // BUTTONSET_H

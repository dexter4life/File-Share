#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QTableWidget>
#include <QAction>
#include <QMenu>

class historyView : public QTableWidget
{
    Q_OBJECT
public:
    historyView(QWidget *parent = 0);
    ~historyView();

    void contextMenuEvent(QContextMenuEvent *event);

    bool newItem;
private:
    QMenu *menu;
    QAction *send, *del;
signals:
    void rowDeleted();
    void sendFile();
};

#endif // HISTORYVIEW_H

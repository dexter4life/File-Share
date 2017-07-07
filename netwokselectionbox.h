#ifndef NETWOKSELECTIONBOX_H
#define NETWOKSELECTIONBOX_H

#include <QComboBox>

class NetwokSelectionBox : public QComboBox
{
    Q_OBJECT
public:
    NetwokSelectionBox(QWidget *parent = 0);
    ~NetwokSelectionBox();

    void mousePressEvent(QMouseEvent *e);
signals:
    void beforeCurrent(int item);
public slots:
    void remit( int item );
private:
    // before the currentIndexChanged() signals is called
    int beforeIndex;
};

#endif // NETWOKSELECTIONBOX_H

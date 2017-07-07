#ifndef SENDFILEBUTTON_H
#define SENDFILEBUTTON_H


#include <QPushButton>

class SendFileButton : public QPushButton
{
    Q_OBJECT
public:
    SendFileButton(QWidget *parent = 0);
    ~SendFileButton();

    virtual void mousePressEvent(QMouseEvent *event);

signals:
    void ctrlOnClick();
};

#endif // SENDFILEBUTTON_H

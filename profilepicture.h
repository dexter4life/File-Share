#ifndef PROFILEPICTURE_H
#define PROFILEPICTURE_H

#include <QWidget>

class ProfilePicture : public QWidget
{
    Q_OBJECT
public:
    explicit ProfilePicture(QWidget *parent = 0, QPixmap pixmap = QPixmap());
    ~ProfilePicture();

    virtual void paintEvent(QPaintEvent *event);

    void setPixmap( QPixmap &pixmap );

private:
    QPixmap pix;
};

#endif // PROFILEPICTURE_H

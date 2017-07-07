#ifndef APPBUTTONWIDGET_H
#define APPBUTTONWIDGET_H

#include <QWidget>
#include <QList>

class AppButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppButtonWidget(QWidget *parent = 0);
    ~AppButtonWidget();

signals:

public slots:
private:
    QList< QPixmap > imageList;
};

#endif // APPBUTTONWIDGET_H

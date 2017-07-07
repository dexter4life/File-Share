#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>

class FileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileWidget(QString path, QWidget *parent );
    explicit FileWidget(QString path);
    ~FileWidget();

    inline void setRow(int r) {row = r;}
    inline int getRow() const { return row; }

    void setFileIcon(QString iconPath);
    void setFileName(QString name);
    void setFileSize(qint64 size);
    QString sizePrefix(qint64 size);

    void InitializeWidget(QString path);
    void setTransferStatus(qint64 status);
public slots:
    void setWidgetInformation(QString path);
    void triggered();
    void updateProgressBar(double value);
signals:
    void buttonClicked(int row);

private:
    int row;
    int progress;
    QLabel *iconImage, *fileName, *fileSize, *transferStatus;
  };

#endif // FILEWIDGET_H

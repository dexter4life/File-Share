/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "filewidget.h"
#include <QFileIconProvider>
#include <QtWidgets>
#include "customExitButton.h"
#include "global.h"

void FileWidget::setWidgetInformation(QString path)
{
    QFileInfo file(path);
    setToolTip(path);
    setFileIcon(path);
    setFileName(file.fileName());
    setFileSize(file.size());
}

FileWidget::FileWidget(QString path)
    : progress(0)
{
    InitializeWidget(path);
}

void FileWidget::InitializeWidget(QString path)
{
    setFixedSize(600, 50);

    CustomExitButton *msg = new CustomExitButton(this);
    msg->setFixedSize(15,15);

    setRow(0);

    iconImage = new QLabel(this);

    fileName = new QLabel(this);
    fileName->setFixedWidth(300);
    fileName->move(50, 2);

    fileSize = new QLabel(this);
    fileSize->move(50,20);

    transferStatus = new QLabel(this);
    transferStatus->move(530, 20);

    QGridLayout *layout = new QGridLayout(this);
    layout->setHorizontalSpacing(1);

    layout->addWidget(msg, 0, 3, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(iconImage, 0, 0, Qt::AlignLeft | Qt::AlignCenter);

    setWidgetInformation(path);

    connect(msg, SIGNAL(clicked()), this, SLOT(triggered()));
    connect(msg, SIGNAL(clicked()), this, SLOT(close()));
}

FileWidget::FileWidget(QString path, QWidget *parent)
    : QWidget(parent)
{
    InitializeWidget(path);
}

FileWidget::~FileWidget()
{

}
void FileWidget::setTransferStatus(qint64 status)
{
    if( status == 100 )
        transferStatus->setText("Completed");
    else
        transferStatus->setText(QString("%1%").arg(status));
}
void FileWidget::setFileName(QString name) { fileName->setText(name); }

void FileWidget::setFileSize(qint64 size) { fileSize->setText("size: " + sizePrefix(size)); }

void FileWidget::setFileIcon(QString iconPath)
{
    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon(QFileInfo(iconPath));
    iconImage->setPixmap(icon.pixmap(iconImage->size()));
}
void FileWidget::triggered()
{
    emit buttonClicked(row);
}

void FileWidget::updateProgressBar(double value)
{
    PRINTLOG(tr("%1").arg(value).toLatin1());

    if( transferStatus->text() == "Completed")
        return;

    setTransferStatus((qint64)value);

}
QString FileWidget::sizePrefix(qint64 size)
{
    if(size < 1024)
        return QString("%1byte").arg(QString::number(size));
    if(size >= 1024 && size < 1048576 )
        return QString("%1KB").arg(QString::number(double(size)/1024, 'f', 2));
    if(size >= 1048576 && size < 1073741824)
        return QString("%1MB").arg(QString::number(double(size)/1048576, 'f', 2));
    return "";
}

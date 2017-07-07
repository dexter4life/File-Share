#include "fileexplorer.h"
#include "ui_fileexplorer.h"


FileExplorer::FileExplorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileExplorer)
{
    ui->setupUi(this);

   // setWindowFlags(Qt::Widget);

    model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());

    ui->listView->setModel(model);
    ui->listView->setRootIndex(model->index(QDir::currentPath()));

    connect(ui->listView, &QAbstractItemView::clicked, this, &FileExplorer::changeDir);
}

FileExplorer::~FileExplorer()
{
    delete ui;
}

void FileExplorer::changeDir(QModelIndex index)
{
    model->setRootPath(index.data().toString());
    ui->listView->setModel(model);
}

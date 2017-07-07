/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include <QApplication>
#include "fileshare.h"
#include <QStyleFactory>
#include <QException>
#include <QMessageBox>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/Share.png"));

    a.setStyle(QStyleFactory::create("fusion"));
    FileShare window;
    window.show();
    return a.exec();
}

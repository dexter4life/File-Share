/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "buttonset.h"
#include <QAction>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>

ButtonSet::ButtonSet(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(false);

    viewHistory = new QAction(QIcon(":/image/viewHistory.png"),tr("History"), this);

    homeView = new QAction(QIcon(QStringLiteral(":/image/Apps-Home-icon.png")),tr("Home"), this);
    homeView->setToolTip(tr("Home View"));

    settingView = new QAction(QIcon(QStringLiteral(":/image/Settings-icon.png")),"Settings", this);
    settingView->setToolTip(tr("Settings"));

    toolbar = new QToolBar(this);
    toolbar->move(pos());
    toolbar->setFixedHeight(60);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolbar->setIconSize(QSize(50,30));


    QList< QAction * > list;

    list << homeView << viewHistory << settingView;
    toolbar->addActions(list);

    connect(settingView, SIGNAL(triggered()), this, SIGNAL(settingClicked()));
    connect(homeView, SIGNAL(triggered()), this, SIGNAL(homeButtonClicked()));
    connect(viewHistory, SIGNAL(triggered()), this, SIGNAL(recentButtonClicked()));
}

ButtonSet::~ButtonSet(){}


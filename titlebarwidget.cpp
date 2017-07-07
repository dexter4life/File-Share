/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "titlebarwidget.h"
#include "minimizebutton.h"
#include "customExitButton.h"
#include "buttonset.h"

#include <QtWidgets>
#include <QtGui>

TitleBarWidget::~TitleBarWidget(){}

TitleBarWidget::TitleBarWidget(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(false);
    m_Minimize = new MinimizeButton(this);
    m_Minimize->setFixedSize(17,17);
    m_Minimize->move(550, 10);

    exitButton = new CustomExitButton(this);
    exitButton->setFixedSize(17,17);
    exitButton->move(570, 10);


    QLabel *label = new QLabel(this);
    label->setFont(QFont("Consolas", 16, QFont::Bold));
    label->setText("Swift v1.0");
    label->setBackgroundRole(QPalette::BrightText);
    label->setStyleSheet(QStringLiteral("QLabel {color: white}"));
    label->move(70,15);

    QWidget *logo = new QWidget(this);
    logo->setFixedSize(50,50);
    logo->setStyleSheet(QStringLiteral("QWidget { image: url(:/image/share-icon.png); }"));
    logo->move(5,5);

    connect(m_Minimize, SIGNAL(clicked()), this, SIGNAL(minimizeWindow()));
    connect(exitButton, SIGNAL(clicked()), this, SIGNAL(closeWindow()));

    buttons = new ButtonSet(this);
    buttons->setFixedWidth(200);
    buttons->move(200, 1);
    connect(buttons, SIGNAL(settingClicked()), this, SIGNAL(displaySetting()));
    connect(buttons, SIGNAL(homeButtonClicked()), this, SIGNAL(displayHome()));
    connect(buttons, SIGNAL(recentButtonClicked()), this, SIGNAL(displayRecent()));
}

void TitleBarWidget::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    painter.save();
    painter.drawImage(rect(),QImage(":/image/background.jpg"));
    painter.restore();
    QWidget::paintEvent(e);
}

void TitleBarWidget::mouseMoveEvent(QMouseEvent *e)
{
    if( m_draged){
        emit moveWindow(e->globalPos() - m_dragPos );
        e->accept();
    }
}

void TitleBarWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    m_draged = false;
}

void TitleBarWidget::mousePressEvent(QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton){
        m_draged = true;
        m_dragPos = e->globalPos() - this->mapToGlobal(pos());
        e->accept();
    }
}


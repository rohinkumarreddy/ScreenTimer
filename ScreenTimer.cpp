#include "ScreenTimer.h"
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QLayout>
#include <QPushButton>
#include <QPainter>
#include "BlurPanel.h"
#include "centralWgt.h"

static const unsigned int CNTRL_SH_INT = 1500;//msec
static const float WGT_WDTH_PERC = 0.15f;//widget width percentage of screen
static const float WGT_HGHT_PERC = 0.07f;//widget height percentage of screen
static const int SCRN_X_OFFSET = 5;//screen offset in x axis
static const int SCRN_Y_OFFSET = 5;//scrren offset in y axis

ScreenTimer::ScreenTimer(QWidget *parent)
: QMainWindow(parent)
{
    QDesktopWidget qdw;
    QRect scrnRct = qdw.availableGeometry();

    setupUi(QSize(scrnRct.size()));
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setAttribute(Qt::WA_NoSystemBackground, true);

	this->move(scrnRct.x() + scrnRct.width() * (1 - WGT_WDTH_PERC) - SCRN_X_OFFSET, scrnRct.y() + SCRN_Y_OFFSET);

    /*curved frame*/
    const int radius = 10;
    QPainterPath pPath;
    pPath.addRoundedRect(this->rect(), radius, radius);
    QRegion mask = QRegion(pPath.toFillPolygon().toPolygon());
    this->setMask(mask);

    /*enable mouse tracking*/
    centralWidget->setMouseTracking(true);
}

ScreenTimer::~ScreenTimer()
{
    //m_pBlur is dynamically deleted
    /*if (m_pBlur != Q_NULLPTR)
        m_pBlur->deleteLater();*/
}

void ScreenTimer::setupUi(QSize scrnSz)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());

    printf("screen width %d height %d\n", scrnSz.width(), scrnSz.height());
    printf("widget width %f height %f\n", scrnSz.width() * WGT_WDTH_PERC, scrnSz.height() * WGT_HGHT_PERC);
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("ScreenTimer"));
	this->resize(scrnSz.width() * WGT_WDTH_PERC, scrnSz.height() * WGT_HGHT_PERC);
    this->setSizePolicy(sizePolicy);
    this->setMinimumSize(QSize(scrnSz.width() * WGT_WDTH_PERC, scrnSz.height() * WGT_HGHT_PERC));
    this->setMaximumSize(QSize(scrnSz.width() * WGT_WDTH_PERC, scrnSz.height() * WGT_HGHT_PERC));
    
    centralWidget = new CentralWgt(scrnSz, this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy);
    centralWidget->setMinimumSize(QSize(scrnSz.width() * WGT_WDTH_PERC, scrnSz.height() * WGT_HGHT_PERC));
    centralWidget->setMaximumSize(QSize(scrnSz.width() * WGT_WDTH_PERC, scrnSz.height() * WGT_HGHT_PERC));

    this->setCentralWidget(centralWidget);

    //QMetaObject::connectSlotsByName(this);
} // setupUi
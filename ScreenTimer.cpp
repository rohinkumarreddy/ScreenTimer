#include "ScreenTimer.h"
#include <QDesktopWidget>
#include <QMouseEvent>
#include "BlurPanel.h"

static const unsigned int CNTRL_SH_INT = 1500;//msec

ScreenTimer::ScreenTimer(QWidget *parent)
: QMainWindow(parent),
m_InWid(false),
m_WidMov(false),
m_pBlur(Q_NULLPTR),
m_pNoBlur(Q_NULLPTR),
m_Pos(0,0)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    this->setMouseTracking(true);
    QDesktopWidget qdw;
    QRect scrnRct = qdw.availableGeometry();
    this->move(scrnRct.x()+scrnRct.width()-255, scrnRct.y()+5);
    
    QSizePolicy sz = ui.xBtn->sizePolicy();
    sz.setRetainSizeWhenHidden(true);
    ui.xBtn->setSizePolicy(sz);
    ui.xBtn->setVisible(false);
    QPixmap xPix(":/ScreenTimer/res/close.png");
    xPix.scaled(ui.xBtn->size());
    ui.xBtn->setIcon(QIcon(xPix));

    sz = ui.sBtn->sizePolicy();
    sz.setRetainSizeWhenHidden(true);
    ui.sBtn->setSizePolicy(sz);
    ui.sBtn->setVisible(false);
    QPixmap sPix(":/ScreenTimer/res/play.png");
    sPix.scaled(ui.sBtn->size());//QSize(40,36));
    ui.sBtn->setIcon(QIcon(sPix));
    
    sz = ui.rBtn->sizePolicy();
    sz.setRetainSizeWhenHidden(true);
    ui.rBtn->setSizePolicy(sz);
    ui.rBtn->setVisible(false);
    QPixmap rPix(":/ScreenTimer/res/retry.png");
    rPix.scaled(ui.rBtn->size());
    ui.rBtn->setIcon(QIcon(rPix));

    m_BtnTmr.setSingleShot(true);

    /*curved frame*/
    const int radius = 10;
    QPainterPath pPath;
    pPath.addRoundedRect(this->rect(), radius, radius);
    QRegion mask = QRegion(pPath.toFillPolygon().toPolygon());
    this->setMask(mask);

    /*blur effect*/
    m_pNoBlur = this->graphicsEffect();

    connect(ui.sBtn, &QPushButton::clicked, this, &ScreenTimer::sClick);
    connect(ui.rBtn, &QPushButton::clicked, this, &ScreenTimer::rClick);
    connect(ui.xBtn, &QPushButton::clicked, this, &ScreenTimer::xClick);
    connect(&m_BtnTmr, &QTimer::timeout, this, &ScreenTimer::cntrlOff);
}

ScreenTimer::~ScreenTimer()
{
    //m_pBlur is dynamically deleted
    /*if (m_pBlur != Q_NULLPTR)
        delete m_pBlur;*/
}

void ScreenTimer::mousePressEvent(QMouseEvent* e)
{
    //setCursor(Qt::OpenHandCursor);
    /*m_Pos = e->pos();
    m_WidMov = true;*/
    QWidget::mousePressEvent(e);
}

void ScreenTimer::mouseReleaseEvent(QMouseEvent* e)
{
    //setCursor(Qt::ArrowCursor);
    m_WidMov = false;

    Q_UNUSED(e)
}

void ScreenTimer::mouseDoubleClickEvent(QMouseEvent* e)
{
    Q_UNUSED(e)
}

void ScreenTimer::enterEvent(QEvent* e)
{
    m_InWid = true;
    setAttribute(Qt::WA_TranslucentBackground, false);
    setAttribute(Qt::WA_NoSystemBackground, false);
    //m_pBlur is dynamically deleted
    /*if (m_pBlur != Q_NULLPTR)
        delete m_pBlur;*/
	m_pBlur = new BlurPanel();
    this->setGraphicsEffect(m_pBlur);
    //update();
    ui.xBtn->setVisible(true);
    ui.sBtn->setVisible(true);
    ui.rBtn->setVisible(true);
    QWidget::enterEvent(e);
}

void ScreenTimer::leaveEvent(QEvent* e)
{
    m_InWid = false;
    m_BtnTmr.start(CNTRL_SH_INT);
    QWidget::leaveEvent(e);
}

void ScreenTimer::mouseMoveEvent(QMouseEvent* e)
{
    //setAttribute(Qt::WA_TranslucentBackground, false);
    //setAttribute(Qt::WA_NoSystemBackground, false);
    ////this->setGraphicsEffect(m_pBlur);
    //update();
    //ui.sBtn->setVisible(true);
    //ui.rBtn->setVisible(true);

    /*if (m_WidMov)
    {
        QPoint diff = e->pos() - m_Pos;
        window()->move(window()->pos() + diff);
    }*/
    QWidget::mouseMoveEvent(e);
}

void ScreenTimer::sClick()
{

}

void ScreenTimer::rClick()
{

}

void ScreenTimer::xClick()
{
    close();
    exit(0);
}

void ScreenTimer::cntrlOff()
{
    if (/*!isActiveWindow() &&*/ !m_InWid)
    {
        ui.xBtn->setVisible(false);
        ui.sBtn->setVisible(false);
        ui.rBtn->setVisible(false);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, true);
        this->setGraphicsEffect(m_pNoBlur);
        //m_pBlur is dynamically deleted
        /*if (m_pBlur != Q_NULLPTR)
            delete m_pBlur;*/
        update();
    }
}

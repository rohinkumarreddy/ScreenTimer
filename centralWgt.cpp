#include "centralWgt.h"
#include <QMouseEvent>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QPainter>
#include "BlurPanel.h"

static const unsigned int CNTRL_SH_INT = 1500;//Panel dissappears in msec
static const float TIMER_INT = 10;//update interval msec
static const float BTN_SZ_PERC = 0.09f;//button size percentage
static const float WGT_WDTH_PERC = 0.15f;//widget width percentage of screen
static const float WGT_HGHT_PERC = 0.07f;//widget height percentage of screen
static const int X_OFFSET = 5;//screen offset in x axis
static const int Y_OFFSET = 5;//scrren offset in y axis

CentralWgt::CentralWgt(QSize scrnSz, QWidget* parent)
:QWidget(parent),
m_InWid(false),
m_WidMov(false),
m_Run(false),
m_Edit(true),
m_pBlur(Q_NULLPTR),
m_pNoBlur(Q_NULLPTR),
m_pHorizontalLayout(Q_NULLPTR),
m_pVerticalLayout(Q_NULLPTR),
m_pXbtn(Q_NULLPTR),
m_pSbtn(Q_NULLPTR),
m_pRbtn(Q_NULLPTR),
m_pSpacer(Q_NULLPTR),
m_Pos(0, 0),
m_ScrnSz(scrnSz),
m_M1(0),
m_M2(0),
m_S1(0),
m_S2(0),
m_SS1(0),
m_SS2(0)
{
    setupUi();
    m_BtnTmr.setSingleShot(true);
    m_Timer.setInterval(TIMER_INT);

    /*blur effect*/
    m_pNoBlur = this->graphicsEffect();

    connect(m_pSbtn, &QPushButton::clicked, this, &CentralWgt::sClick);
    connect(m_pRbtn, &QPushButton::clicked, this, &CentralWgt::rClick);
    connect(m_pXbtn, &QPushButton::clicked, this, &CentralWgt::xClick);
    connect(&m_BtnTmr, &QTimer::timeout, this, &CentralWgt::cntrlOff);
    connect(&m_Timer, &QTimer::timeout, this, &CentralWgt::timerTick);
};

void CentralWgt::enterEvent(QEvent* e)
{
    printf("CentralWgt enter event\n");
    m_InWid = true;
    //setAttribute(Qt::WA_TranslucentBackground, false);
    //setAttribute(Qt::WA_NoSystemBackground, false);
    //m_pBlur is dynamically deleted
    /*if (m_pBlur != Q_NULLPTR)
    {
        m_pBlur->deleteLater();
        m_pBlur = Q_NULLPTR;
    }*/
    m_pBlur = new BlurPanel();
    this->setGraphicsEffect(m_pBlur);
    //update();
    m_pXbtn->setVisible(true);
    m_pSbtn->setVisible(true);
    m_pRbtn->setVisible(true);
    //grabMouse();
    QWidget::enterEvent(e);
}

void CentralWgt::leaveEvent(QEvent* e)
{
    printf("CentralWgt leave event\n");
    m_InWid = false;
    m_BtnTmr.start(CNTRL_SH_INT);
    m_CrsPt = m_SelPt = QPoint(0, 0);
    update();
    //releaseMouse();
    QWidget::leaveEvent(e);
}

void CentralWgt::mousePressEvent(QMouseEvent* e)
{
    //setCursor(Qt::OpenHandCursor);
    /*m_Pos = e->pos();
    m_WidMov = true;*/
    m_SelPt = e->pos();
    update();
    QWidget::mousePressEvent(e);
}

void CentralWgt::mouseMoveEvent(QMouseEvent* e)
{
    m_CrsPt = e->pos();
    update();
    //emit updateChildCursor(mapToParent(e->pos()));
    QWidget::mouseMoveEvent(e);
}

void CentralWgt::mouseReleaseEvent(QMouseEvent* e)
{
    //setCursor(Qt::ArrowCursor);
    m_WidMov = false;

    Q_UNUSED(e)
}

void CentralWgt::mouseDoubleClickEvent(QMouseEvent* e)
{
    Q_UNUSED(e)
}

/// <summary>
/// event to handle widget paint
/// </summary>
/// <param name="e">QPaintEvent</param>
void CentralWgt::paintEvent(QPaintEvent* e)
{
    //decide padding
    float wdth = (1 - BTN_SZ_PERC) * width() - X_OFFSET, hght = height();
    float wdth1 = wdth / 8.0f, hght1 = 7 * hght / 8.0f, wdth2 = wdth / 16.0f;
    float padL = wdth / 32.0f + BTN_SZ_PERC * width(), padR = wdth / 32.0f, padI = wdth / 100.0f;
    float padT = hght / 16.0f, padB = hght / 16.0f;

    //decide bounding box for clock digits
    QRectF rectM1(QPointF(padL, padT), QPointF(padL + wdth1, padT + hght1));//M
    QRectF rectM2(QPointF(rectM1.topRight().x() + padI, rectM1.topRight().y()), QPointF(rectM1.bottomRight().x() + wdth1 + padI, rectM1.bottomRight().y()));//MM
    QRectF rectC1(QPointF(rectM2.topRight().x() + padI, rectM2.topRight().y()), QPointF(rectM2.bottomRight().x() + wdth2 + padI, rectM2.bottomRight().y()));//MM:
    QRectF rectS1(QPointF(rectC1.topRight().x() + padI, rectC1.topRight().y()), QPointF(rectC1.bottomRight().x() + wdth1 + padI, rectC1.bottomRight().y()));//MM:S
    QRectF rectS2(QPointF(rectS1.topRight().x() + padI, rectS1.topRight().y()), QPointF(rectS1.bottomRight().x() + wdth1 + padI, rectS1.bottomRight().y()));//MM:SS
    QRectF rectC2(QPointF(rectS2.topRight().x() + padI, rectS2.topRight().y()), QPointF(rectS2.bottomRight().x() + wdth2 + padI, rectS2.bottomRight().y()));//MM:SS:
    QRectF rectSS1(QPointF(rectC2.topRight().x() + padI, rectC2.topRight().y()), QPointF(rectC2.bottomRight().x() + wdth1 + padI, rectC2.bottomRight().y()));//MM:SS:s
    QRectF rectSS2(QPointF(rectSS1.topRight().x() + padI, rectSS1.topRight().y()), QPointF(rectSS1.bottomRight().x() + wdth1 + padI, rectSS1.bottomRight().y()));//MM:SS:ss

    QPainter p(this);

    //pen setup for clock
    QPen pen(Qt::white);
    pen.setCapStyle(Qt::RoundCap);
    //pen.setWidth(10);
    p.setPen(pen);

    //font setup for clock
    QFont f = p.font();
    f.setPointSize(40);
    p.setFont(f);

    //draw text for clock
    if (rectM1.contains(m_CrsPt) && m_Edit)
    {
        //highlight M1
        pen.setColor(Qt::gray);
        p.setPen(pen);
        //draw M1
        p.drawText(rectM1, QString::number(m_M1));
        //reset pen
        pen.setColor(Qt::white);
        p.setPen(pen);
    }
    else
        p.drawText(rectM1, QString::number(m_M1));//draw M1

    if (rectM2.contains(m_CrsPt) && m_Edit)
    {
        //highlight M2
        pen.setColor(Qt::gray);
        p.setPen(pen);
        //draw M2
        p.drawText(rectM2, QString::number(m_M2));
        //reset pen
        pen.setColor(Qt::white);
        p.setPen(pen);
    }
    else
        p.drawText(rectM2, QString::number(m_M2));//draw M2

    p.drawText(rectC1, ":");//draw colon 1

    if (rectS1.contains(m_CrsPt) && m_Edit)
    {
        //highlight S1
        pen.setColor(Qt::gray);
        p.setPen(pen);
        //draw S1
        p.drawText(rectS1, QString::number(m_S1));
        //reset pen
        pen.setColor(Qt::white);
        p.setPen(pen);
    }
    else
        p.drawText(rectS1, QString::number(m_S1));//draw S1

    if (rectS2.contains(m_CrsPt) && m_Edit)
    {
        //highlight S2
        pen.setColor(Qt::gray);
        p.setPen(pen);
        //draw S2
        p.drawText(rectS2, QString::number(m_S2));
        //reset pen
        pen.setColor(Qt::white);
        p.setPen(pen);
    }
    else
        p.drawText(rectS2, QString::number(m_S2));

    p.drawText(rectC2, ":");//draw colon 2

    if (rectSS1.contains(m_CrsPt) && m_Edit)
    {
        //highlight SS1
        pen.setColor(Qt::gray);
        p.setPen(pen);
        //draw SS1
        p.drawText(rectSS1, QString::number(m_SS1));
        //reset pen
        pen.setColor(Qt::white);
        p.setPen(pen);
    }
    else
        p.drawText(rectSS1, QString::number(m_SS1));//draw SS1

    if (rectSS2.contains(m_CrsPt) && m_Edit)
    {
        //highlight SS2
        pen.setColor(Qt::gray);
        p.setPen(pen);
        //draw SS2
        p.drawText(rectSS2, QString::number(m_SS2));
        //reset pen
        pen.setColor(Qt::white);
        p.setPen(pen);
    }
    else
        p.drawText(rectSS2, QString::number(m_SS2));//draw SS2

    //draw bounding box based on hover
    pen.setColor(Qt::black);
    p.setPen(pen);

    if (m_Edit)
    {
        if (rectM1.contains(m_SelPt))
            //draw selection for M1
            p.drawRect(rectM1);
        else
            if (rectM2.contains(m_SelPt))
                //draw selection for M2
                p.drawRect(rectM2);
            else
                if (rectS1.contains(m_SelPt))
                    //draw selection for S1
                    p.drawRect(rectS1);
                else
                    if (rectS2.contains(m_SelPt))
                        //draw selection for S2
                        p.drawRect(rectS2);
                    else
                        if (rectSS1.contains(m_SelPt))
                            //draw selection for SS1
                            p.drawRect(rectSS1);
                        else
                            if (rectSS2.contains(m_SelPt))
                                //draw selection for SS2
                                p.drawRect(rectSS2);
    }
    QWidget::paintEvent(e);
}

void CentralWgt::setupUi()
{
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->resize(QSize(m_ScrnSz.width() * WGT_WDTH_PERC, m_ScrnSz.height() * WGT_HGHT_PERC));

    float btnSize = BTN_SZ_PERC * width();

    printf("wdgt size %d\n", width());
    printf("button size %f\n", btnSize);

	/*close button*/
	m_pXbtn = new QPushButton(this);
	m_pXbtn->setObjectName(QString::fromUtf8("xBtn"));
	sizePolicy.setHeightForWidth(m_pXbtn->sizePolicy().hasHeightForWidth());
	sizePolicy.setRetainSizeWhenHidden(true);
	m_pXbtn->setSizePolicy(sizePolicy);
	m_pXbtn->setMinimumSize(QSize(btnSize, btnSize));
	m_pXbtn->setMaximumSize(QSize(btnSize, btnSize));
	m_pXbtn->setVisible(false);
	m_pXbtn->setStyleSheet(QString::fromUtf8("QPushButton#xBtn{\n"
		"border: none;\n"
		"}\n"
		"QPushButton:hover:pressed#xBtn{\n"
		"border-width: 2px;\n"
		"border-color: rgba(255,0,0,125);\n"
		"border-style: solid;\n"
		"border-radius: 10px;\n"
		"background-color: rgba(255,0,0,125);\n"
		"}\n"
		"QPushButton:hover:!pressed#xBtn{\n"
		"border-width: 1px;\n"
		"border-color: rgba(255,0,0,75);\n"
		"border-style: solid;\n"
		"border-radius: 10px;\n"
		"background-color: rgba(255,0,0,75);\n"
		"}"));
	QPixmap xPix(":/ScreenTimer/res/close.png");
	xPix.scaled(m_pXbtn->size());
    m_pXbtn->setIcon(QIcon(xPix));

	/*start button*/
    m_pSbtn = new QPushButton(this);
    m_pSbtn->setObjectName(QString::fromUtf8("sBtn"));
	sizePolicy.setHeightForWidth(m_pSbtn->sizePolicy().hasHeightForWidth());
	sizePolicy.setRetainSizeWhenHidden(true);
	m_pSbtn->setSizePolicy(sizePolicy);
	m_pSbtn->setMinimumSize(QSize(btnSize, btnSize));
	m_pSbtn->setMaximumSize(QSize(btnSize, btnSize));
	m_pSbtn->setVisible(false);
	m_pSbtn->setStyleSheet(QString::fromUtf8("QPushButton#sBtn{\n"
		"border:none\n"
		"}\n"
		"QPushButton:hover:pressed#sBtn{\n"
		"border-width: 2px;\n"
		"border-color: rgba(0,255,0,125);\n"
		"border-style: solid;\n"
		"border-radius: 10px;\n"
		"background-color: rgba(0,255,0,125);\n"
		"}\n"
		"QPushButton:hover:!pressed#sBtn{\n"
		"border-width: 1px;\n"
		"border-color: rgba(0,255,0,75);\n"
		"border-style: solid;\n"
		"border-radius: 10px;\n"
		"background-color: rgba(0,255,0,75);\n"
		"}"));
	QPixmap sPix(":/ScreenTimer/res/play.png");
	sPix.scaled(m_pSbtn->size());//QSize(40,36));
    m_pSbtn->setIcon(QIcon(sPix));

	/*reset button*/
    m_pRbtn = new QPushButton(this);
    m_pRbtn->setObjectName(QString::fromUtf8("rBtn"));
	sizePolicy.setHeightForWidth(m_pRbtn->sizePolicy().hasHeightForWidth());
	sizePolicy.setRetainSizeWhenHidden(true);
	m_pRbtn->setSizePolicy(sizePolicy);
	m_pRbtn->setMinimumSize(QSize(btnSize, btnSize));
	m_pRbtn->setMaximumSize(QSize(btnSize, btnSize));
	m_pRbtn->setVisible(false);
	m_pRbtn->setStyleSheet(QString::fromUtf8("QPushButton#rBtn{\n"
		"border:none;\n"
		"}\n"
		"QPushButton:hover:pressed#rBtn{\n"
		"border-width: 2px;\n"
		"border-color: rgba(0,0,255,125);\n"
		"border-style: solid;\n"
		"border-radius: 10px;\n"
		"background-color: rgba(0,0,255,125);\n"
		"}\n"
		"QPushButton:hover:!pressed#rBtn{\n"
		"border-width: 1px;\n"
		"border-color:  rgba(0,0,255,75);\n"
		"border-style: solid;\n"
		"border-radius: 10px;\n"
		"background-color: rgba(0,0,255,75);\n"
		"}"));
	QPixmap rPix(":/ScreenTimer/res/retry.png");
	rPix.scaled(m_pRbtn->size());
    m_pRbtn->setIcon(QIcon(rPix));

    m_pSpacer = new QSpacerItem(width() - btnSize - X_OFFSET, height(), QSizePolicy::Fixed, QSizePolicy::Fixed);

	m_pVerticalLayout = new QVBoxLayout();
	m_pVerticalLayout->setSpacing(0);
	m_pVerticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

	m_pVerticalLayout->addWidget(m_pXbtn, 0, Qt::AlignHCenter);
	m_pVerticalLayout->addWidget(m_pSbtn, 0, Qt::AlignHCenter);
	m_pVerticalLayout->addWidget(m_pRbtn, 0, Qt::AlignHCenter);

	m_pHorizontalLayout = new QHBoxLayout(this);
	m_pHorizontalLayout->setSpacing(0);
	m_pHorizontalLayout->setContentsMargins(11, 11, 11, 11);
	m_pHorizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	m_pHorizontalLayout->setContentsMargins(0, 0, 0, 0);

    m_pHorizontalLayout->addLayout(m_pVerticalLayout);
    m_pHorizontalLayout->addItem(m_pSpacer);

	retranslateUi();

	//QMetaObject::connectSlotsByName(this);
} // setupUi

void CentralWgt::retranslateUi()
{
    this->setWindowTitle(QString());
    m_pXbtn->setText(QString());
    m_pSbtn->setText(QString());
    m_pRbtn->setText(QString());
} // retranslateUi

void CentralWgt::sClick()
{
    if (m_Timer.isActive())
    {
        m_Timer.stop();
        m_Run = false;
    }
    else
    {
        m_Timer.start();
        m_Run = true;
    }
    m_Edit = false;
    update();
}

void CentralWgt::rClick()
{
    if (!m_Timer.isActive())
		m_Edit = true;
    m_M1 = m_M2 = m_S1 = m_S2 = m_SS1 = m_SS2 = 0;
    update();
}

void CentralWgt::xClick()
{
    if(m_Timer.isActive())
		m_Timer.stop();
    close();
    exit(0);
}

void CentralWgt::cntrlOff()
{
    if (!m_InWid)
    {
        m_pXbtn->setVisible(false);
        m_pSbtn->setVisible(false);
        m_pRbtn->setVisible(false);
        this->setGraphicsEffect(m_pNoBlur);
        update();
    }
}

void CentralWgt::timerTick()
{
    m_SS2++;
    if (m_SS2 > 9)
    {
        m_SS2 = 0;
        m_SS1++;
        if (m_SS1 > 9)
        {
            m_SS1 = 0;
            m_S2++;
			if (m_S2 > 9)//|| (m_S1 * 10 + m_S2)
            {
                m_S2 = 0;
                m_S1++;
                if (m_S1 > 5)
                {
                    m_S1 = 0;
                    m_M2++;
                    if (m_M2 > 9)
                    {
                        m_M2 = 0;
                        m_M1++;
                        if (m_M1 > 5)
                            m_M1 = 0;
                    }
                }
            }
        }
    }
    update();
}

//m_pBlur is dynamically deleted
/*if (m_pBlur != Q_NULLPTR)
{
    m_pBlur->deleteLater();
    m_pBlur = Q_NULLPTR;
}*/
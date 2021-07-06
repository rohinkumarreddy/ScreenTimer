#pragma once

#include <QWidget>
#include <QTimer>

//forward declaration
class BlurPanel;
class QGraphicsEffect;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QSpacerItem;

class CentralWgt : public QWidget
{
	Q_OBJECT

public:
    CentralWgt(QSize scrnSz, QWidget* parent = Q_NULLPTR);
    inline void setScreenSize(const QSize sz) { m_ScrnSz = sz; }
protected:
	void mouseMoveEvent(QMouseEvent* e);
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void paintEvent(QPaintEvent* e);
private:
    QHBoxLayout* m_pHorizontalLayout;
    QVBoxLayout* m_pVerticalLayout;
    QPushButton* m_pXbtn;
    QPushButton* m_pSbtn;
    QPushButton* m_pRbtn;
    QSpacerItem* m_pSpacer;
    BlurPanel* m_pBlur;
    QGraphicsEffect* m_pNoBlur;
    QTimer m_Timer;
    QTimer m_BtnTmr;
    QPoint m_Pos;
    QPoint m_CrsPt;//current cursor position
    QPoint m_SelPt;//current selected cursor position
    QSize  m_ScrnSz;//current screen size
    int  m_M1;
    int  m_M2;
    int  m_S1;
    int  m_S2;
    int  m_SS1;
    int  m_SS2;
    bool m_InWid;
    bool m_WidMov;
    bool m_Run;
    bool m_Edit;
    void setupUi();
    void retranslateUi();
private slots:
    void sClick();
    void rClick();
    void xClick();
    void cntrlOff();
    void timerTick();
signals:
	void updateChildCursor(QPoint& pt);
};
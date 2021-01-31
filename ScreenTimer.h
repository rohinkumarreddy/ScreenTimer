#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ScreenTimer.h"
#include <QTimer>

//forward declaration
class BlurPanel;
class QGraphicsEffect;

class ScreenTimer : public QMainWindow
{
    Q_OBJECT

public:
    ScreenTimer(QWidget *parent = Q_NULLPTR);
    ~ScreenTimer();
protected:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);

private:
    Ui::ScreenTimerClass ui;
    BlurPanel* m_pBlur;
    QGraphicsEffect* m_pNoBlur;
    QTimer m_Timer;
    QTimer m_BtnTmr;
    QPoint m_Pos;
    bool m_InWid;
    bool m_WidMov;
private slots:
    void sClick();
    void rClick();
    void xClick();
    void cntrlOff();
};

#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>

//forward declaration
class CentralWgt;

class ScreenTimer : public QMainWindow
{
    Q_OBJECT

public:
    ScreenTimer(QWidget *parent = Q_NULLPTR);
    ~ScreenTimer();
private:
    CentralWgt* centralWidget;
private:
    void setupUi(QSize scrnSz);
};

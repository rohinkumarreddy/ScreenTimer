#include "ScreenTimer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenTimer w;
    w.show();
    return a.exec();
}

#include "armaxmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ARMAXmainWindow w;
    w.show();

    return a.exec();
}

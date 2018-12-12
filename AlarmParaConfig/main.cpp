#include "configdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigDialog w;
    w.show();

    return a.exec();
}

#include "DarkCode.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DarkCode w;
    w.show();
    return a.exec();
}

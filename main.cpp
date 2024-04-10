#include "devide.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DevIde w;
    w.show();
    return a.exec();
}

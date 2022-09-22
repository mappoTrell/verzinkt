#include "verzinkt.h"

#include <QApplication>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    verzinkt w;
    w.show();
    return a.exec();
}

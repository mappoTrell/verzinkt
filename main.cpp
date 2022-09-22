#include "verzinkt.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(icons);
    QApplication a(argc, argv);
    verzinkt w;
    w.show();
    return a.exec();
}

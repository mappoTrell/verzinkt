#include "verzinkt.h"

#include <QApplication>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QFile file(":/dark/stylesheet.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    a.setStyleSheet(stream.readAll());

    verzinkt w;
    w.show();
    return a.exec();
}

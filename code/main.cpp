#include "fenprinc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    fenPrinc w;
    w.show();
    QFile file(":/stylesheets/main.qss");
    QString tempS = "";
    if(file.open(QFile::ReadOnly))
    {tempS = QLatin1String(file.readAll());}
    else{
        return 0;
    }

    a.setStyleSheet(tempS);
    QPalette pal = a.palette();
    pal.setColor(QPalette::Window,"#fcfcd4");
    a.setPalette(pal);

    return a.exec();
}

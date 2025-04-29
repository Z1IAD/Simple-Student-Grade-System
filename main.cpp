#include"mainwindow.h"
#include<QApplication>
#include<QFont>

int main(int argc,char*argv[])
{
    QApplication a(argc,argv);
    a.setLayoutDirection(Qt::RightToLeft);
    QFont font("Arial",10);
    a.setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}

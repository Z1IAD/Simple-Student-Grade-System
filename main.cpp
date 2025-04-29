#include "mainwindow.h"

#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // تعيين الاتجاه من اليمين إلى اليسار للغة العربية
    a.setLayoutDirection(Qt::RightToLeft);

    // تعيين خط مناسب للعربية
    QFont font("Arial", 10);
    a.setFont(font);

    MainWindow w;
    w.show();
    return a.exec();
}

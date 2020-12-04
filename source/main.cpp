#include <QApplication>
#include <QSettings>

#include <QDebug>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Edgeflow");
    QCoreApplication::setOrganizationDomain("edgeflow.net");
    QCoreApplication::setApplicationName("QAddApp");

    QApplication app(argc, argv);
    /* app.setWindowIcon(QIcon(":/images/aqueduct_logo_32.png")); */

    MainWindow main;
    main.show();

    return app.exec();
}

#include <QApplication>
#include <QSettings>

#include <QDebug>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    // enable visibility of Debugmessages on Linux 
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);

    QCoreApplication::setOrganizationName("Edgeflow");
    QCoreApplication::setOrganizationDomain("edgeflow.net");
    QCoreApplication::setApplicationName("QAddApp");

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/qaddapp_icon_64.png"));

    MainWindow main;
    main.show();

    return app.exec();
}

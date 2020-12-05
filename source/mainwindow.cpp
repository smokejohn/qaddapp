#include "mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtWidgets>

MainWindow::MainWindow() : QMainWindow() {
    // initialize paths
    binaryPath = new QDir("/");
    iconPath = new QDir("/");
    destPath = new QDir(QDir::homePath() + QDir::separator() + ".local");
    linkPath = new QDir("/");

    qDebug() << binaryPath->absolutePath();
    qDebug() << iconPath->absolutePath();
    qDebug() << destPath->absolutePath();
    qDebug() << linkPath->absolutePath();

    resize(650, 400);

    // populate the menu bar
    createActions();
    createInputs();

    statusBar()->showMessage("Ready");
}

void MainWindow::createActions() {
    // creating file menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *aboutMenu = menuBar()->addMenu("&About");

    // creating quit menuitem
    QAction *quitAct = new QAction("&Quit", this);
    quitAct->setShortcut(QKeySequence::Quit);
    quitAct->setStatusTip("Quit QAddApp");

    // creating about menuitem
    QAction *aboutAct = new QAction("&About", this);
    aboutAct->setStatusTip("About QAddApp");

    // connecting signal and slot
    connect(quitAct, &QAction::triggered, this, &QApplication::exit);

    // adding quit menuitem to menubar
    fileMenu->addAction(quitAct);
    aboutMenu->addAction(aboutAct);
}

void MainWindow::createInputs() {
    // Mainwindow Central Widget
    QWidget *inputWidget = new QWidget;

    // Mainlayout Column
    QVBoxLayout *vboxMain = new QVBoxLayout(inputWidget);

    // Row Binary: Label, Textinput, Browsebutton
    QHBoxLayout *hboxBinary = new QHBoxLayout;
    QLabel *lblBinary = new QLabel("Binary/AppImage:");

    leBinary = new QLineEdit;
    QPushButton *btnBrowseBinary = new QPushButton("...");
    hboxBinary->addWidget(leBinary);
    hboxBinary->addWidget(btnBrowseBinary);

    // Row Icon: Label, Textinput, Browsebutton
    QHBoxLayout *hboxIcon = new QHBoxLayout;
    QLabel *lblIcon = new QLabel("Application Icon:");

    leIcon = new QLineEdit;
    QPushButton *btnBrowseIcon = new QPushButton("...");
    hboxIcon->addWidget(leIcon);
    hboxIcon->addWidget(btnBrowseIcon);

    // Row Destination: Textinput, Browsebutton
    QHBoxLayout *hboxDest = new QHBoxLayout;
    QLabel *lblDest = new QLabel("Destination:");

    leDest = new QLineEdit(destPath->absolutePath());
    QPushButton *btnBrowseDest = new QPushButton("...");
    hboxDest->addWidget(leDest);
    hboxDest->addWidget(btnBrowseDest);

    // Row Category
    QHBoxLayout *hboxCat = new QHBoxLayout;
    QLabel *lblCat = new QLabel("Application Category:");
    QComboBox *cbCat = new QComboBox;
    QList<QString> *cbItems = new QList<QString>{
        "AudioVideo", "Audio",    "Video",   "Development", "Education",
        "Game",       "Graphics", "Network", "Office",      "Science",
        "Settings",   "System",   "Utility"};
    cbCat->addItems(*cbItems);
    cbCat->setCurrentIndex(cbItems->length() - 1);

    hboxCat->addWidget(lblCat);
    hboxCat->addWidget(cbCat);

    // Row Go Button
    QHBoxLayout *hboxGo = new QHBoxLayout;
    QPushButton *btnGo = new QPushButton("Go");

    hboxGo->addStretch(2);
    hboxGo->addWidget(btnGo);

    vboxMain->addWidget(lblBinary);
    vboxMain->addLayout(hboxBinary);
    vboxMain->addWidget(lblIcon);
    vboxMain->addLayout(hboxIcon);
    vboxMain->addWidget(lblDest);
    vboxMain->addLayout(hboxDest);
    vboxMain->addLayout(hboxCat);
    vboxMain->addLayout(hboxGo);
    vboxMain->addStretch(0);

    connect(btnBrowseBinary, &QPushButton::clicked, this,
            &MainWindow::getBinaryPath);
    connect(btnBrowseIcon, &QPushButton::clicked, this,
            &MainWindow::getIconPath);
    connect(btnBrowseDest, &QPushButton::clicked, this,
            &MainWindow::getDestPath);

    connect(btnGo, &QPushButton::clicked, this, &MainWindow::addApp);

    setCentralWidget(inputWidget);
}

void MainWindow::getBinaryPath() {
    QString path = QFileDialog::getOpenFileName(this, "Select Binary/AppImage",
                                                QDir::homePath());
    if (path.isNull() == false) {
        binaryPath->setPath(path);
        leBinary->setText(path);
    }
}

void MainWindow::getIconPath() {
    QString path =
        QFileDialog::getOpenFileName(this, "Select Icon file", QDir::homePath(),
                                     "Image Files (*.png *.jpg *.bmp *.svg)");
    if (path.isNull() == false) {
        iconPath->setPath(path);
        leIcon->setText(path);
    }
}

void MainWindow::getDestPath() {
    QString path = QFileDialog::getExistingDirectory(
        this, "Select Destination path", QDir::homePath());
    if (path.isNull() == false) {
        destPath->setPath(path);
        leDest->setText(path);
    }
}

void MainWindow::addApp() {
    qDebug() << binaryPath->absolutePath();
    if (binaryPath->absolutePath() == "/" |
        iconPath->absolutePath() == "/" |
        linkPath->absolutePath() == "/") {
        return;
    }

    // Create a folder for the app
    QString appname = binaryPath->dirName();
    qDebug() << appname;

    // Create Desktop file
    QFile desktopFile(QDir::homePath() + "/.local/share/applications/" +
                      appname.toLower() + ".desktop");

    if (!desktopFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Could not open file for writing.";
        return;
    }

    QTextStream out(&desktopFile);
    out << "[Desktop Entry]\n"
        << "Type=Application\n"
        << "Name=" + appname + "\n"
        << "Exec=" + binaryPath->absolutePath() + "\n"
        << "Icon=" + iconPath->absolutePath() + "\n"
        << "Categories=Utility;\n";

    desktopFile.flush();
    desktopFile.close();
}

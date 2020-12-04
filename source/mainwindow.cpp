#include <QtWidgets>
#include <iostream>

#include "mainwindow.h"

MainWindow::MainWindow() :
    QMainWindow()
{
    resize(650, 400);

    destPath = QDir::homePath() + QDir::separator() + ".local";


    // populate the menu bar
    createActions();
    createInputs();

    statusBar()->showMessage("Ready");
}

void MainWindow::createActions()
{
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

void MainWindow::createInputs(){
    
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
    QLabel *lblIcon = new QLabel("Icon:");

    leIcon = new QLineEdit;
    QPushButton *btnBrowseIcon = new QPushButton("...");
    hboxIcon->addWidget(leIcon);
    hboxIcon->addWidget(btnBrowseIcon);

    // Row Destination: Textinput, Browsebutton
    QHBoxLayout *hboxDest = new QHBoxLayout;
    QLabel *lblDest = new QLabel("Destination:");

    leDest = new QLineEdit(destPath);
    QPushButton *btnBrowseDest = new QPushButton("...");
    hboxDest->addWidget(leDest);
    hboxDest->addWidget(btnBrowseDest);


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
    vboxMain->addLayout(hboxGo);
    vboxMain->addStretch(0);

    connect(btnBrowseBinary, &QPushButton::clicked, this, &MainWindow::getBinaryPath);
    connect(btnBrowseIcon, &QPushButton::clicked, this, &MainWindow::getIconPath);
    setCentralWidget(inputWidget);
}

void MainWindow::getBinaryPath(){
    QString path = QFileDialog::getOpenFileName(this, "Select Binary/AppImage", QDir::homePath());
    if (path.isNull() == false)
    {
        binaryPath = path;
        leBinary->setText(path);
        std::cout << binaryPath.toStdString();
    }
}

void MainWindow::getIconPath(){
    QString path = QFileDialog::getOpenFileName(this, "Select Icon file", QDir::homePath());
    if (path.isNull() == false)
    {
        iconPath = path;
        leIcon->setText(path);
        std::cout << iconPath.toStdString();
    }
}

void MainWindow::getDestPath(){
    QString path = QFileDialog::getExistingDirectory(this, "Select Destination path", QDir::homePath());
    if (path.isNull() == false)
    {
        destPath = path;
        leDest->setText(path);
        std::cout << destPath.toStdString();
    }
}

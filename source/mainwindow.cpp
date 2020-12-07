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

    resize(850, 400);

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

    // connecting signal and slot
    connect(quitAct, &QAction::triggered, this, &QApplication::exit);

    // creating about menuitem
    QAction *aboutAct = new QAction("&About", this);
    aboutAct->setStatusTip("About QAddApp");

    // adding quit menuitem to menubar
    fileMenu->addAction(quitAct);
    aboutMenu->addAction(aboutAct);
}

void MainWindow::createInputs() {
    // Mainwindow Central Widget
    QWidget *inputWidget = new QWidget;

    // Mainlayout Column
    QVBoxLayout *vboxMain = new QVBoxLayout(inputWidget);
    vboxMain->setMargin(15);

    // Row Binary: Label, Textinput, Browsebutton
    piBinary = new PathInput("Binary/AppImage:", true, false,
                             "Select AppImage or Binary");

    // Row Icon: Label, Textinput, Browsebutton
    piIcon = new PathInput("Icon File:", true, false, "Select Icon file", "",
                           QDir::homePath(),
                           "Image Files (*.png *.jpg *.bmp *.svg)");

    // Group Relocate
    QVBoxLayout *vboxRelo = new QVBoxLayout;
    vboxRelo->setContentsMargins(0, 10, 0, 10);
    gbxRelo = new QGroupBox("Package and relocate AppImage/Binary?");
    gbxRelo->setCheckable(true);
    gbxRelo->setChecked(false);
    gbxRelo->setStyleSheet(
        "QGroupBox { "
        "border: 1px solid #555555;"
        "border-radius: 5px; "
        "margin-top: 1ex;"
        "padding: 10px;"
        "}"
        "QGroupBox::title {"
        "subcontrol-origin: margin; }");

    piDest =
        new PathInput("Destination:", true, true, "Select destination folder",
                      QDir::homePath() + "/.local");
    ckbBinFolder = new QCheckBox("Move Folder containing binary?");
    piBinFolder = new PathInput("Containing Folder:", true, true,
                                "Select folder containing the binary");

    piBinFolder->setEnabled(false);

    vboxRelo->addWidget(piDest);
    vboxRelo->addWidget(ckbBinFolder);
    vboxRelo->addWidget(piBinFolder);

    gbxRelo->setLayout(vboxRelo);

    // Row Category
    QHBoxLayout *hboxCat = new QHBoxLayout;
    QLabel *lblCat = new QLabel("Application Category:");
    cbCategory = new QComboBox;
    QList<QString> *cbItems = new QList<QString>{
        "AudioVideo", "Audio",    "Video",   "Development", "Education",
        "Game",       "Graphics", "Network", "Office",      "Science",
        "Settings",   "System",   "Utility"};
    cbCategory->addItems(*cbItems);
    cbCategory->setCurrentIndex(cbItems->length() - 1);

    hboxCat->addWidget(lblCat);
    hboxCat->addWidget(cbCategory);

    // Row Go Button
    QHBoxLayout *hboxGo = new QHBoxLayout;
    QPushButton *btnGo = new QPushButton("Go");

    hboxGo->addStretch(2);
    hboxGo->addWidget(btnGo);

    vboxMain->addWidget(piBinary);
    vboxMain->addWidget(piIcon);

    vboxMain->addWidget(gbxRelo);

    vboxMain->addLayout(hboxCat);
    vboxMain->addLayout(hboxGo);
    vboxMain->addStretch(0);

    connect(ckbBinFolder, &QCheckBox::stateChanged, this,
            &MainWindow::enableBinFolder);
    connect(btnGo, &QPushButton::clicked, this, &MainWindow::addApp);
    connect(piBinary, &PathInput::pathChanged, this,
            &MainWindow::updateBinFolder);

    setCentralWidget(inputWidget);
}

void MainWindow::updateBinFolder(const QString &path) {
    QFileInfo pathInfo(path);
    QString dirPath = pathInfo.absolutePath();
    piBinFolder->setPath(dirPath);
    piBinFolder->isDirty = false;
}

void MainWindow::enableBinFolder(int state) {
    if (state == 2)
        piBinFolder->setEnabled(true);
    else
        piBinFolder->setEnabled(false);
}

void MainWindow::addApp() {
    if (this->piBinary->isDirty) {
        qDebug() << "AppImage/Binary path is wrong or not set";
        return;
    }
    if (this->piIcon->isDirty) {
        qDebug() << "Icon path is wrong or not set";
        return;
    }
    if (this->gbxRelo->isChecked()) {
        if (this->piDest->isDirty) {
            qDebug() << "Destination path is wrong or not set";
            return;
        }
        if (this->ckbBinFolder->isChecked() && this->piBinFolder->isDirty) {
            qDebug() << "Binary folder path is wrong or not set";
            return;
        }
    }

    // Create a folder for the app
    QString appname = this->piBinary->getPath()->dirName();
    qDebug() << appname;

    writeDesktopFile();
}

void MainWindow::writeDesktopFile() {
    qDebug() << "writing .desktop file";

    QString appname = this->piBinary->getPath()->dirName();
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
        << "Exec=" + this->piBinary->getPath()->absolutePath() + "\n"
        << "Icon=" + this->piIcon->getPath()->absolutePath() + "\n"
        << "Categories=" + this->cbCategory->currentText() + ";\n";

    desktopFile.flush();
    desktopFile.close();
}

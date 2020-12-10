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
    binDirPath = new QDir("/");
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
    ckbBinDir = new QCheckBox("Move Folder containing binary?");
    piBinDir = new PathInput("Containing Folder:", true, true,
                             "Select folder containing the binary");

    piBinDir->setEnabled(false);

    vboxRelo->addWidget(piDest);
    vboxRelo->addWidget(ckbBinDir);
    vboxRelo->addWidget(piBinDir);

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

    connect(ckbBinDir, &QCheckBox::stateChanged, this,
            &MainWindow::enableBinDir);
    connect(btnGo, &QPushButton::clicked, this, &MainWindow::addApp);
    connect(piBinary, &PathInput::pathChanged, this, &MainWindow::updateBinDir);

    setCentralWidget(inputWidget);
}

void MainWindow::updateBinDir(const QString &path) {
    QFileInfo pathInfo(path);
    QString dirPath = pathInfo.absolutePath();
    piBinDir->setPath(dirPath);
    piBinDir->isDirty = false;
}

void MainWindow::enableBinDir(int state) {
    if (state == 2)
        piBinDir->setEnabled(true);
    else
        piBinDir->setEnabled(false);
}

void MainWindow::addApp() {
    if (!isFormValid()) return;

    this->binaryPath = this->piBinary->getPath();
    this->iconPath = this->piIcon->getPath();

    if (gbxRelo->isChecked()) {
        if (!ckbBinDir->isChecked()) {
            this->destPath = this->piDest->getPath();
            // Create a folder for the app
            QString appname = this->piBinary->getPath()->dirName();
            QDir basePath(this->destPath->absolutePath() + QDir::separator() +
                          appname);
            basePath.mkpath(basePath.absolutePath());

            // move the binary and icon
            QFileInfo binaryFI(this->binaryPath->absolutePath());
            QFileInfo iconFI(this->iconPath->absolutePath());
            QString newBinLoc = basePath.absolutePath() + QDir::separator() +
                                binaryFI.fileName();
            QString newIconLoc =
                basePath.absolutePath() + QDir::separator() + iconFI.fileName();

            // remove existing files
            if (QFile::exists(newBinLoc)) QFile::remove(newBinLoc);
            if (QFile::exists(newIconLoc)) QFile::remove(newIconLoc);

            // move the files
            this->binaryPath->rename(this->binaryPath->absolutePath(),
                                     newBinLoc);
            this->iconPath->rename(this->iconPath->absolutePath(), newIconLoc);

            // point QDirs at new location
            this->binaryPath->setPath(newBinLoc);
            this->iconPath->setPath(newIconLoc);
        } else {

            this->destPath = this->piDest->getPath();
            this->binDirPath = this->piBinDir->getPath();

            // get relative path of binary to binDir
            QString relBinToDir = this->binDirPath->relativeFilePath(this->binaryPath->absolutePath());

            QString newBinDirLoc = destPath->absolutePath() + QDir::separator() + binDirPath->dirName();
            
            // move the folder
            this->binDirPath->rename(this->binDirPath->absolutePath(), newBinDirLoc);

            // check if icon is in the binDir
            if (QString(iconPath->absolutePath()).contains(binDirPath->absolutePath())){
                // update the iconPath to reflect the new binDir location
                QString relIconToDir = this->binDirPath->relativeFilePath(this->iconPath->absolutePath());
                this->iconPath->setPath(newBinDirLoc + QDir::separator() + relIconToDir);
            } else { 
                // move the icon file to the new binDir and update the iconPath
                QFileInfo iconFI(this->iconPath->absolutePath());
                QString newIconLoc = newBinDirLoc + QDir::separator() + iconFI.fileName();
                this->iconPath->rename(this->iconPath->absolutePath(), newIconLoc);
                this->iconPath->setPath(newIconLoc);
            }

            this->binaryPath->setPath(newBinDirLoc + QDir::separator() + relBinToDir);

        }
    }

    writeDesktopFile();
}

void MainWindow::writeDesktopFile() {

    QString appname = this->binaryPath->dirName();
    // Create Desktop file
    QFile desktopFile(QDir::homePath() + "/.local/share/applications/" +
                      appname.toLower() + ".desktop");

    if (!desktopFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Could not open file for writing.";
        return;
    }

    QString category = this->cbCategory->currentText();

    if (category == "Audio" | category == "Video")
        category = "AudioVideo;" + this->cbCategory->currentText();

    QTextStream out(&desktopFile);
    out << "[Desktop Entry]\n"
        << "Type=Application\n"
        << "Name=" + appname + "\n"
        << "Exec=" + this->binaryPath->absolutePath() + "\n"
        << "Icon=" + this->iconPath->absolutePath() + "\n"
        << "Categories=" + category + ";\n";

    desktopFile.flush();
    desktopFile.close();
}

bool MainWindow::isFormValid() {
    if (this->piBinary->isDirty) {
        qDebug() << "AppImage/Binary path is wrong or not set";
        QMessageBox msgBox;
        msgBox.setText(
            "The AppImage/Binary file path has not been set or the file "
            "doesn't exist");
        msgBox.exec();
        return false;
    }
    if (this->piIcon->isDirty) {
        qDebug() << "Icon path is wrong or not set";
        QMessageBox msgBox;
        msgBox.setText(
            "The icon file path has not been set or the file doesn't exist");
        msgBox.exec();
        return false;
    }
    if (this->gbxRelo->isChecked()) {
        if (this->piDest->isDirty) {
            qDebug() << "Destination path is wrong or not set";
            QMessageBox msgBox;
            msgBox.setText(
                "The destination folder path has not been set or doesn't "
                "exist");
            msgBox.exec();
            return false;
        }
        if (this->ckbBinDir->isChecked() && this->piBinDir->isDirty) {
            qDebug() << "Binary folder path is wrong or not set";
            QMessageBox msgBox;
            msgBox.setText(
                "The binary folder path has not been set or doesn't exist");
            msgBox.exec();
            return false;
        }
    }

    return true;
}

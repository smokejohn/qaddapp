#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pathinput.h"
#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow {
    Q_OBJECT;

   public:
    MainWindow();

   public slots:
    void enableBinDir(int state);
    void addApp();
    void updateBinDir(const QString &path);


   private:
    QDir *binaryPath;
    QDir *iconPath;
    QDir *destPath;
    QDir *binDirPath;
    QDir *linkPath;

    QComboBox *cbCategory;

    QGroupBox *gbxRelo;
    QCheckBox *ckbBinDir;
    PathInput *piBinary;
    PathInput *piIcon;
    PathInput *piDest;
    PathInput *piBinDir;

    void createActions();
    void createInputs();

    void writeDesktopFile();
    bool isFormValid();
};

#endif  // MAINWINDOW_H

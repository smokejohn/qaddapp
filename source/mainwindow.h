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
    void enableBinFolder(int state);
    void addApp();
    void updateBinFolder(const QString &path);


   private:
    QDir *binaryPath;
    QDir *iconPath;
    QDir *destPath;
    QDir *linkPath;

    QComboBox *cbCategory;

    QGroupBox *gbxRelo;
    QCheckBox *ckbBinFolder;
    PathInput *piBinary;
    PathInput *piIcon;
    PathInput *piDest;
    PathInput *piBinFolder;

    void createActions();
    void createInputs();

    void writeDesktopFile();
};

#endif  // MAINWINDOW_H

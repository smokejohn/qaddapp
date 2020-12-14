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
    void enableLinkDir(int state);
    void addApp();
    void updateBinDir(const QString &path);
    void updateAppName(const QString &path);
    void updateLinkPath(const QString &path);
    void showAbout();

   private:
    QDir *binaryPath;
    QDir *iconPath;
    QDir *destPath;
    QDir *binDirPath;
    QDir *linkPath;

    QComboBox *cbCategory;

    PathInput *piBinary;
    PathInput *piIcon;
    QGroupBox *gbxRelo;
    QCheckBox *ckbBinDir;
    QCheckBox *ckbLinkDir;
    PathInput *piDest;
    PathInput *piBinDir;
    PathInput *piLinkDir;

    QLineEdit *leName;

    void createActions();
    void createInputs();

    void writeDesktopFile();
    void createSymlink();
    bool isFormValid();
};

#endif  // MAINWINDOW_H

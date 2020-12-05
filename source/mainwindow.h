#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow {
    Q_OBJECT;

   public:
    MainWindow();

   public slots:
    void getBinaryPath();
    void getIconPath();
    void getDestPath();
    void addApp();

   private:
    QDir *binaryPath;
    QDir *iconPath;
    QDir *destPath;
    QDir *linkPath;

    QLineEdit *leBinary;
    QLineEdit *leIcon;
    QLineEdit *leDest;

    void createActions();
    void createInputs();
};

#endif  // MAINWINDOW_H

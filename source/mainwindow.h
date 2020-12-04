#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>


class MainWindow : public QMainWindow
{
    Q_OBJECT;

    public:
        MainWindow();

    public slots:
        void getBinaryPath();
        void getIconPath();
        void getDestPath();

    private:
        QString binaryPath;
        QString iconPath;
        QString destPath;
        QString linkPath;

        QLineEdit *leBinary;
        QLineEdit *leIcon;
        QLineEdit *leDest;

        void createActions();
        void createInputs();

};

#endif //MAINWINDOW_H

#ifndef PATHINPUT_H
#define PATHINPUT_H

#include <QtWidgets>

class QaaPathEdit : public QLineEdit {
    Q_OBJECT;

   public:
    QaaPathEdit(bool folder);

   signals:
    void changedByDrop(const QString &path);

   private:
    bool folder;
    void dropEvent(QDropEvent *event) override;
};

class PathInput : public QWidget {
    Q_OBJECT;

   public:
    PathInput(QString label, bool markDirty, bool folder = false,
              const QString &caption = "Select a file",
              const QString &defaultPath = QString(),
              const QString &dir = QDir::homePath(),
              const QString &filter = QString());

    bool isDirty;
    QDir* getPath();

   public slots:
    void setPath(const QString &path);
    void setPathDialog();
    void validateInput(const QString &path);

   signals:
    void pathChanged(const QString &path);

   private:
    bool folder;
    QString caption;
    QString dir;
    QString filter;
    QDir *path;

    QLabel *label;
    QaaPathEdit *pePath;
    QToolButton *btnBrowse;
};

#endif  // PATHINPUT_H

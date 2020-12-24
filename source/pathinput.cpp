#include "pathinput.h"

#include <qnamespace.h>

#include <QtWidgets>

QaaPathEdit::QaaPathEdit(bool folder) : QLineEdit() { this->folder = folder; }

void QaaPathEdit::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasFormat("text/plain")) {
        if (event->mimeData()->text().startsWith("file://")) {
            QString filePath = event->mimeData()->text();
            // remove file://
            filePath.remove(0, 7);
            // remove trailing /r/n
            filePath.chop(2);
            // replace %20 with spaces
            filePath.replace("%20", " ");

            /* QFileInfo *dropFile = new QFileInfo(filePath); */
            QFileInfo dropFile(filePath);
            if (dropFile.isFile() && !this->folder) {
                this->clear();
                this->setText(filePath);
                emit changedByDrop(filePath);
            } else if (dropFile.isDir() && this->folder) {
                this->clear();
                this->setText(filePath);
                emit changedByDrop(filePath);
            }
        }
    }
}

PathInput::PathInput(QString label, bool markDirty, bool isSaveFile,
                     bool folder, const QString &caption,
                     const QString &defaultPath, const QString &dir,
                     const QString &filter)
    : QWidget(), caption(caption), dir(dir), filter(filter) {
    // initalize elements
    this->isSaveFile = isSaveFile;
    this->isDirty = markDirty;
    this->folder = folder;
    this->path = new QDir("/");

    // widgets
    this->label = new QLabel(label);
    this->pePath = new QaaPathEdit(folder);
    this->btnBrowse = new QToolButton();
    this->btnBrowse->setText("...");

    // layout
    QHBoxLayout *hboxMain = new QHBoxLayout;
    hboxMain->setContentsMargins(0, 5, 0, 5);
    hboxMain->addWidget(this->label, 1);
    hboxMain->addWidget(this->pePath, 5);
    hboxMain->addWidget(this->btnBrowse);

    // signals slots
    connect(this->pePath, &QaaPathEdit::changedByDrop, this,
            &PathInput::validateInput);
    connect(this->pePath, &QLineEdit::textEdited, this,
            &PathInput::validateInput);
    connect(this->btnBrowse, &QPushButton::clicked, this,
            &PathInput::setPathDialog);

    this->setPath(defaultPath);

    setLayout(hboxMain);
}

void PathInput::setPath(const QString &path) {
    QFileInfo inputPath(path);

    if (this->isSaveFile) {
        this->path->setPath(path);
        emit pathChanged(path);
        this->pePath->setText(path);
        this->isDirty = false;
    }

    if (inputPath.exists()) {
        this->path->setPath(path);

        emit pathChanged(path);
        // update lineedit
        this->pePath->setText(path);

        this->isDirty = false;
    }
}

void PathInput::validateInput(const QString &path) {
    QFileInfo pathInput = QFileInfo(path);

    if (isSaveFile) {
        // TODO: maybe test for write access here?
        /*
        QDir dirInput = QDir(pathInput.absolutePath());
        if (!dirInput.exists()) {
            QPalette badInput;
            badInput.setColor(QPalette::Base, Qt::darkRed);
            this->pePath->setPalette(badInput);
            this->isDirty = true;
        } else {
            */
            QPalette goodInput;
            pePath->setPalette(goodInput);
            this->setPath(pePath->text());
            this->isDirty = false;
        //}
    } else {
        if (!pathInput.exists()) {
            QPalette badInput;
            badInput.setColor(QPalette::Base, Qt::darkRed);
            this->pePath->setPalette(badInput);
            this->isDirty = true;
        } else {
            QPalette goodInput;
            pePath->setPalette(goodInput);
            this->setPath(pePath->text());
            this->isDirty = false;
        }
    }
}

void PathInput::setPathDialog() {
    if (this->folder) {
        QString path =
            QFileDialog::getExistingDirectory(this, this->caption, this->dir);
        if (path.isNull() == false) {
            QPalette goodInput;
            pePath->setPalette(goodInput);
            this->setPath(path);
        }
    } else if (this->isSaveFile) {
        QString path = QFileDialog::getSaveFileName(this, this->caption, this->dir);
        if (path.isNull() == false) {
            QPalette goodInput;
            pePath->setPalette(goodInput);
            this->setPath(path);
        }

    } else {
        QString path = QFileDialog::getOpenFileName(this, this->caption,
                                                    this->dir, this->filter);
        if (path.isNull() == false) {
            QPalette goodInput;
            pePath->setPalette(goodInput);
            this->setPath(path);
        }
    }
}

QDir *PathInput::getPath() { return this->path; }

/*******************************************************************************************
**
** Project: Photo Sorter
** File: MainWindow.cpp
**
** Copyright (C) 2014 techvoid.
** All rights reserved.
** http://sourceforge.net/projects/photosorter/
**
** This program is free software: you can redistribute it and/or modify it under the terms
** of the GNU General Public License as published by the Free Software Foundation, either
** version 3 of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
** without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along with this
** program. If not, see <http://www.gnu.org/licenses/>.
**
*******************************************************************************************/

/*======================================== HEADERS =======================================*/

#include "MainWindow.h"

#include <QtGui/QFileDialog>
#include <QtGui/QProgressDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QShortcut>
#include <QtGui/QTreeView>

#include <QtCore/QCryptographicHash>
#include <QtCore/QDirIterator>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>
#include <QtCore/QDir>

//#include <windows.h>
#include <gdiplus.h>

#include "Global.h"

using namespace Gdiplus;

/*======================================== PUBLIC ========================================*/

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent)
{
    ui.setupUi(this);
    ui.listWidget->setAcceptDrops(true);
    ui.radioButton->setChecked(true);
    ui.checkBox->setChecked(true);
    ui.checkBox_2->setChecked(true);
    ui.checkBox_2->setEnabled(false);
    setWindowTitle(APP_PRODUCTNAME);

    connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(ChooseTarget()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(AddElement()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(RemoveElements()));
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Start()));

    QShortcut *scDelete = new QShortcut(QKeySequence(Qt::Key_Delete), ui.listWidget);
    connect(scDelete, SIGNAL(activated()), this, SLOT(RemoveElements()));
}

void MainWindow::SetTarget(QString pPath)
{
    ui.lineEdit->setText(pPath);
}

QString MainWindow::GetTarget() const
{
    return ui.lineEdit->text();
}

void MainWindow::SetMode(int pMode)
{
    if (pMode > 0) {
        ui.radioButton_2->setChecked(true);
    } else {
        ui.radioButton->setChecked(true);
    }
}

void MainWindow::AddElement(const QString &pPath)
{
    ui.listWidget->addEditableItem(pPath);
}

bool MainWindow::IsContentSame(const QString &pFirstFile, const QString &pSecondFile)
{
    QFile FirstFile(pFirstFile);
    if (!FirstFile.open(QIODevice::ReadOnly))
        return false;

    QFile SecondFile(pSecondFile);
    if (!SecondFile.open(QIODevice::ReadOnly))
        return false;

    if (FirstFile.readAll() != SecondFile.readAll())
        return false;

    return true;
}

bool MainWindow::Process()
{
    QProgressDialog mProgress(this);
    mProgress.setCancelButtonText("Abort");
    mProgress.setWindowFlags(windowFlags());
    mProgress.setMinimumSize(mProgress.size());
    mProgress.setMaximumSize(mProgress.size());
    mProgress.setWindowModality(Qt::WindowModal);
    mProgress.setMinimumDuration(0);
    mProgress.setLabelText("Preparing...");

    QFileInfoList filesToTransfer;

    mProgress.setValue(0);
    int count = ui.listWidget->count();
    for (int index = 0; index < count; index++) {
        QListWidgetItem *item = ui.listWidget->item(index);
        QString itemName = item->text();
        QFileInfo fileInfo(itemName);

        if (!fileInfo.exists()) {
            break;
        }

        if (!fileInfo.isDir()) {
            filesToTransfer.append(itemName);
        } else {
            QDirIterator iterator(itemName, QDir::Files | QDir::NoDotAndDotDot,
                                  QDirIterator::Subdirectories);
            while (iterator.hasNext()) {
                if (mProgress.wasCanceled())
                    return false;
                iterator.next();
                filesToTransfer.append(iterator.fileInfo());
            }
        }
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    int tValue = 0;
    mProgress.setLabelText("Processing data...");
    mProgress.setMaximum(filesToTransfer.count());
    foreach(QFileInfo fileToTransfer, filesToTransfer) {
        mProgress.setValue(tValue++);
        if (mProgress.wasCanceled())
            return false;

        QDateTime fileDate = GetPhotoTakenTime(fileToTransfer.filePath());
        if (fileDate.isNull())
            fileDate = fileToTransfer.lastModified();

        QString fileLastMod = fileDate.toString("yyyy.MM.dd");
        QStringList filter(fileLastMod+" *");
        QFileInfoList dateFolders = QDir(GetTarget()).entryInfoList(filter, QDir::Dirs);

        QFileInfo stdDateFolder(GetTarget(), fileLastMod);

        // use standard folder if there aren't any non-standard ones.
        if (dateFolders.isEmpty())
            dateFolders << stdDateFolder;

        QFileInfoList similarFiles;
        foreach(QFileInfo dateFolder, dateFolders) {
            QDirIterator iterator(dateFolder.filePath(), QDir::Files |
                                  QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (iterator.hasNext()) {
                iterator.next();

                if (iterator.fileName() == fileToTransfer.fileName())
                    similarFiles += iterator.fileInfo();
            }
        }

        // if there aren't any similar files, redirect operation to standard folder.
        if (similarFiles.isEmpty())
            similarFiles += QFileInfo(stdDateFolder.filePath(), fileToTransfer.fileName());

        foreach(QFileInfo similarFile, similarFiles) {
            if (ui.checkBox_2->isChecked()) {
                QDateTime fileTargetDate = GetPhotoTakenTime(similarFile.filePath());
                if (fileTargetDate.isNull())
                    fileTargetDate = similarFile.lastModified();

                if (fileDate < fileTargetDate) {
                    break;
                }
            }

            if (ui.checkBox->isChecked()) {
                if (IsContentSame(fileToTransfer.filePath(), similarFile.filePath())) {
                    break;
                }
            }

            QDir dirOp;
            dirOp.mkpath(similarFile.dir().path());
            dirOp.remove(similarFile.filePath());

            if (ui.radioButton_2->isChecked()) {
                QFile::rename(fileToTransfer.filePath(),similarFile.filePath());
            } else if (ui.radioButton->isChecked()) {
                QFile::copy(fileToTransfer.filePath(),similarFile.filePath());
            }
        }
    }

    GdiplusShutdown(gdiplusToken);

    QMessageBox::information(&mProgress, tr("Information"),
                             tr("Processing has been completed."));
    return true;
}

/*===================================== PUBLIC SLOTS =====================================*/

void MainWindow::ChooseTarget()
{
    QString tPath = GetTarget();
    if (tPath.isEmpty())
        tPath = QDir::currentPath();

    QString rFile = GetDirectoryDialog(tPath);

    if (!rFile.isEmpty())
        ui.lineEdit->setText(rFile);
}

void MainWindow::AddElement()
{
    QString tPath;
    if (ui.listWidget->selectedItems().count() > 0) {
        tPath = ui.listWidget->selectedItems().at(0)->text();
    } else {
        tPath = QDir::currentPath();
    }

    QStringList tFiles = GetElementDialog(tPath);

    foreach(QString rFile, tFiles) {
        AddElement(rFile);
    }
}

void MainWindow::RemoveElements()
{
    foreach(QListWidgetItem *item, ui.listWidget->selectedItems()) {
        delete item;
    }
}

void MainWindow::Start()
{
    Process();
}

/*===================================== PRIVATE SLOTS ====================================*/

/*======================================= PROTECTED ======================================*/

/*======================================== PRIVATE =======================================*/

QString MainWindow::GetDirectoryDialog(const QString &pPath)
{
    return QFileDialog::getExistingDirectory(this, tr("Choose folder"), pPath);
}

QStringList MainWindow::GetElementDialog(const QString &pPath)
{
    QFileDialog tFileDialog(this);
    tFileDialog.setFileMode(QFileDialog::AnyFile);
    tFileDialog.setDirectory(pPath);
    tFileDialog.setWindowTitle(tr("Choose files and/or folders"));
    tFileDialog.setLabelText(QFileDialog::Accept, "Select");
    //tFileDialog.setOption(QFileDialog::DontUseNativeDialog, true);

    QListView *rListView = tFileDialog.findChild<QListView *>("listView");
    if (rListView) {
        rListView->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    QTreeView *rTreeView = tFileDialog.findChild<QTreeView *>();
    if (rTreeView) {
        rTreeView->setSelectionMode(QAbstractItemView::MultiSelection);
    }

    if (tFileDialog.exec() > 0)
        return tFileDialog.selectedFiles();

    return QStringList();
}

QDateTime MainWindow::GetPhotoTakenTime(const QString &pPath)
{
    Image *image = new Image(pPath.toStdWString().c_str());

    // Assume that the image has a property item of type PropertyTagExifDTOrig.
    // Get the size of that property item.
    UINT size = image->GetPropertyItemSize(PropertyTagExifDTOrig);

    // Allocate a buffer to receive the property item.
    PropertyItem *propertyItem = (PropertyItem *)malloc(size);

    // Get the property item.
    if (image->GetPropertyItem(PropertyTagExifDTOrig, size, propertyItem) != Ok){
        free(propertyItem);
        delete image;
        return QDateTime();
    }

    // PropertyTagExifDTOrig has the following format: YYYY:MM:DD HH:MM:SS
    QDateTime timeTaken = QDateTime::fromString((char *)propertyItem->value,
                          "yyyy:MM:dd hh:mm:ss");

    free(propertyItem);
    delete image;
    return timeTaken;
}

/*========================================================================================*/

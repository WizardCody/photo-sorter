/*******************************************************************************************
**
** Project: Photo Sorter
** File: MainWindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "generated/MainWindow_ui.h"

#include <QtCore/QString>

/*======================================== CLASSES =======================================*/

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *pParent = 0);

        void SetTarget(QString pPath);
        QString GetTarget() const;

        void SetMode(int pMode);

        void AddElement(const QString &pPath);

        bool IsContentSame(const QString &pFirstFile, const QString &pSecondFile);

        bool Process();

    public slots:
        void ChooseTarget();
        void AddElement();
        void RemoveElements();
        void Start();

    private slots:

    signals:

    protected:

    private:
        Ui_MainWindow ui;

        QString GetDirectoryDialog(const QString &pPath);
        QStringList GetElementDialog(const QString &pPath);

        // You have to start the Gdiplus before using this function
        QDateTime GetPhotoTakenTime(const QString &pPath);
};


#endif // MAINWINDOW_H

/*========================================================================================*/

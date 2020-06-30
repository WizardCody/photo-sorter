/*******************************************************************************************
**
** Project: Photo Sorter
** File: PhotoSorter.cpp
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

#include <QtGui/QApplication>
#include <QtCore/QStringList>

#include "MainWindow.h"

/*======================================== GLOBAL ========================================*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStringList tArguments = app.arguments();
    tArguments.takeFirst();

    MainWindow mWindow;
    mWindow.show();

    if (tArguments.count() > 2) {
        mWindow.SetMode(tArguments.at(0).toInt());
        mWindow.SetTarget(tArguments.at(1));
        mWindow.AddElement(tArguments.at(2));
        mWindow.Start();
    }

    return app.exec();
}

/*========================================================================================*/

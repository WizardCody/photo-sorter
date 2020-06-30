/*******************************************************************************************
**
** Project: Photo Sorter
** File: LineEdit.h
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

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QtGui/QListWidget>

/*======================================== CLASSES =======================================*/


class ListView : public QListWidget
{
        Q_OBJECT

    public:
        ListView(QWidget *pParent = 0);

        void addEditableItem(const QString &pItem);

    public slots:

    private slots:

    signals:

    protected:
        void dragEnterEvent(QDragEnterEvent *pEvent);
        void dragMoveEvent(QDragMoveEvent *pEvent);
        void dragLeaveEvent(QDragLeaveEvent *pEvent);
        void dropEvent(QDropEvent *pEvent);

    private:

};

#endif // LISTVIEW_H

/*========================================================================================*/

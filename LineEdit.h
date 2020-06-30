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

#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QtGui/QLineEdit>

/*======================================== CLASSES =======================================*/

class LineEdit : public QLineEdit
{
        Q_OBJECT

    public:
        LineEdit(QWidget *pParent = 0);

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

#endif // LINEEDIT_H

/*========================================================================================*/

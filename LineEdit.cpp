/*******************************************************************************************
**
** Project: Photo Sorter
** File: LineEdit.cpp
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

#include "LineEdit.h"

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QUrl>

#include <QtCore/QFileInfo>

/*======================================== PUBLIC ========================================*/

LineEdit::LineEdit(QWidget *pParent) :
    QLineEdit(pParent)
{
    // cst
}

/*===================================== PUBLIC SLOTS =====================================*/

/*===================================== PRIVATE SLOTS ====================================*/

/*======================================= PROTECTED ======================================*/

void LineEdit::dragEnterEvent(QDragEnterEvent *pEvent)
{
    pEvent->acceptProposedAction();
}
void LineEdit::dragMoveEvent(QDragMoveEvent *pEvent)
{
    pEvent->acceptProposedAction();
}
void LineEdit::dragLeaveEvent(QDragLeaveEvent *pEvent)
{
    pEvent->accept();
}
void LineEdit::dropEvent(QDropEvent *pEvent)
{
    const QMimeData *tMimeData = pEvent->mimeData();
    if (tMimeData->hasUrls()) {
        pEvent->acceptProposedAction();
        QList<QUrl> tUrlList = tMimeData->urls();
        setText(QFileInfo(tUrlList.at(0).toLocalFile()).canonicalFilePath());
    }
}

/*======================================== PRIVATE =======================================*/

/*========================================================================================*/

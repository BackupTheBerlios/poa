/*****************************************************************************
 * POA - Programmierumgebung Offener Antrieb
 *
 * Copyright (C) 2003 Necati Aydin, Anselm Garbe, Stefan Hauser,
 * Steffen Keul, Marcel Kilgus, Steffen Pingel, Tammo van Lessen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Id: util.cpp,v 1.1 2003/09/18 13:06:22 garbeam Exp $
 *
 *****************************************************************************/

#include "util.h"

#include <qfileinfo.h>
#include <qstring.h>
#include <qstringlist.h> 

bool Util::copyFile(QFile *source, QFile *target)
{
    QStringList lines;
    if (source->open(IO_ReadOnly) && target->open(IO_WriteOnly)) {

        QTextStream istream(source);

        QString line;
        while (!istream.eof()) {
            line = istream.readLine();
            lines += line;
        }
        source->close();

        QTextStream ostream(target);
        for (QStringList::Iterator it = lines.begin(); it != lines.end(); ++it) {
            ostream << *it << "\n";
        }
        target->close();

        return true;
    }

    return false;
}

bool Util::removeDir(QDir *subDir)
{
    const QFileInfoList *filist = subDir->entryInfoList();
    QFileInfoListIterator it(*filist);
    QFileInfo *fi;
    while ((fi = it.current()) != 0) {
        ++it;
        if (fi->isDir()) {
            QDir *dir = new QDir(fi->filePath());
            if (!removeDir(dir))
            {
                delete dir; // free
                // error, directory is not empty!
                return false;
            }
            delete dir; // else
        }
        else {
            if (!QFile(fi->filePath()).remove())
            {
                return false;
            }
        }
    }

    return subDir->rmdir(subDir->absPath());
}

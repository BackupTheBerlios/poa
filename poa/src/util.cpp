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
 * $Id: util.cpp,v 1.6 2004/01/12 19:13:57 squig Exp $
 *
 *****************************************************************************/

#include "util.h"

#include <qfileinfo.h>
#include <qstring.h>
#include <qstringlist.h>

const QString Util::ICON_PATH = QString("icons/");

QString Util::EXECUTABLE_PATH = QString("");

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

QString Util::findIcon(QString filename)
{
    return findResource(ICON_PATH + filename);
}

QString Util::findResource(QString filename)
{
    QFileInfo fi = QFileInfo(filename);
    if (fi.exists()) {
        return fi.absFilePath();
    }

    fi = QFileInfo(EXECUTABLE_PATH + "/" + filename);
    if (fi.exists()) {
        return fi.absFilePath();
    }

    fi = QFileInfo(QString("/usr/share/poa/") + filename);
    if (fi.exists()) {
        return fi.absFilePath();
    }

    return QString();
}

bool Util::removeDir(QDir *subDir)
{
    const QFileInfoList *filist = subDir->entryInfoList();
    if (filist != 0) {
        QFileInfoListIterator it(*filist);
        QFileInfo *fi;
        while ((fi = it.current()) != 0) {
            ++it;
            if (fi->fileName() == "." || fi->fileName() == "..") {
                continue;
            }
            else if (fi->isDir()) {
                QDir dir(fi->filePath());
                if (!removeDir(&dir)) {
                    // error, directory is not empty!
                    return false;
                }
            }
            else if (!QFile(fi->filePath()).remove()) {
                return false;
            }
        }
    }

    return subDir->rmdir(subDir->absPath());
}

QString Util::squeeze(QString text, int maxWidth, QFont font)
{
    QFontMetrics fm = QFontMetrics(font);
    if (fm.width(text) < maxWidth) {
        return text;
    }
    else {
        QString s = text.left(1);
        int dotWidth = fm.width("...");
        int i = 1;
        while (fm.width(s) + fm.width(text[i]) + dotWidth <= maxWidth) {
            s += text[i];
            i++;
        }
        return s + "...";
    }
}

void Util::setExecutablePath(QString path)
{
    Util::EXECUTABLE_PATH = path;
}

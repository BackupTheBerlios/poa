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
 * $Id: util.h,v 1.3 2003/09/19 15:09:28 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_UTIL_H
#define POA_UTIL_H

#include <qdir.h>
#include <qfile.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qstring.h>
#include <qstringlist.h>

/**
 * Provides utilities for
 * <ul>
 *   <li>file/directory manipulation</li>
 *   <li>string manipulation</li>
 * </ul>
 */
class Util
{

 public:

    /**
     * Copies <code>source</code>'s content line by line
     * to <code>target</code>.
     * Assumes that:
     * <ul>
     *  <li><code>source</code> exists, is accessible, is not opened</li>
     *  <li><code>target</code> does not exist yet (or will be
     *  overwritten), is not opened</li>
     *  <li> <code>target</code>'s directory exists.
     * </ul>
     * Returns <code>true</code> if the file could be copied
     * successfully, <code>false</code> otherwise.
     */
    static bool copyFile(QFile *source, QFile *target);

    /**
     * Removes a directory with all it's files subdirectories.
     * Assumes that the user has all permissions to perform the removal.
     * Returns <code>true</code> if the directory could be deleted
     * successfully, <code>false</code> otherwise.
     * @param subDir the directory to be removed.
     */
    static bool removeDir(QDir *subDir);

    /**
     * Shortens text to fit it in maxWidth pixels. If text does not
     * fit characters are chopped from the end and three dots "..."
     * are appended.
     *
     * @return the shortened text
     */
    static QString squeeze(QString text, int maxWidth, QFont font);

};

#endif // POA_UTIL_H

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
 * $Id: util.h,v 1.1 2003/09/18 13:06:22 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_UTIL_H
#define POA_UTIL_H

#include <qdir.h>
#include <qfile.h>

/**
 * Provides utilities for
 * <ul>
 *   <li>file/directory manipulation</li>
 *   <li>etc.</li>
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


};

#endif // POA_UTIL_H

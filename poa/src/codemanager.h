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
 * $Id: codemanager.h,v 1.9 2003/09/17 15:38:52 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_CODEMANAGER_H
#define POA_CODEMANAGER_H

#include "cpumodel.h"

#include <qdir.h>
#include <qfile.h>
#include <qstring.h>

/**
 * Manages a portion of source code.
 */
class CodeManager : public QObject
{

    Q_OBJECT

public:

    /**
     * CodeManager singleton instance.
     */
    static CodeManager *instance();

    /**
     * Returns path to the source file.
     * If the project was not saved, the source file will be
     * the standard template. Otherwise it will be managed implicitly.
     */
    QString sourceFilePath(CpuModel *model);

    /**
     * Returns path to the CPU specific top level directory.
     */
    QString sourcePath(CpuModel *model);

    /**
     * Executes the compiler and returns it's return code.
     */
    int compile(CpuModel *model);

    /**
     * Opens the external editor with the source code of this CPU.
     * If the CPU_NN_sdk directory does not exist, it will be created.
     */
    void edit(CpuModel *model);


protected:
    CodeManager();
    ~CodeManager();

    /**
     * Removes the source template structure created by save().
     * Called by the destructor.
     */
    void remove(CpuModel *model);


private:
    static CodeManager* instance_;

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
    bool copyFile(QFile *source, QFile *target);

    /**
     * Removes a directory with all it's files subdirectories.
     * Assumes that the user has all permissions to perform the removal.
     * Returns <code>true</code> if the directory could be deleted
     * successfully, <code>false</code> otherwise.
     * @param subDir the directory to be removed.
     */
    bool removeDir(QDir *subDir);


public slots:
    /**
     * Saves the source template into a structure as follows:
     * CPU_nn_sdk
     *  src/cpu_nn.c
     *  lib/
     *  inc/
     */
    void save(CpuModel *model);

};

#endif // POA_CODEMANAGER_H

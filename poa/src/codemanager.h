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
 * $Id: codemanager.h,v 1.15 2004/02/09 19:12:12 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_CODEMANAGER_H
#define POA_CODEMANAGER_H

class CpuModel;
class Project;

#include <qdir.h>
#include <qstring.h>

/**
 * Manages a portion of source code.
 */
class CodeManager {

public:

    CodeManager(Project *project, CpuModel *cpuModel);
    ~CodeManager();

    /**
     * Returns path to the CPU specific top level directory.
     */
    QString cpuPath();

    void copyTemplate();

    void createDirectories();

    /**
     * Returns path to the CPU specific library directory.
     */
    QString libPath();

    /**
     * Returns path to the CPU specific inclusion directory.
     */
    QString incPath();

    /**
     * Returns filename of the source file of this cpu.
     */
    QString filename(QString extension = "c");

    /**
     * Executes the compiler and returns it's return code.
     */
    int compile();

    /**
     * Opens the external editor with the source code of this CPU and
     * returns <code>true</code> if editor launch was successful.
     * If the CPU_NN_sdk directory does not exist, it will be created.
     */
    bool edit();

    /**
     * Removes the source template structure created by save().
     */
    //void remove(CpuModel *model);

    void saveSource();

    /**
     * Returns the content of the source file in a QString.
     */
    QString sourceCode();

    /**
     * Returns path to the source file.
     */
    QString sourceFilePath(QString extension = "c");

    /**
     * Returns path to the CPU specific source directory.
     */
    QString sourcePath();

    /**
     * Returns <code>true</code> if the template is substitutable.
     */
    bool templateIsSubstitutable();

    /**
     * Prepends substitution comment markers at the beginning of the
     * template.
     */
    void prependSubstitutionMarkers();

    /**
     * Substitutes current models_ pin values to the template.
     * Asserts that templateIsSubstitutable() returns <code>true</code>.
     */
    void substitute();

private:
    void createDirectory(const QDir &path);

    CpuModel *model_;
    Project *project_;

};

#endif // POA_CODEMANAGER_H

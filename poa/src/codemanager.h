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
 * $Id: codemanager.h,v 1.4 2003/09/16 15:18:13 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_CODEMANAGER_H
#define POA_CODEMANAGER_H

#include "abstractmodel.h"

#include <qstring.h>

/**
 * Manages a portion of source code.
 */
class CodeManager : public QObject
{

    Q_OBJECT

public:

    /**
     * Basic constructor.
     */
    CodeManager(AbstractModel *parent);

    /**
     * Destructs this CodeManager and removes all related data
     * physically (e.g. the Source code) {@link remove()}.
     */
    ~CodeManager();

    /**
     * Returns path to the source file.
     * If the project was not saved, the source file will be
     * the standard template. Otherwise it will be managed implicitly.
     */
    QString sourcePath();

    /**
     * Executes the compiler and returns it's return code.
     */
    int compile();

    /**
     * Saves the source template into a structure as follows:
     * CPU_nn_sdk
     *  src/cpu_nn.c
     *  lib/
     *  inc/
     */
    void save();


private:
    AbstractModel *model_;

public slots:
    /**
     * Removes the source template structure created by save().
     * Called by the destructor.
     */
    void remove();

};

#endif // POA_CODEMANAGER_H

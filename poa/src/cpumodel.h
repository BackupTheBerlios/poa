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
 * $Id: cpumodel.h,v 1.8 2003/08/22 22:47:49 squig Exp $
 *
 *****************************************************************************/
#ifndef POA_CPUMODEL_H
#define POA_CPUMODEL_H

#include "blockmodel.h"

#include <qdom.h>

/*****************************************************************************
 * A block that executes some source code everytime it is triggered.
 * A CpuModel-object can calculate its execution time based on its
 * source code.
 *
 * @author keulsn
 */
class CpuModel: public BlockModel
{

 public:
    CpuModel(unsigned short id, bool autoExecTime, QString name,
             QString description);
    CpuModel(QDomElement cpuElement);

    QDomElement serialize(QDomDocument *document);

 private:

    unsigned short id_;
    bool autoExecTime_;

};

#endif

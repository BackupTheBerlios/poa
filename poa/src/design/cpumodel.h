/*****************************************************************************
 *
 *  POA - Programmierumgebung Offener Antrieb
 *
 *  Copyright (C) 2003 Necati Aydin, Anselm Garbe, Stefan Hauser,
 *  Steffen Keul, Marcel Kilgus, Steffen Pingel, Tammo van Lessen
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *  First Author: Steffen Keul
 *
 *  $RCSfile: cpumodel.h,v $, $Revision: 1.2 $
 *  $Author: keulsn $
 *  $Date: 2003/07/08 17:34:13 $
 *
 *****************************************************************************/


#ifndef POA_CPUMODEL_H
#define POA_CPUMODEL_H


#include "codemanager.h"
#include "functionblockmodel.h"


/*****************************************************************************
 *
 *  A block that executes some source code everytime it is triggered.
 *  A CpuModel-object can calculate its execution time based on its
 *  source code.
 *
 */
class CpuModel: public FunctionBlockModel
{

 public:

 private:

    unsigned short id_;
    CodeManager *code_;
    bool autoExecTime_;

};


#endif

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
 *  $RCSfile: functionblockmodel.h,v $, $Revision: 1.1 $
 *  $Author: keulsn $
 *  $Date: 2003/07/07 23:35:41 $
 *
 *****************************************************************************/


#ifndef POA_FUNCTIONBLOCKMODEL_H
#define POA_FUNCTIONBLOCKMODEL_H


#include "muxmodel.h"


/*****************************************************************************
 *
 *  A block that receives inputs, calculates a function and then produces
 *  outputs. The calculation process takes some time.
 *
 */
class FunctionBlockModel: public MuxModel
{

  public:
  
    virtual bool isProducer() const;

  private:

    unsigned long execTime;

};


#endif

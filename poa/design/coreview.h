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
 * $Id: coreview.h,v 1.1 2003/08/28 18:24:21 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_COREVIEW_H
#define POA_COREVIEW_H

#include "coremodel.h"
#include "inputblockview.h"
#include "outputblockview.h"


/*****************************************************************************
 * Definition of a core view.
 * @author garbeam
 */
class CoreView: public InputBlockView, public OutputBlockView
{

private:

    CoreModel *coreModel_;

public:

    CoreModel *getModel();
    void setModel(CoreModel *coreModel);

};

#endif // POA_COREVIEW_H

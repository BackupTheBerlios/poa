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
 * $Id: router.h,v 1.1 2003/12/03 11:13:02 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_ROUTER
#define POA_ROUTER

#include <qcanvas.h>

class Router 
{
public:

    /**
     * Re-routes all {@link ConnectorViewList}s in <code>items</code>.
     */
    virtual void route(QCanvasItemList items) = 0;

};


#endif // POA_ROUTER

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
 * $Id: modulelibraryitem.h,v 1.3 2003/08/22 13:45:53 garbeam Exp $
 *
 *****************************************************************************/


#ifndef POA_MODULELIBRARYITEM_H
#define POA_MODULELIBRARYITEM_H


#include <qobject.h>
class QString;

/*****************************************************************************
 * Base class for items in the module library.
 *
 * @author keulsn
 * @author Steffen Pingel
 */
class ModuleLibraryItem : public QObject
{

public:
	/*************************************************************************
	 * Returns a description of the item.
	 */
	virtual QCString description() = 0;

	/*************************************************************************
	 * Returns the name of the item.
	 */
	virtual QCString name() = 0;

	/*************************************************************************
	 * Returns an XML representation of this instance.
	 */
//    virtual QByteArray serialize() = 0;

};

#endif

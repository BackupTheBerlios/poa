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
 * $Id: modelfactory.h,v 1.10 2003/09/30 21:54:18 vanto Exp $
 *
 *****************************************************************************/
#ifndef MODELFACTORY_H
#define MODELFACTORY_H

class AbstractModel;

#include <qvaluelist.h>
class QDomNode;

/**  class AbstractModelList : public QValueList<AbstractModel *> { */
/**  }; */

/**
 * Generates model objects from xml data.
 */
class ModelFactory
{
public:

    /**
     * Generates cpu respectivly core models
     * @param virgin set this to true to create clean new instances.
     */
    static QValueList<AbstractModel *> generate(QDomNode node, bool virgin = true);
};

#endif // MODELFACTORY_H

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
 * $Id: modelfactory.cpp,v 1.10 2003/09/07 19:07:46 squig Exp $
 *
 *****************************************************************************/
#include "modelfactory.h"

#include "cpumodel.h"
#include "coremodel.h"
#include "project.h"
#include <qdom.h>

/*****************************************************************************
 * Generates model objects from xml data.
 */
QValueList<AbstractModel *> ModelFactory::generate(QDomNode node)
{
    QValueList<AbstractModel *> l;

    QDomNodeList children
        = (node.isDocument())
        ? node.toDocument().elementsByTagName("model-item")
        : node.childNodes();
    for (uint i = 0; i < children.length(); i++) {
        QDomNode item = children.item(i);
        if (item.isElement() && item.nodeName() == "model-item") {
            AbstractModel *model = 0;
            QDomElement element = item.toElement();
            if (element.attribute("block-type","") == "cpu") {
                model = new CpuModel(element);
            }
            else if (element.attribute("block-type","") == "core") {
                model = new CoreModel(element);
            }

            if (model != 0) {
                model->setDescription(QString::null);
                model->setId(0);
                model->setName("new "+model->type());
                l.append(model);
            }
        }
    }

    return l;
}

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
 * $Id: project.h,v 1.5 2003/08/27 21:12:45 vanto Exp $
 *
 *****************************************************************************/
#ifndef PROJECT_H
#define PROJECT_H

#include "abstractmodel.h"
#include "blockmodel.h"
#include "gridcanvas.h"

#include <qptrlist.h>
#include <qdom.h>
#include <qmap.h>

/*****************************************************************************
 * Basic map container for BlockModel objects.
 * Used by NetworkCanvas to store its BlockModelView items.
 */
class Project : public QObject
{
    Q_OBJECT

public:
    Project();
    ~Project();

    void add(AbstractModel *item, int x, int y);
    void addCanvas(GridCanvas *canvas);
    const QPtrList<GridCanvas> *canvasList() const;
    QDomDocument serialize();
    void deserialize(QDomDocument *document);

signals:
    void modelAdded(AbstractModel *item, int x, int y);

private:
    QPtrList<AbstractModel> items_;
    QPtrList<GridCanvas> canvasList_;
};

#endif // PROJECT_H
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
 * $Id: project.h,v 1.8 2003/08/29 10:45:49 vanto Exp $
 *
 *****************************************************************************/
#ifndef PROJECT_H
#define PROJECT_H

#include "abstractmodel.h"
#include "blockmodel.h"
#include "gridcanvas.h"

#include <qptrlist.h>
#include <qdom.h>
#include <qcanvas.h>
#include <qmap.h>

/*****************************************************************************
 * Project
 * Contains the model and a list of canvases
 */
class Project : public QObject
{
    Q_OBJECT

public:
    /*
     * Creates a new project with the given name.
     * @param name will be showed in the mdiwindow title
     */
    Project(QString name);

    /*
     * Creates a new project and deserialized the given xml document
     * @param document will be deserialized
     * @param name will be showed in the mdiwindow title
     */
    Project(QString name, QDomDocument *document);

    /*
     * Destroys the project
     */
    ~Project();

    /*
     * Adds a model to the project
     * @param item the AbstractModel
     */
    void add(AbstractModel *item);

    /*
     * Returns the <code>project</code>'s name
     */
    QString name();

    /*
     * Creates a new Canvas with the given name
     * @param name canvas' name
     */
    GridCanvas *newCanvas(const QString name);

    /*
     * Returns a QPtrList of all canvases
     */
    const QPtrList<GridCanvas> *canvasList() const;

    /*
     * Serializes this project to xml
     */
    QDomDocument serialize();

private:
    /*
     * Deserializes the given xml document to this project
     */
    void deserialize(QDomDocument *document);

    /* List of <code>AbstractModel</code>s */
    QPtrList<AbstractModel> items_;
    /* List of <code>GridCanvas</code>'s */
    QPtrList<GridCanvas> canvasList_;
    /* project name */
    QString name_;
};

#endif // PROJECT_H

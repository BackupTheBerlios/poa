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
 * $Id: project.h,v 1.17 2003/09/16 15:18:13 garbeam Exp $
 *
 *****************************************************************************/
#ifndef PROJECT_H
#define PROJECT_H

#include "abstractmodel.h"
#include "blockmodel.h"
#include "gridcanvas.h"
#include "pinmodel.h"
#include "pinview.h"

#include <qptrlist.h>
#include <qdom.h>
#include <qcanvas.h>
#include <qmap.h>

/**
 * Project
 * Contains the model and a list of canvases
 */
class Project : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new project with the given name.
     * @param name will be showed in the mdiwindow title
     * @param document will be deserialized, if != 0
     */
    Project(QString name, QDomDocument *document = 0);

    /**
     * Destroys the project
     */
    ~Project();

    /**
     * Adds a block model to the project
     * @param item the AbstractModel
     */
    void addBlock(AbstractModel *item);

    /**
     * Creates the item's corresponding view object on all canvases.
     *
     * @see GridCanvas#addView(AbstractModel *)
     */
    void createViews(AbstractModel *item, int x = 0, int y = 0);

    /**
     * Creates ConnectorViews between the two given PinViews.
     * @param source the source PinView.
     * @param target the target PinView.
     */
    void createConnectorViews(PinView *source, PinView *target);

    /**
     * Returns the project's filenname.
     * @return QString::null, if no filename has been set
     */
    QString filename();

    /**
     * Returns the <code>project</code>'s name
     */
    QString name();

    /**
     * Returns the <code>project</code>s path
     */
    QString path();

    /**
     * Creates a new Canvas with the given name
     * @param name canvas' name
     */
    GridCanvas *newCanvas(const QString name);

    /**
     * Returns a QPtrList of all canvases
     */
    const QPtrList<GridCanvas> *canvasList() const;

    /**
     * Serializes this project to xml
     */
    QDomDocument serialize();

    /**
     * Sets the project's filenname for serialization.
     */
    void setFilename(const QString &filename);

private:
    /**
     * Deserializes the given xml document to this project
     */
    void deserialize(QDomDocument *document);

    /** List of <code>AbstractModel</code>s */
    QPtrList<AbstractModel> blocks_;
    /** List of <code>GridCanvas</code>'s */
    QPtrList<GridCanvas> canvasList_;
    /** project name */
    QString name_;
    /** current model id - provides project-wide unique ids */
    uint currentBlockId_;
    /** The filename for serialization. */
    QString filename_;
    /** The project path (without project file) for serialization */
    QString path_;
};

#endif // PROJECT_H

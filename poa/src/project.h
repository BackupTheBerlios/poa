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
 * $Id: project.h,v 1.29 2004/03/19 15:54:34 vanto Exp $
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
 * Stores the model items for a project.
 * Contains the model and a list of canvases
 */
class Project : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new project with the given name.
     * @param path path to project.xml. will be used to load and store project
     *        data and shown in the title of the mdiwindow.
     */
    Project(QString path);


    /**
     * Destroys the project
     */
    ~Project();

    /**
     * Adds a model to the project.
     * @param item the AbstractModel.
     */
    void addBlock(AbstractModel *item);

    /*
     * Removes a model from the project.
     * @param item the AbstractModel, must belong to the project.
     */
    void removeBlock(AbstractModel *item);

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
     * Returns the <code>project</code>'s name
     */
    QString name();

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
     * Returns a QPtrList of all blocks
     */
    QPtrList<AbstractModel> *Project::blocks();

    /**
     * Sets the changed-status of this project.
     * Set it to false on save, to true on changes.
     * Emits a modified() signal on true.
     */
    void setModified(bool);

    /**
     * Returns the changed-status of the project.
     * Returns true, if the project has been changed since the last save
     */
    bool isModified();

    /**
     * Serializes this project to xml.
     */
    QDomDocument serialize();

    /**
     * Saves the project
     */
    void save();

    /**
     * Saves the project in another directory.
     */
    void saveAs(QString path);

    /**
     * Sets the location of the project.
     */
    void setPath(QString path);

    /**
     * Returns the current project path.
     */
    QString projectPath();

    /*
     * Loads the project content.
     */
    void open();

signals:
    void modified(bool);

private:
    /**
     * Deserializes the given xml document to this project.
     */
    void deserialize(QDomDocument *document);

    /** List of <code>AbstractModel</code>s */
    QPtrList<AbstractModel> blocks_;
    /** List of <code>GridCanvas</code>'s */
    QPtrList<GridCanvas> canvasList_;
    /** project name */
    QString name_;
    /** current model id - provides project-wide unique ids. */
    uint currentBlockId_;
    /** The filename for serialization. */
    QString filename_;
    /** The project path (without project file) for serialization. */
    QString path_;
    /** is project changed since last save */
    bool modified_;
};

#endif // PROJECT_H

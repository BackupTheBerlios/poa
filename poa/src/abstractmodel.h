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
 * $Id: abstractmodel.h,v 1.24 2004/02/16 16:24:01 squig Exp $
 *
 *****************************************************************************/


#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include "serializable.h"
class GridCanvas;
class Project;

#include <qobject.h>
#include <qdom.h>
class QDomDocument;
class QDomElement;
class QCanvas;
class QCanvasItemList;

/**
 * Represents the base class of model classes that can be stored in
 * the library or a project. The AbstractModel class provides methods
 * for serialization and deserialization that should be reimplented by
 * subclasses.
 *
 * Each model can have multiple associated view objects that display
 * the model on a GridCanvas.
 *
 * @see LibraryWindow
 * @see Project
 */
class AbstractModel : public QObject, public Serializable
{
    Q_OBJECT

public:

    /**
     * Constructs a model.
     *
     * @param type the library category
     * @param description a short description
     * @param id the project unique id
     */
    AbstractModel(QString type = QString::null,
                  QString description = QString::null,
                  uint id = 0);

    /**
     * Returns the description of the model.
     */
    virtual QString description() const;

    /**
     * Returns the project-unique id of the model. This property is
     * managed by Project#addBlock(AbstractModel). Not used for
     * library items.
     */
    virtual uint id() const;

    /**
     * Creates the corresponding view objects on canvas.
     *
     * @param canvas the canvas the items will be created on
     */
    virtual QCanvasItemList createView(GridCanvas *canvas) = 0;

    /**
     * \copydoc Serializable::deserialize(QDomElement)
     */
    virtual void deserialize(QDomElement element);

    /**
     * Returns the name of the model.
     */
    virtual QString name() const;

    /**
     * Sets the description of the model
     *
     * @see #description()
     */
    virtual void setDescription(const QString &description);

    /**
     * Sets the project-wide id.
     *
     * @see #id()
     */
    virtual void setId(uint id);

    /**
     * Sets the name of the model.
     *
     * @see #name()
     */
    virtual void setName(QString name);

    /**
     * Sets the type of the model.
     *
     * @see #type()
     */
    virtual void setType(const QString &type);

    /**
     * \copydoc Serializable::serialize(QDomDocument)
     */
    virtual QDomElement serialize(QDomDocument *document);

    /**
     * Invoked when serializing the model for shipment to the
     * clipboard. Invokes serialize(QDomDocument), subclasses should
     * overwrite this if clipboard serialization requires special
     * treatment.
     *
     * @see #serialize(QDomDocument)
     * @see CpuModel#serializeCopy(QDomDocument,Project)
     */
    virtual QDomElement serializeCopy(QDomDocument *document, Project *project);

    /**
     * Needs to be invoked after the model properties have been
     * changed. Emits the updated() signal to notify the views
     * to repaint.
     */
    void updatePerformed();

    /**
     * Returns the type of this block, eg. CPU. The type is used as
     * the category in the library.
     */
    virtual QString type() const;

private:

    QString type_;
    QString description_;
    uint id_;
    QString name_;

signals:

    /**
     * Emitted when updatePerformed() is called.
     */
    virtual void updated();

};

#endif

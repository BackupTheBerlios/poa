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
 * $Id: abstractmodel.h,v 1.23 2004/01/28 16:35:51 squig Exp $
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
 * Base class for all model classes.
 */
class AbstractModel : public QObject, public Serializable
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    AbstractModel(QString type = QString::null,
                  QString description = QString::null,
                  uint id = 0);

    /**
     * Returns the description of the model.
     * This value contains different content
     * - in the library: the description of the generic block
     * - in the project: the description of the instanciated block
     */
    virtual QString description() const;

    /**
     * Returns the project-wide id of this block.
     * This property should be set in Project::add(),
     * for library items id should be 0
     */
    virtual uint id() const;

    /**
     * Creates the corresponding view objects.
     */
    virtual QCanvasItemList createView(GridCanvas *canvas) = 0;

    /**
     * \copydoc Serializable::deserialize(QDomElement)
     */
    virtual void deserialize(QDomElement element);

    /**
     * Returns the name of this model.
     */
    virtual QString name() const;

    /**
     * Sets the block description
     * {@link #description}
     */
    virtual void setDescription(const QString &description);

    /**
     * Sets the project-wide id
     * {@see #id}
     */
    virtual void setId(uint id);

    /**
     * Sets the name of this model.
     */
    virtual void setName(QString name);

    /**
     * Sets the block type
     * {@link #type}
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
     * changed. Emits the {@link updated()} signal to notify the views
     * to repaint. */
    void updatePerformed();

    /**
     * Returns the type of this block, eg. NIOS16-CPU
     */
    virtual QString type() const;

protected:
    QString name_;

private:
    QString type_;
    QString description_;
    uint id_;

signals:
    /**
     * Emitted when updatePerformed() was called.
     */
    virtual void updated();

};

#endif

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
 * $Id: pinmodel.h,v 1.30 2004/01/12 14:35:51 squig Exp $
 *
 *****************************************************************************/


#ifndef POA_PINMODEL_H
#define POA_PINMODEL_H

#include "abstractmodel.h"
#include "pinview.h"

#include <qcanvas.h>
#include <qdom.h>

class BlockModel;
class BlockView;
class ConnectorModel;
class QDomDocument;
class QDomElement;

/**
 * Stores the properties of a pin that is part of a {@link BlockModel}.
 */
class PinModel : public QObject
{
    Q_OBJECT

public:

    /**
     * Defines the type of this pin
     */
    enum PinType {INPUT, OUTPUT, EPISODIC};

    /**
     * Creates a new <code>PinModel</code>.
     * @param parent is the parent model.
     * @param id is its id.
     * @param name is its name.
     * @param address is its address.
     * @param bits is its bits.
     */
    PinModel(BlockModel *parent, const QString &name,
             unsigned address = 0, unsigned bits = 0,
             PinType type = INPUT, unsigned position = 0);

    /**
     * Creates a view for <code>PinModel</code> on <code>canvas</code> and de-
     * serializes the given xml subtree to set this' properties.
     * @param parent is the parent BlockModel
     */
    PinModel(BlockModel *parent, QDomElement pinElem);

    /**
     * Default destructor
     */
    virtual ~PinModel();

    /**
     * Returns this' parent
     */
    BlockModel *parent();

    /**
     * Sets the parent of this pin.
     */
    void setParent(BlockModel *parent);

    /**
     * Attach to another PinModel
     */
    void attach(PinModel *connectTo);

    /**
     * Detach from the attached PinModel
     */
    void detach();

    /**
     * Returns the connected pin.
     */
    PinModel *connected();

    /*
     * Returns if this pin is connectable to <code>toPin</code>.
     */
    bool isConnectable(PinModel *toPin);

    /**
     * Returns the type of this pin
     */
    PinType type();

    /**
     * Sets the type of this pin
     */
    void setType(PinType type);

    /**
     * Returns the position of the pin.
     * The position is set by the BlockConfDialog.
     */
    unsigned position();

    /**
     * Sets the position of the pin.
     * Used by the BlockModel
     */
    void setPosition(unsigned position);

    /**
     * Returns this' id.
     * The id is set by the BlockModel which contains this pin.
     */
    unsigned id();

    /**
     * Sets this' id.
     * Used by the BlockModel
     */
    void setId(const unsigned id);

    /**
     * Returns this' name
     */
    QString name();

    /**
     * Sets this' name
     */
    void setName(QString name);

    /**
     * Returns this pin's address on the cpld
     */
    unsigned int address();

    /**
     * Sets this pin's address on the cpld
     */
    void setAddress(unsigned int address);

    /**
     * Returns this pin's bit width on the cpld
     */
    unsigned int bits();

    /**
     * Sets this pin's bit width on the cpld
     */
    void setBits(unsigned int bits);

    /**
     * DON'T USE THIS, use createView(BlockView, DockPosition)
     * instead!
     */
    //    QCanvasItemList createView(QCanvas *canvas);

    /**
     * Creates the CanvasItems for this.
     */
    PinView *createView(BlockView *block,
            PinView::DockPosition dockPosition);


    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create
     * elements
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);

    /**
     * Clones this model.
     */
    PinModel *clone();

private:

    unsigned id_;
    unsigned position_;
    QString name_;
    BlockModel *parent_;
    unsigned address_;
    unsigned bits_;   // data type to be used in C source and width of pin
                      // one pin can be wider than one bit.
    PinModel *connected_;
    PinType type_;
    PinView *view_;

signals:
    /**
     * Emitted, when th pin is deleted.
     */
    void deleted();

    /**
     * Emitted, when the pin is detached.
     */
    void detached();

    /**
     * Emitted, when the pin is updated.
     */
    void updated();

};

#endif // POA_PINMODEL_H

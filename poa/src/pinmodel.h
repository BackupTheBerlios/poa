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
 * $Id: pinmodel.h,v 1.33 2004/02/16 16:24:01 squig Exp $
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
 * Represents a pin. Each pin is part of a BlockModel called the
 * parent. Pins are means for blocks to read or write data. Each pin
 * has an address that represents the memory location on the CPLD and
 * width that determines the number of bits of the data.
 *
 * Pins can be connected to other pins by invoking attach() which
 * means that data written to the pin is transfered to the connected
 * pin on the CPLD.
 *
 * PinModel objects are represented by PinView objects on screen.
 */
class PinModel : public QObject
{
    Q_OBJECT

public:

    /**
     * Defines the pin types.
     */
    enum PinType { INPUT, OUTPUT, EPISODIC };

    /**
     * Construct a pin.
     *
     * @param parent the parent block
     * @param name the name of the pin
     * @param address the address of the pin
     * @param bits the number of bits for the pin
     * @param type the type of the pin
     * @param position the position of the pin
     */
    PinModel(BlockModel *parent, const QString &name,
             unsigned address = 0, unsigned bits = 0,
             PinType type = INPUT, unsigned position = 0);

    /**
     * Constructs a pin, deserializing its properties from element.
     *
     * @param parent the parent block
     * @param element stores the properties
     */
    PinModel(BlockModel *parent, QDomElement element);

    virtual ~PinModel();

    /**
     * Returns the parent.
     */
    BlockModel *parent();

    /**
     * Returns the full qualified name.
     *
     * @return the name of the parent and the name of the pin
     */
    QString absName();

    /**
     * Attach to another PinModel.
     */
    void attach(PinModel *connectTo);

    /**
     * Detach from the attached PinModel.
     */
    void detach();

    /**
     * Returns the connected pin.
     */
    PinModel *connected();

    /*
     * Returns true, if this pin can be connected to target. The
     * return value depenends on the type of the pin and on the type
     * of target.
     *
     * The following connections are possible:
     *
     *  - Input <-> Output
     *  - Episodic <-> Output
     *  - Episodic <-> Input
     *  - Episodic <-> Episodic
     *
     * @return false, if the pin or target is already connected or
     * if the types of the pins do not match any of the conditions above
     */
    bool isConnectable(PinModel *target);

    /**
     * Returns the type of the pin
     */
    PinType type();

    /**
     * Sets the type of the pin
     */
    void setType(PinType type);

    /**
     * Returns the position of the pin. The position determines the
     * order in which the pins are displayed on the canvas. Lower
     * positions are shown above or left from higher
     * positions. Invoked by BlockConfDialog when the pins are
     * reordered.
     */
    unsigned position() const;

    /**
     * Sets the position of the pin.
     *
     * @see position()
     */
    void setPosition(unsigned position);

    /**
     * Returns parent-unique id. Used by BlockModel.
     */
    unsigned id();

    /**
     * Sets the parent-unique id. Invoked by BlockModel
     */
    void setId(const unsigned id);

    /**
     * Returns the name of the pin.
     */
    QString name();

    /**
     * Sets the name of the pin.
     */
    void setName(QString name);

    /**
     * Returns the address of the pin. The address determines the
     * memory location on the CPLD and is used by the CPU template
     * algorithm as the value for the corresponding variable.
     */
    unsigned int address();

    /**
     * Sets the address of the pin.
     *
     * @see #address()
     */
    void setAddress(unsigned int address);

    /**
     * Returns the number of bits of the pin, also called width of the
     * pin. Determines the data type for the corresponding variable to
     * be used in the C source.
     */
    unsigned int bits();

    /**
     * Sets the number of bits of the pin.
     */
    void setBits(unsigned int bits);

    /**
     * Creates the corresponding PinView object.
     *
     * @param block the parent view
     * @param dockPosition the edge of parent the pin is displayed at
     * @return the view object
     */
    PinView *createView(BlockView *block,
                        PinView::DockPosition dockPosition);


    /**
     * Serializes the pin to an xml subtree.
     *
     * @param document the parent document, needed to create elements.
     * @return the node, storing all persistent block properties
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Sets the properties of the pin from element.
     *
     * @param element stores the properties
     */
    void deserialize(QDomElement element);

    /**
     * Returns the tooltip text.
     */
    QString tip();

    /**
     * Needs to be invoked after the properties have been
     * changed. Emits the updated() signal to notify the views to
     * repaint.
     */
    void updatePerformed();

private:

    unsigned id_;
    unsigned position_;
    QString name_;
    BlockModel *parent_;
    unsigned address_;
    unsigned bits_;
    PinModel *connected_;
    PinType type_;

signals:
    /**
     * Emitted, before the pin is deleted.
     */
    void deleted();

    /**
     * Emitted, after the pin has been detached.
     */
    void detached();

    /**
     * Emitted, after the properties of the pin have changed.
     *
     * @see #updatePerformed()
     */
    void updated();

};

#endif // POA_PINMODEL_H

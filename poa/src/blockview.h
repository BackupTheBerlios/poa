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
 * $Id: blockview.h,v 1.48 2004/01/24 16:50:53 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKVIEW_H
#define POA_BLOCKVIEW_H

#include <qapplication.h>
#include <qcanvas.h>
#include <qdom.h>
#include <qfontmetrics.h>
#include <qobject.h>
#include <qptrlist.h>
#include <qvaluelist.h>

#include "copyable.h"
#include "moveable.h"
#include "removeable.h"
#include "serializable.h"
#include "tooltipable.h"
class AbstractModel;
class BlockModel;
class GridCanvas;
class PinModel;
class PinView;

/**
 * Block view for all blocks on a <code>GridCanvas</code>.
 * The block itself is a rectangle. Every pin that connects to the block
 * has its own <code>PinView</code>-instance. Those pin views are created
 * when this is created. When this moves on the canvas (by calling
 * <code>this->move</code> or <code>this->moveBy</code>) then all pin views
 * are moved as well. The pin views must not be moved separately.
 */
class BlockView: public QObject,
                 public QCanvasRectangle,
                 public Copyable,
                 public Moveable,
                 public Removeable,
                 public Serializable,
                 public Tooltipable
{
    Q_OBJECT

public:

    /**
     * Creates a view for <code>model</code> on <code>canvas</code>. Creates
     * also canvas view objects for all pins that connects to
     * <code>model</code>. Use the member {@link #addPinViewsTo} to get a
     * list of the pin views that were created.
     */
    BlockView(BlockModel *model, GridCanvas *canvas);

    /**
     * Default destructor
     */
    virtual ~BlockView();

    /**
     * Returns the model for <code>this</code>.
     */
    virtual AbstractModel *model();

    /**
     * Drags <code>this</code> and all attached pin views by the specified
     * amount
     */
    virtual QPoint dragBy(int dx, int dy);

    virtual QCanvasItem *item();

    /**
     * Moves <code>this</code> and the pin views that are connected to
     * <code>this</code>.
     * @param dx Number of pixels to move in horizontal direction
     * @param dy Number of pixels to move in vertical direction
     * @see #addPinViewsTo
     */
    virtual void moveBy(double dx, double dy);

    /**
     * Returns a list of all pins. The list is a copy, so
     * modifications of the list will have no effect on the BlockView.
     */
    QValueList<PinView *> pins();

    /**
     * Invoked when the item is removed by the user.
     */
    virtual void remove(Project *project);

    /**
     * Run Time Type Identification
     */
    virtual int rtti() const;
    static const int RTTI;

    /**
     * Sets the brush to red if Pin is selected, otherwise black
     */
    virtual void setSelected(bool yes);

    /**
     * Sets the z layer of the block and its pins to z.
     */
    virtual void setZ(double z);

    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create
     *        elements
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);

    /**
     * Returns the tooltip text.
     *
     * @see BlockModel#tip()
     */
    virtual QString tip();

public slots:
    void addPin(PinModel *);

    /**
     * Invoked by PinView#PinView() when a PinView object is instanciated.
     */
    void addPin(PinView *);
    void deletePinView(PinView *);
    void updateView();
    void deleteView();

protected:

    virtual void drawShape (QPainter &p);

    /**
     * Forces rerouting of all incident connector views.
     */
    virtual void arrangeConnectors();

    /**
     * Recalculates height and rearranges all pins.
     */
    virtual void arrangePins();

    /**
     * Alignes pins that ared docked on the bottom.
     */
    virtual void arrangeHorizontalPins();

    /**
     * Alignes all pins that dock onto <code>this</code> either on the left
     * side or on the right.
     */
    virtual void arrangeVerticalPins();

    /** default width of a block */
    static const int DEFAULT_WIDTH;
    /** default height of text drawn on the canvas */
    static int DEFAULT_FONT_HEIGHT;
    /** default vertical space between the top of the rectangle and the
     *  title */
    static const int DEFAULT_TOP_SPACING;
    /** default vertical space between
     * <li> the title and the separating horizontal line </li>
     * <li> the separating horizontal line and the pin list </li>
     * <li> the horizontal pin list and the vertical pin list </li>
     */
    static const int DEFAULT_HEADER_SPACING;
    /** default vertical space between the last pin name and the bottom of
     *  the rectangle */
    static const int DEFAULT_BOTTOM_SPACING;
    /** default vertical space between two consecutive pin names */
    static const int DEFAULT_PIN_SEPARATION;
    /** default horizontal space between the left border and the text */
    static const int DEFAULT_LEFT_BORDER;
    /** default horizontal space between the text and the right border */
    static const int DEFAULT_RIGHT_BORDER;
    /** Default space between two adjacent labels */
    static const int DEFAULT_LABEL_SPACING;
    /** Space per episodic pin label. */
    static const int DEFAULT_LABEL_WIDTH;

private:
    /**
     * Returns the vector that should contain pin.
     */
    QValueList<PinView*> *pinList(PinView *pin);

    /**
     * Sets the pen and brush according to the active and selection state.
     */
    void updateProperties();

    /**
     * Sorts list by pin position.
     */
    void sort(QValueList<PinView *> &list);

    BlockModel *model_;

    QValueList<PinView *> leftPins_;
    QValueList<PinView *> rightPins_;
    QValueList<PinView *> bottomPins_;

    void addPins(const QValueList<PinModel *>);
};

#endif // POA_BLOCKVIEW_H

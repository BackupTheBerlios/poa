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
 * $Id: blockview.h,v 1.36 2003/11/26 11:09:18 garbeam Exp $
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
#include "serializable.h"
#include "tooltipable.h"
class AbstractModel;
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
class BlockView: public QObject, public QCanvasRectangle, public Copyable,
                 public Moveable, public Serializable, public Tooltipable
{
    Q_OBJECT

public:

    /**
     * Creates a view for <code>model</code> on <code>canvas</code>. Creates
     * also canvas view objects for all pins that connects to
     * <code>model</code>. Use the member {@link #addPinViewsTo} to get a
     * list of the pin views that were created.
     */
    BlockView(AbstractModel *model, QCanvas *canvas);

    /**
     * Default destructor
     */
    virtual ~BlockView();

    /**
     * Returns a list of all canvas items that were created implicitely with
     * <code>this</code>.
     * @see #BlockView
     */
    void addPinViewsTo(QCanvasItemList &list);

    /**
     * Returns the model for <code>this</code>.
     */
    virtual AbstractModel *model();

    /**
     * @return true
     */
    virtual bool isDraggable();

    /**
     * Drags <code>this</code> and all attached pin views by the specified
     * amount
     */
    virtual QSize dragBy(double dx, double dy);

/*     /\** */
/*      * Activates a {@link MovingAction } for <code>view</code> that moves */
/*      * the BlockView. */
/*      *\/ */
/*     virtual void mousePressEvent(CanvasView *view, QMouseEvent *e); */

/*     /\** */
/*      * Opens the block model dialog for <code>view</code>. */
/*      *\/ */
/*     virtual void mouseDoubleClickEvent(CanvasView *view, QMouseEvent *e); */

    /**
     * Moves <code>this</code> and the pin views that are connected to
     * <code>this</code>.
     * @param dx Number of pixels to move in horizontal direction
     * @param dy Number of pixels to move in vertical direction
     * @see #addPinViewsTo
     */
    virtual void moveBy(double dx, double dy);

    /**
     * Run Time Type Identification
     */
    virtual int rtti() const;

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
    void addPins(const QPtrList<PinModel> *);
    void deletePinView(PinView *);
    void updateView();
    void deleteView();

protected:

    virtual void drawShape (QPainter &p);

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
    static const int DEFAULT_WIDTH = 70;
    /** default height of text drawn on the canvas */
    static int DEFAULT_FONT_HEIGHT;
    /** default vertical space between the top of the rectangle and the
     *  title */
    static const int DEFAULT_TOP_SPACING = 3;
    /** default vertical space between
     * <li> the title and the separating horizontal line </li>
     * <li> the separating horizontal line and the pin list </li>
     * <li> the horizontal pin list and the vertical pin list </li>
     */
    static const int DEFAULT_HEADER_SPACING = 4;
    /** default vertical space between the last pin name and the bottom of
     *  the rectangle */
    static const int DEFAULT_BOTTOM_SPACING = 4;
    /** default vertical space between two consecutive pin names */
    static const int DEFAULT_PIN_SEPARATION = 0;
    /** default horizontal space between the left border and the text */
    static const int DEFAULT_LEFT_BORDER = 3;
    /** default horizontal space between the text and the right border */
    static const int DEFAULT_RIGHT_BORDER = 5;
    /** Default space between two adjacent labels */
    static const int DEFAULT_LABEL_SPACING = 5;
    /** Space per episodic pin label. */
    static const int DEFAULT_LABEL_WIDTH = 40;

private:
    /**
     * Returns the vector that should contain pin.
     */
    QValueList<PinView*> *pinList(PinView *pin);

    /**
     * Sets the pen and brush according to the active and selection state.
     */
    void updateProperties();

    AbstractModel *model_;

    QValueList<PinView *> leftPins_;
    QValueList<PinView *> rightPins_;
    QValueList<PinView *> bottomPins_;

    static const int RTTI = 1000;
};

#endif // POA_BLOCKVIEW_H

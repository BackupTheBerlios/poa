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
 * $Id: blockview.h,v 1.10 2003/08/27 13:56:13 keulsn Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKVIEW_H
#define POA_BLOCKVIEW_H

#include <qcanvas.h>
#include <qvaluevector.h>

#include "abstractview.h"
class BlockModel;
class PinView;


/*****************************************************************************
 * Definition of a block view.
 */
class BlockView: public AbstractView, public QCanvasRectangle
{

public:

    BlockView(BlockModel *model, QCanvas *canvas);
    virtual ~BlockView();

    void addPinViewsTo(QCanvasItemList &list);

    BlockModel *model();
    void setModel(BlockModel *model);

    virtual void moveBy(double dx, double dy);

    virtual int rtti() const;

protected:

    virtual void drawShape (QPainter &p);

    virtual void arrangeVerticalPins();

    static const int DEFAULT_WIDTH = 150;
    static const int DEFAULT_FONT_HEIGHT = 12;
    static const int DEFAULT_TOP_SPACING = 3;
    static const int DEFAULT_HEADER_SPACING = 4;
    static const int DEFAULT_BOTTOM_SPACING = 4;
    static const int DEFAULT_PIN_SEPARATION = 2;
    static const int DEFAULT_LEFT_BORDER = 3;
    static const int DEFAULT_RIGHT_BORDER = 5;

private:

    BlockModel *model_;

    QValueVector<PinView*> leftPins_;
    QValueVector<PinView*> rightPins_;
    QValueVector<PinView*> bottomPins_;

    static const int RTTI = 1000;
};

#endif // POA_BLOCKVIEW_H

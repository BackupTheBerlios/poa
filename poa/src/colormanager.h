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
 * $Id: colormanager.h,v 1.7 2004/01/25 18:18:09 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_COLORMANAGER_H
#define POA_COLORMANAGER_H

#include "abstractmodel.h"
#include "blockmodel.h"
#include "moveable.h"
#include "serializable.h"

#include <qcanvas.h>
#include <qcolor.h>
#include <qmap.h>
#include <qptrlist.h>

class Palette;
class SortedBlockList;

/**
 * Provides a color manager and a palette widget.
 *
 * This class manages the association between block's clock value and colors.
 * For each clock value in the project (per canvas), the color manager
 * provides an individual color.
 *
 * The palette widget displays a vertical box and displays a legend for
 * the color -> nanoseconds mapping.
 */
class ColorManager : public QObject,
                     public QCanvasRectangle,
                     public Moveable,
                     public Serializable
{
    Q_OBJECT
 public:

    /**
     * Instanciates a color manager.
     */
    ColorManager(QCanvas *canvas, Palette *palette);

    /**
     * Default destructor.
     */
    ~ColorManager();

    /**
     * Returns the color of the given block. The color depends on the
     * block's clock value.
     */
    QColor color(const BlockModel *model, int luminance = 100);

    /**
     * Returns the 'normal' color of the given model.
     * Calls color(BlockModel*) if the given model represents a block.
     * In all others cases, the default settings will be used.
     */
    QColor color(AbstractModel *model, int luminance = 100);

    /**
     * Returns activated color of the given model.
     *  - Returns the default settings.
     */
    QColor activatedColor(AbstractModel *model, int luminance = 100);

    /**
     * Returns selected color of the given model.
     *  - Returns the default settings.
     */
    QColor selectedColor(AbstractModel *model, int luminance = 100);

    /**
     * \copydoc Moveable::item()
     */
    virtual QCanvasItem *item();

    /**
     * \copydoc Moveable::dragBy(int, int);
     */
    virtual QPoint dragBy(int dx, int dy);

    /**
     * \copydoc Serializable::serialize(QDomDocument)
     */
    virtual QDomElement serialize(QDomDocument *document);

    /**
     * \copydoc Serializable::deserialize(QDomElement)
     */
    virtual void deserialize(QDomElement element);

    /**
     * Draws the palette widget
     */
    void drawShape(QPainter &p);

    /**
     * Return own type information
     */
    virtual int rtti() const;
    static const int RTTI;

 private:

    /**
     * Updates the widget size when new legend entries were added.
     */
    void updateWigetSize(int clock);

    /** current palette */
    Palette *palette_;

    /** maps nanoseconds to a palette entry */
    QMap<int, int> nsToPalIndex_;

    /** holds all registered block models */
    QPtrList<BlockModel> models_;

    /** default height of text drawn on the canvas */
    static int FONT_HEIGHT;

    /** points to the next palette entry */
    int palPosition_;

 private slots:

     /**
      * Updates the ns-palette mapping
      */
     void updateMap();

    /**
     * Handles model's delete signal.
     */
     void deleteModel(BlockModel *);
};


/**
 * Provides a palette implementation.
 * Contains QColor elements.
 */
class Palette
{

 public:

    /**
     * Instanciates a palette.
     */
    Palette(QString name);

    /**
     * Returns the name of the palette.
     */
    QString name();

    /**
     * Returns the color for the given index.
     */
    QColor color(int index) const;

    /**
     * Returns the size of the palette.
     */
    int size();

    /**
     * Adds a color to the palette.
     */
    void addColor(const QColor color);


    /**
     * Returns a static palette (strong colors).
     */
    static Palette *createStrongPalette();

    /**
     * Returns a static palette (light colors).
     */
    static Palette *createLightPalette();


 private:

    /** the name of the palette. */
    QString name_;

    /** the palette */
    QValueList<QColor> colorList_;

};


#endif // POA_COLORMANAGER_H

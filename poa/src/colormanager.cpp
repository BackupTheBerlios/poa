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
 * $Id: colormanager.cpp,v 1.9 2004/01/25 18:18:09 vanto Exp $
 *
 *****************************************************************************/

#include "colormanager.h"
#include "settings.h"

#include <qpainter.h>

const int ColorManager::RTTI = 1003;
const int VSPACE = 5;
const int HSPACE = 5;
const int SAMPLE_SIZE = 12;
const QString LEGEND_TITLE = "Clocks";
int ColorManager::FONT_HEIGHT = 12;


ColorManager::ColorManager(QCanvas *canvas, Palette *palette)
    : QCanvasRectangle(canvas),
      palette_(palette),
      palPosition_(0)
{
    setBrush(white);
    FONT_HEIGHT = QFontMetrics(QApplication::font()).height();
}

ColorManager::~ColorManager()
{
}

QColor ColorManager::color(const BlockModel *model, int luminance)
{
    if (!nsToPalIndex_.contains(model->clock())) {
        // insert model into modellist
        //        models_->inSort(model);
        models_.append(model);

        // connect to update event
        connect(model, SIGNAL(updated()),
                this, SLOT(updateMap()));

        // connect to delete event
        connect(model, SIGNAL(deleted(BlockModel *)),
                this, SLOT(deleteModel(BlockModel *)));

        // update mapping and widget size
        updateMap();
    }

    return palette_->color(nsToPalIndex_[model->clock()]).light(luminance);
}

QColor ColorManager::color(AbstractModel *model, int luminance)
{
    BlockModel *bm = dynamic_cast<BlockModel*>(model);
    if ((bm != 0) && (bm->clock() != 0)) {
        return color(bm, luminance);

    }

    return Settings::instance()->defaultBrushColor();
}

QColor ColorManager::activatedColor(AbstractModel*, int)
{
    return Settings::instance()->activatedColor();
}

QColor ColorManager::selectedColor(AbstractModel*, int)
{
    return Settings::instance()->selectedColor();
}

void ColorManager::updateMap()
{
    BlockModel *model;

    int width = QFontMetrics(QApplication::font()).width(tr(LEGEND_TITLE));

    // actually existing clocks
    QValueList<int> reqNs;

    for (model = models_.first(); model; model = models_.next()) {

        // rotate palette
        if (palPosition_ > palette_->size()) {
            palPosition_ = 0;
        }

        reqNs += model->clock();

        // insert mapping
        if (!nsToPalIndex_.contains(model->clock())) {
            nsToPalIndex_.insert(model->clock(), palPosition_++);
        }

        // calculate max width
        width = QMAX(width, QFontMetrics(QApplication::font())
                     .width(QString::number(model->clock()) + " ns")
                     + SAMPLE_SIZE + (3 * HSPACE));
    }

    // remove unused map entries
    for (QMap<int,int>::iterator it = nsToPalIndex_.begin();
         it != nsToPalIndex_.end(); it++) {
        if (!reqNs.contains(it.key())) {
            nsToPalIndex_.remove(it);
        }
    }

    // calculate height
    int height = ((nsToPalIndex_.count() + 1) * (FONT_HEIGHT + VSPACE))
        + (2* VSPACE);

    setSize(width, height);
    update();
    canvas()->update();
}

void ColorManager::deleteModel(BlockModel *block)
{
    models_.remove(block);
    updateMap();
}

QPoint ColorManager::dragBy(int dx, int dy)
{
    // must convert to be consistent with return value
    moveBy(dx, dy);
    return QPoint(dx, dy);
}

QCanvasItem *ColorManager::item()
{
    return this;
}

int ColorManager::rtti() const
{
    return ColorManager::RTTI;
}

QDomElement ColorManager::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("palette-view");
    for (QMap<int,int>::const_iterator it = nsToPalIndex_.begin();
         it != nsToPalIndex_.end(); it++) {
        QDomElement mapping = document->createElement("mapping");
        mapping.setAttribute("ns", it.key());
        mapping.setAttribute("pal-entry", it.data());
        root.appendChild(mapping);
    }
    root.setAttribute("name", palette_->name());
    return root;
}

void ColorManager::deserialize(QDomElement element)
{
    int width = QFontMetrics(QApplication::font()).width(tr(LEGEND_TITLE));

    // pal entries
    QDomNodeList mappingElements = element.elementsByTagName("mapping");
    for (unsigned int k = 0; k < mappingElements.count(); k++) {
        QDomElement mappingElement = mappingElements.item(k).toElement();
        if (mappingElement.attribute("ns", "0").toUInt() != 0) {
            nsToPalIndex_.insert(mappingElement.attribute("ns", "0").toUInt(),
                                 mappingElement.attribute("pal-entry", "0")
                                 .toUInt());

            // calculate max width
            width = QMAX(width, QFontMetrics(QApplication::font())
                         .width(mappingElement.attribute("ns", "0") + " ns")
                         + SAMPLE_SIZE + (3 * HSPACE));

        }
    }

    // calculate height
    int height = ((nsToPalIndex_.count() + 1) * (FONT_HEIGHT + VSPACE))
        + (2* VSPACE);

    setSize(width, height);
    update();
    //canvas()->update();

    // position
    moveBy(element.attribute("x", "0").toUInt(),
           element.attribute("y", "0").toUInt());
}

void ColorManager::drawShape(QPainter &p)
{
    QCanvasRectangle::drawShape(p);

    QRect textArea((int)x(),
                   (int)y(),
                   width(),
                   FONT_HEIGHT);

    QFont f = p.font();

    f.setBold(true);
    p.setFont(f);
    p.drawText(textArea, QObject::AlignHCenter, tr(LEGEND_TITLE));

    f.setBold(false);
    p.setFont(f);

    int ly = (int)y() + FONT_HEIGHT + VSPACE + VSPACE;
    int lx = (int)x() + HSPACE;

    for (QMap<int,int>::const_iterator it = nsToPalIndex_.begin();
         it != nsToPalIndex_.end(); it++) {

        p.fillRect(lx,
                   ly,
                   SAMPLE_SIZE,
                   SAMPLE_SIZE,
                   QBrush(palette_->color(it.data())));

        textArea = QRect(lx + SAMPLE_SIZE + HSPACE,
                         ly,
                         width() - HSPACE,
                         ly + FONT_HEIGHT);
        p.drawText(textArea, QObject::AlignLeft,
                  QString::number(it.key()) + " ns");

        ly += FONT_HEIGHT + VSPACE;
    }
}


/* ------------------------------------------------------------------------- */

Palette::Palette(QString name)
    : name_(name)
{
}

QString Palette::name()
{
    return name_;
}

QColor Palette::color(int index) const
{
    return colorList_[index];
}

int Palette::size()
{
    return colorList_.count();
}

void Palette::addColor(const QColor color)
{
    colorList_.append(color);
}

Palette *Palette::createStrongPalette()
{
    Palette *palette = new Palette("Strong Palette");

    /*  stronger colors */
    palette->addColor(QColor("#FF0000"));
    palette->addColor(QColor("#FFA500"));
    palette->addColor(QColor("#FFFF00"));
    palette->addColor(QColor("#008000"));
    palette->addColor(QColor("#0000FF"));
    palette->addColor(QColor("#800080"));
    palette->addColor(QColor("#7FFFDF"));
    palette->addColor(QColor("#8A2BE2"));
    palette->addColor(QColor("#A52A2A"));
    palette->addColor(QColor("#DEB887"));
    palette->addColor(QColor("#5F9EA0"));
    palette->addColor(QColor("#7FFF00"));
    palette->addColor(QColor("#D2691E"));
    palette->addColor(QColor("#FF7F50"));
    palette->addColor(QColor("#DC143C"));

    return palette;
}

Palette *Palette::createLightPalette()
{
    Palette *palette = new Palette("Light Palette");

    /* lighter colors */
    palette->addColor(QColor("#ADD8E6")); // light blue
    palette->addColor(QColor("#F08080")); // light coral
    palette->addColor(QColor("#E0FFFF")); // light cyan
    palette->addColor(QColor("#FAFAD2")); // light goldenrod yellow
    palette->addColor(QColor("#90EE90")); // light green
    palette->addColor(QColor("#FFb6C1")); // light pink
    palette->addColor(QColor("#FFA07A")); // light salmon
    palette->addColor(QColor("#20B2AA")); // light sea green
    palette->addColor(QColor("#87CEFA")); // light sky blue
    palette->addColor(QColor("#778899")); // light slate gray
    palette->addColor(QColor("#B0C4DE")); // light steel blue
    palette->addColor(QColor("#FFFFE0")); // light yellow
    palette->addColor(QColor("#F5DEB3")); // wheat

    return palette;
}

/* ------------------------------------------------------------------------- */

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
 * $Id: colormanager.cpp,v 1.18 2004/01/27 20:22:40 kilgus Exp $
 *
 *****************************************************************************/

#include "colormanager.h"
#include "settings.h"

#include <qintdict.h>
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
    Q_ASSERT(palette != 0);

    setBrush(white);
    FONT_HEIGHT = QFontMetrics(QApplication::font()).height();
    Q_ASSERT(FONT_HEIGHT >= SAMPLE_SIZE);
}

ColorManager::~ColorManager()
{
    delete palette_;
}

QColor ColorManager::color(AbstractModel *model)
{
    BlockModel *bm = dynamic_cast<BlockModel*>(model);

    if (bm == 0) {
        return Settings::instance()->defaultBrushColor();
    }

    if (!models_.contains(bm)) {
        // insert model into modellist
        //        models_->inSort(model);
        models_.append(bm);

        // connect to update event
        connect(bm, SIGNAL(updated()),
                this, SLOT(updateMap()));

        // connect to delete event
        connect(bm, SIGNAL(deleted(BlockModel *)),
                this, SLOT(deleteModel(BlockModel *)));
    }

    if (bm->clock() == 0) {
        return Settings::instance()->defaultBrushColor();
    }
    else {
        // insert mapping
        if (!nsToPalIndex_.contains(bm->clock())) {
            // rotate palette
            if (palPosition_ >= palette_->size()) {
                palPosition_ = 0;
            }

            nsToPalIndex_.insert(bm->clock(), palPosition_++);

            // update widget size
            recalculateSize();
            canvas()->update();
        }
        return palette_->color(nsToPalIndex_[bm->clock()]);
    }
}

QColor ColorManager::activatedColor(AbstractModel*)
{
    return Settings::instance()->activatedColor();
}

QColor ColorManager::selectedColor(AbstractModel*)
{
    return Settings::instance()->selectedColor();
}

void ColorManager::updateMap()
{
    // actually existing clocks
    QIntDict<int> reqNs;
    int value = 1;
    for (BlockModel *model = models_.first(); model; model = models_.next()) {
        reqNs.insert(model->clock(), &value);
    }

    // remove unused map entries
    QMap<int,int>::iterator it = nsToPalIndex_.begin();
    while (it != nsToPalIndex_.end()) {
        // Advance iterator NOW because we might want to delete the current entry
		// Otherwise the iterator gets undefined! (crash under Windows)
        QMap<int,int>::iterator it2 = it;
        it++;
        if (reqNs.find(it2.key()) == 0) {
            nsToPalIndex_.remove(it2);
        }
    }

    recalculateSize();
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

void ColorManager::recalculateSize()
{
    QFont headerFont = QApplication::font();
    headerFont.setBold(true);
    int width = QFontMetrics(headerFont).width(LEGEND_TITLE) + 2 * HSPACE;

    QFontMetrics metrics(QApplication::font());
    for (QMap<int,int>::iterator it = nsToPalIndex_.begin();
         it != nsToPalIndex_.end(); it++) {

        // calculate max width
        width = QMAX(width, metrics.width(QString::number(*it) + " ns")
                     + SAMPLE_SIZE + 4 * HSPACE);

    }

    // calculate height
    int height = (nsToPalIndex_.count() + 1) * (FONT_HEIGHT + VSPACE)
        + 2 * VSPACE;

    setSize(width, height);
    update();
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
    // pal entries
    QDomNodeList mappingElements = element.elementsByTagName("mapping");
    for (unsigned int k = 0; k < mappingElements.count(); k++) {
        QDomElement mappingElement = mappingElements.item(k).toElement();
        if (mappingElement.attribute("ns", "0").toUInt() != 0) {
            nsToPalIndex_.insert(mappingElement.attribute("ns", "0").toUInt(),
                                 mappingElement.attribute("pal-entry", "0")
                                 .toUInt());
        }
    }

    // position
    moveBy(element.attribute("x", "0").toUInt(),
           element.attribute("y", "0").toUInt());

    recalculateSize();
}

void ColorManager::drawShape(QPainter &p)
{
    QCanvasRectangle::drawShape(p);

    QRect textArea((int)x(),
                   (int)y() + VSPACE,
                   width(),
                   FONT_HEIGHT);

    QFont f = QApplication::font();

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
                   ly + ((FONT_HEIGHT - SAMPLE_SIZE) / 2),
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

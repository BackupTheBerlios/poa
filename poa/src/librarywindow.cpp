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
 * $Id: librarywindow.cpp,v 1.31 2004/01/20 19:13:07 squig Exp $
 *
 *****************************************************************************/
#include "librarywindow.h"

#include "abstractmodel.h"
#include "cpumodel.h"
#include "modelfactory.h"
#include "muxmodel.h"
#include "pinmodel.h"
#include "util.h"

#include <qapplication.h>
#include <qdir.h>
#include <qdom.h>
#include <qdragobject.h>
#include <qfileinfo.h>
#include <qframe.h>
#include <qlayout.h>
#include <qpalette.h>
#include <qsplitter.h>
#include <qtextbrowser.h>
#include <qvariant.h>

/**
 * Constructs the window.
 */
LibraryWindow::LibraryWindow(Place p, QWidget* parent, const char* name,
                             WFlags f)
    : QDockWindow(p, parent, name, f)
{
    modified_ = false;

    setCloseMode(Never);
    setMovingEnabled(TRUE);
    setResizeEnabled(TRUE);

    splitter_ = new QSplitter(Qt::Vertical, this);
    setWidget(splitter_);

    modelListView_ = new LibraryListView(splitter_);
    modelListView_->addColumn(tr("Module"));

    descriptionTextBrowser_ = new QTextBrowser(splitter_);
    QPalette palette = QApplication::palette();
    descriptionTextBrowser_->setFrameStyle(QFrame::NoFrame);
    descriptionTextBrowser_->setPaper
            (palette.brush(QPalette::Normal, QColorGroup::Background));

    initialize();

    connect(modelListView_, SIGNAL(selectionChanged(QListViewItem *)),
            this, SLOT(setDescription(QListViewItem *)));
    connect(this, SIGNAL(orientationChanged(Orientation)),
            this, SLOT(setOrientation(Orientation)));
}

LibraryWindow::~LibraryWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void LibraryWindow::add(AbstractModel *model)
{
    new LibraryListViewItem(getTypeItem(model->type()), model);
    modified_ = true;

}

void LibraryWindow::addDefaultItems()
{
    BlockModel *model;

    // cpu
    model = new CpuModel("CPU", "Default CPU");
    model->addPin(new PinModel(model, "in1", 0, 32, PinModel::INPUT, 1));
    model->addPin(new PinModel(model, "out1", 0, 32, PinModel::OUTPUT, 1));
    model->addPin(new PinModel(model, "clock", 0, 32, PinModel::EPISODIC, 1));
    model->addPin(new PinModel(model, "reset", 0, 32, PinModel::EPISODIC, 2));
    add(model);

    // core
    model = new BlockModel("Core", "Default core");
    model->addPin(new PinModel(model, "in1", 0, 32, PinModel::INPUT, 1));
    model->addPin(new PinModel(model, "out1", 0, 32, PinModel::OUTPUT, 1));
    add(model);

    // input
    model = new BlockModel("Input", "Default input block");
    model->setHasEpisodicPins(false);
    model->setHasInputPins(false);
    model->setHasRuntime(false);
    model->addPin(new PinModel(model, "out1", 0, 32, PinModel::OUTPUT, 1));
    model->addPin(new PinModel(model, "out2", 32, 32, PinModel::OUTPUT, 2));
    model->addPin(new PinModel(model, "out3", 64, 32, PinModel::OUTPUT, 3));
    add(model);

    // output
    model = new BlockModel("Output", "Default output block");
    model->addPin(new PinModel(model, "in1", 0, 32, PinModel::INPUT, 1));
    model->addPin(new PinModel(model, "in2", 32, 32, PinModel::INPUT, 2));
    model->addPin(new PinModel(model, "in3", 64, 32, PinModel::INPUT, 3));
    add(model);

    // mux
    MuxModel *muxModel = new MuxModel("Mux", "Multiplexer / Demultiplexer");
    PinModel *in1 = new PinModel(muxModel, "in1", 0, 32, PinModel::INPUT, 1);
    PinModel *in2 = new PinModel(muxModel, "in2", 32, 32, PinModel::INPUT, 2);
    PinModel *out1 = new PinModel(muxModel, "out1", 0, 64, PinModel::OUTPUT, 1);
    muxModel->addPin(in1);
    muxModel->addPin(in2);
    muxModel->addPin(out1);
    muxModel->addMuxMapping(new MuxMapping(in1, out1, 0, 32, 0, 32));
    muxModel->addMuxMapping(new MuxMapping(in2, out1, 0, 32, 32, 64));
    add(muxModel);
}

QListViewItem *LibraryWindow::getTypeItem(QString type)
{
    QListViewItem *item = typeItemByType.find(type);
    if (item == 0) {
        item = new QListViewItem(modelListView_, type);
        item->setOpen(true);
        typeItemByType.insert(type, item);
    }
    return item;
}

void LibraryWindow::initialize()
{
    // read library file
    QString filename = Util::findResource("library.xml");
    if (!filename.isNull()) {
        QFile file(filename);
        if (file.exists()) {
            open(&file);
        }
    }

    addDefaultItems();

    modified_ = false;
}


void LibraryWindow::open(QFile *file)
{
    // FIX: this is copy & paste code from project.cpp
    if (file->open(IO_ReadOnly)) {
        QDomDocument document;
        if (document.setContent(file)) {
            QDomNodeList mList = document.elementsByTagName("model");
            if (mList.count() == 1) {
                QDomNodeList bList
                    = mList.item(0).toElement().elementsByTagName("blocks");
                if (bList.count() == 1) {
                    // create model instances
                    QDomElement element = bList.item(0).toElement();
                    QValueList<AbstractModel *> l
                        = ModelFactory::generate(element);
                    for (QValueList<AbstractModel *>::Iterator it = l.begin();
                         it != l.end(); ++it) {
                        add(*it);
                    }
                }
            }
        }
        file->close();
    }
}

void LibraryWindow::save(QFile *file)
{
    if (file->open(IO_WriteOnly)) {
        file->close();
    }
}

void LibraryWindow::setDescription(QListViewItem* item)
{
    descriptionTextBrowser_->setText(item->text(1));
}

void LibraryWindow::setOrientation(Qt::Orientation orientation)
{
    splitter_->setOrientation(orientation);
}

LibraryListView::LibraryListView(QWidget *parent, const char *name,
                                 WFlags f)
    : QListView(parent, name, f)
{
}

QDragObject *LibraryListView::dragObject()
{
    LibraryListViewItem *item
        = reinterpret_cast<LibraryListViewItem*>(selectedItem());

    QDomDocument doc;
    QDomElement root = doc.createElement("model");
    doc.appendChild(root);
    root.appendChild(item->data().serialize(&doc));
    QStoredDrag *dragItem = new QStoredDrag("text/xml", this);
    dragItem->setEncodedData(doc.toCString());
    return dragItem;
}

LibraryListViewItem::LibraryListViewItem(QListViewItem *parent,
                                         AbstractModel *item)
    : QListViewItem(parent), item_(item)
{
    setText(0, item->type());
    setText(1, item->description());

    setDragEnabled(TRUE);
}

LibraryListViewItem::~LibraryListViewItem()
{
    delete item_;
}

AbstractModel &LibraryListViewItem::data() const
{
    return *item_;
}

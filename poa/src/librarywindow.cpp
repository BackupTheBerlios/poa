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
 * $Id: librarywindow.cpp,v 1.26 2003/12/20 15:51:05 kilgus Exp $
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
    setCloseMode(Never);
    setMovingEnabled(TRUE);
    setResizeEnabled(TRUE);

    splitter = new QSplitter(Qt::Vertical, this);
    setWidget(splitter);

    moduleListView = new LibraryListView(splitter);
    moduleListView->addColumn(tr("Module"));

    cpuListViewItem = new QListViewItem(moduleListView, tr("CPU"));
    cpuListViewItem->setOpen(TRUE);
    coreListViewItem = new QListViewItem(moduleListView, tr("Core"));
    coreListViewItem->setOpen(TRUE);
    ioListViewItem = new QListViewItem(moduleListView, tr("I/O"));
    ioListViewItem->setOpen(TRUE);
    muxListViewItem = new QListViewItem(moduleListView, tr("Mux"));
    muxListViewItem->setOpen(TRUE);

    descriptionTextBrowser = new QTextBrowser(splitter);
    QPalette palette = QApplication::palette();
    descriptionTextBrowser->setFrameStyle(NoFrame);
    descriptionTextBrowser->setPaper
            (palette.brush(QPalette::Normal, QColorGroup::Background));

    initializeLibrary();

    connect(moduleListView, SIGNAL(selectionChanged(QListViewItem *)),
            this, SLOT(setDescription(QListViewItem *)));
    connect(this, SIGNAL(orientationChanged(Orientation)),
            this, SLOT(setOrientation(Orientation)));
}

/**
 * Destroys the window.
 */
LibraryWindow::~LibraryWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * Adds the modules to the tree.
 */
void LibraryWindow::initializeLibrary()
{
    BlockModel *model;
    PinModel *pin;

//      // NIOS 32
//      BlockModel *model = new CpuModel("NIOS 32", "NIOS 32-bit CPU");
//      PinModel *pin = new PinModel(model, "in1");
//      pin->setType(PinModel::INPUT);
//      model->addPin(pin);
//      pin = new PinModel(model, "out1");
//      pin->setType(PinModel::OUTPUT);
//      model->addPin(pin);
//      pin = new PinModel(model, "clock");
//      pin->setType(PinModel::EPISODIC);
//      model->addPin(pin);
//      pin = new PinModel(model, "reset");
//      pin->setType(PinModel::EPISODIC);
//      model->addPin(pin);
//      new LibraryListViewItem(cpuListViewItem, model);

    // NIOS 16
    model = new CpuModel("CPU", "Default CPU");
    pin = new PinModel(model, "in1");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "out1");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    pin = new PinModel(model, "clock");
    pin->setType(PinModel::EPISODIC);
    model->addPin(pin);
    pin = new PinModel(model, "reset");
    pin->setType(PinModel::EPISODIC);
    model->addPin(pin);
    new LibraryListViewItem(cpuListViewItem, model);

    // core
    model = new BlockModel("Core", "Default core");
    pin = new PinModel(model, "in1");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "out1");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    new LibraryListViewItem(coreListViewItem, model);

    // input
    model = new BlockModel("Input", "Default input block");
    model->setHasEpisodicPins(false);
    model->setHasInputPins(false);
    model->setHasRuntime(false);
    pin = new PinModel(model, "out1");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    pin = new PinModel(model, "out2");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    pin = new PinModel(model, "out3");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    new LibraryListViewItem(ioListViewItem, model);

    // output
    model = new BlockModel("Output", "Default output block");
    model->setHasEpisodicPins(false);
    model->setHasOutputPins(false);
    model->setHasRuntime(false);
    pin = new PinModel(model, "in1");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "in2");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "in3");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    new LibraryListViewItem(ioListViewItem, model);

    // mux
    new LibraryListViewItem(muxListViewItem,
                            new MuxModel("Mux", "Multiplexer"));
    new LibraryListViewItem(muxListViewItem,
                            new MuxModel("Demux", "Demultiplexer"));

    // read library file
    // FIX: this is copy & paste code from project.cpp
    QString filename = Util::findResource("library.xml");
    if (!filename.isNull()) {
        QFile file(filename);
        if (file.open(IO_ReadOnly)) {
            QDomDocument document;
            if (document.setContent(&file)) {
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
                            // FIX: should sort items into appropriate category
                            new LibraryListViewItem(coreListViewItem, *it);
                        }
                    }
                }
            }
            file.close();
        }
    }

}

/**
 * Sets the description
 */
void LibraryWindow::setDescription(QListViewItem* item)
{
    descriptionTextBrowser->setText(item->text(1));
}

/**
 * Sets the orientation of the splitter to the reverse value of orientation.
 */
void LibraryWindow::setOrientation(Qt::Orientation orientation)
{
    splitter->setOrientation(orientation);
}

LibraryListView::LibraryListView(QWidget *parent, const char *name,
                                 WFlags f)
    : QListView(parent, name, f)
{
}

QDragObject *LibraryListView::dragObject()
{
    LibraryListViewItem *item = reinterpret_cast<LibraryListViewItem*>(selectedItem());

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

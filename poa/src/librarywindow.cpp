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
 * $Id: librarywindow.cpp,v 1.18 2003/09/28 21:52:11 squig Exp $
 *
 *****************************************************************************/
#include "librarywindow.h"

#include "abstractmodel.h"
#include "cpumodel.h"
#include "coremodel.h"
#include "inputmodel.h"
#include "muxmodel.h"
#include "outputmodel.h"
#include "pinmodel.h"

#include <qvariant.h>
#include <qdragobject.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qtextbrowser.h>

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
    // NIOS 32
    BlockModel *model = new CpuModel("NIOS 32", "NIOS 32-bit CPU");
    PinModel *pin = new PinModel(model, "in1");
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

    // NIOS 16
    model = new CpuModel("NIOS 16", "NIOS 16-bit CPU");
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

    // full adder
    model = new CoreModel("Full Adder", "Fast binary adder");
    pin = new PinModel(model, "in1");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "in2");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "result");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    new LibraryListViewItem(coreListViewItem, model);

    // input
    model = new InputModel("Input", "Static input block");
    pin = new PinModel(model, "in1");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "in2");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    pin = new PinModel(model, "in3");
    pin->setType(PinModel::INPUT);
    model->addPin(pin);
    new LibraryListViewItem(coreListViewItem, model);

    // output
    model = new OutputModel("Output", "Static output block");
    pin = new PinModel(model, "out1");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    pin = new PinModel(model, "out2");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    pin = new PinModel(model, "out3");
    pin->setType(PinModel::OUTPUT);
    model->addPin(pin);
    new LibraryListViewItem(coreListViewItem, model);

    // mux
    new LibraryListViewItem(muxListViewItem,
                            new MuxModel("Mux", "Multiplexer"));
    new LibraryListViewItem(muxListViewItem,
                            new MuxModel("Demux", "Demultiplexer"));
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
    LibraryListViewItem *item = (LibraryListViewItem *)selectedItem();

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

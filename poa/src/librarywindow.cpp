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
 * $Id: librarywindow.cpp,v 1.42 2004/06/04 15:41:15 kilgus Exp $
 *
 *****************************************************************************/
#include "librarywindow.h"

#include "abstractmodel.h"
#include "cpumodel.h"
#include "modelfactory.h"
#include "muxmodel.h"
#include "pinmodel.h"
#include "settings.h"
#include "util.h"

#include <qapplication.h>
#include <qdir.h>
#include <qdom.h>
#include <qdragobject.h>
#include <qfileinfo.h>
#include <qframe.h>
#include <qinputdialog.h>
#include <qlayout.h>
#include <qpalette.h>
#include <qpopupmenu.h>
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
    modelListView_->setDefaultRenameAction(QListView::Accept);

    descriptionTextBrowser_ = new QTextBrowser(splitter_);
    QPalette palette = QApplication::palette();
    descriptionTextBrowser_->setFrameStyle(QFrame::NoFrame);
    descriptionTextBrowser_->setPaper
            (palette.brush(QPalette::Normal, QColorGroup::Background));

    popupMenu_ = new QPopupMenu();
    popupMenu_->insertItem(tr("Rename"), this, SLOT(renameSelected()));
    popupMenu_->insertItem(tr("Change Type"),
                           this, SLOT(changeTypeOfSelected()));
    popupMenu_->insertSeparator();
    popupMenu_->insertItem(QPixmap(Util::findIcon("editdelete.png")),
                           tr("Remove"), this, SLOT(removeSelected()));

    connect(modelListView_, SIGNAL(selectionChanged(QListViewItem *)),
            this, SLOT(setDescription(QListViewItem *)));
    connect(this, SIGNAL(orientationChanged(Orientation)),
            this, SLOT(setOrientation(Orientation)));
    connect(modelListView_,
            SIGNAL(contextMenuRequested(QListViewItem *, const QPoint &, int)),
            this, SLOT(showPopup(QListViewItem *, const QPoint &, int)));
    connect(modelListView_, SIGNAL(selectionChanged(QListViewItem *)),
            this, SLOT(selectionChanged(QListViewItem *)));

    // load items
    initialize();
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
    model = new CpuModel("CPU", tr("Default CPU"));
    model->addPin(new PinModel(model, "in1", 0, 32, PinModel::INPUT, 1));
    model->addPin(new PinModel(model, "out1", 0, 32, PinModel::OUTPUT, 1));
    model->addPin(new PinModel(model, "clk", 0, 32, PinModel::EPISODIC, 1));
    model->addPin(new PinModel(model, "reset", 0, 32, PinModel::EPISODIC, 2));
    add(model);

    // core
    model = new BlockModel("Core",tr("Default core"));
    model->addPin(new PinModel(model, "in1", 0, 32, PinModel::INPUT, 1));
    model->addPin(new PinModel(model, "out1", 0, 32, PinModel::OUTPUT, 1));
    add(model);

    // input
    model = new BlockModel("Input", tr("Default input block"));
    model->setType("I/O");
    model->setHasEpisodicPins(false);
    model->setHasInputPins(false);
    model->setRuntime(1);
    model->addPin(new PinModel(model, "out1", 0, 32, PinModel::OUTPUT, 1));
    model->addPin(new PinModel(model, "out2", 32, 32, PinModel::OUTPUT, 2));
    model->addPin(new PinModel(model, "out3", 64, 32, PinModel::OUTPUT, 3));
    add(model);

    // output
    model = new BlockModel("Output", tr("Default output block"));
    model->setType("I/O");
    model->setHasEpisodicPins(false);
    model->setHasOutputPins(false);
    model->setRuntime(1);
    model->addPin(new PinModel(model, "in1", 0, 32, PinModel::INPUT, 1));
    model->addPin(new PinModel(model, "in2", 32, 32, PinModel::INPUT, 2));
    model->addPin(new PinModel(model, "in3", 64, 32, PinModel::INPUT, 3));
    add(model);

    // mux
    MuxModel *muxModel = new MuxModel("Mux", tr("Multiplexer / Demultiplexer"));
    PinModel *in1 = new PinModel(muxModel, "in1", 0, 32, PinModel::INPUT, 1);
    PinModel *in2 = new PinModel(muxModel, "in2", 32, 32, PinModel::INPUT, 2);
    PinModel *out1 = new PinModel(muxModel, "out1", 0, 64, PinModel::OUTPUT, 1);
    muxModel->addPin(in1);
    muxModel->addPin(in2);
    muxModel->addPin(out1);
    muxModel->addMuxMapping(new MuxMapping(in1, out1, 0, 32, 0, 32));
    muxModel->addMuxMapping(new MuxMapping(in2, out1, 0, 32, 32, 64));
    add(muxModel);

    modified_ = false;
}

void LibraryWindow::changeTypeOfSelected()
{
    LibraryListViewItem *item
        = dynamic_cast<LibraryListViewItem*>(modelListView_->selectedItem());
    if (item != 0) {
        QStringList typeNames = types();
        bool ok;
        QString type = QInputDialog::getItem
            ("POA", tr("Select a type"), typeNames,
             typeNames.findIndex(item->data().type()), true, &ok, this);
        if (ok) {
            item->data().setType(type);
            item->parent()->takeItem(item);
            getTypeItem(type)->insertItem(item);
        }
    }
}

QString LibraryWindow::defaultFilename()
{
    return Settings::instance()->confPath() + "/library.xml";
}


QListViewItem *LibraryWindow::getTypeItem(const QString &type)
{
    QListViewItem *item = typeItemByType.find(type);
    if (item == 0) {
        item = new TypeListViewItem(this, modelListView_, type);
        item->setOpen(true);
        if (type == "I/O") {
            item->setPixmap(0, QPixmap(Util::findIcon("connect_established.png")));
        }
        else if (type == "CPU") {
            item->setPixmap(0, QPixmap(Util::findIcon("kcmprocessor.png")));
        }
        else if (type == "Core") {
            item->setPixmap(0, QPixmap(Util::findIcon("memory.png")));
        }
        typeItemByType.insert(type, item);
    }
    return item;
}

void LibraryWindow::initialize()
{
    // read library file
    QFile file(defaultFilename());
    if (file.exists()) {
        open(&file);
    }
    else {
        addDefaultItems();
    }
}

bool LibraryWindow::isModified() const
{
    return modified_;
}

void LibraryWindow::renameTypeItem(QListViewItem *parent, const QString &oldName,
                                   const QString &newName)
{
    typeItemByType.remove(oldName);
    parent->setText(0, newName);
    typeItemByType.insert(newName, parent);

    QListViewItem *item = parent->firstChild();
    while (item != 0) {
        LibraryListViewItem *libraryItem
            = dynamic_cast<LibraryListViewItem*>(item);
        if (libraryItem != 0) {
            libraryItem->data().setType(newName);
        }
        item = item->nextSibling();
    }
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
        modified_ = false;
    }
}

void LibraryWindow::removeSelected()
{
    QListViewItem *item = modelListView_->selectedItem();
    if (item != 0) {
        delete item;
    }
}

void LibraryWindow::renameSelected()
{
    QListViewItem *item = modelListView_->selectedItem();
    if (item != 0) {
        item->startRename(0);
    }
}

void LibraryWindow::showPopup(QListViewItem *, const QPoint &pos, int)
{
    popupMenu_->exec(pos);
}

void LibraryWindow::save()
{
    if (isModified()) {
        QFile file(defaultFilename());
        save(&file);
    }
}

void LibraryWindow::save(QFile *file)
{
    if (file->open(IO_WriteOnly)) {
        QDomDocument document;
        QDomElement proj = document.createElement("project");
        document.appendChild(proj);
        QDomElement mlist = document.createElement("model");
        proj.appendChild(mlist);
        QDomElement blist = document.createElement("blocks");
        mlist.appendChild(blist);

        QListViewItem *item = modelListView_->firstChild();
        while (item != 0) {
            QListViewItem *child = item->firstChild();
            while (child != 0) {
                LibraryListViewItem *libraryItem
                    = dynamic_cast<LibraryListViewItem*>(child);
                if (libraryItem != 0) {
                    QDomElement mElem = libraryItem->data().serialize(&document);
                    blist.appendChild(mElem);
                }
                child = child->nextSibling();
            }
            item = item->nextSibling();
        }

        QTextStream out(file);
        document.save(out, 2);
        file->close();

        modified_ = false;
    }
    else {
        qWarning(QString(tr("Could not save library to %1")).arg(file->name()));
    }
}

void LibraryWindow::selectionChanged(QListViewItem *item)
{
    // enable change type menu only for library items
    popupMenu_->setItemEnabled(1, INSTANCEOF(item,LibraryListViewItem));
}

void LibraryWindow::setDescription(QListViewItem* item)
{
    descriptionTextBrowser_->setText(item->text(1));
}

void LibraryWindow::setOrientation(Qt::Orientation orientation)
{
    splitter_->setOrientation(orientation);
}

QStringList LibraryWindow::types()
{
    QStringList types;
    QListViewItem *item = modelListView_->firstChild();
    while (item != 0) {
        types.append(item->text(0));
        item = item->nextSibling();
    }
    return types;
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
    setText(0, item->name());
    setText(1, item->description());

    setDragEnabled(true);
    setRenameEnabled(0, true);
}

LibraryListViewItem::~LibraryListViewItem()
{
    delete item_;
}

AbstractModel &LibraryListViewItem::data() const
{
    return *item_;
}

void LibraryListViewItem::okRename(int col)
{
    QListViewItem::okRename(col);

    if (col == 0) {
        data().setName(text(col));
    }
}

TypeListViewItem::TypeListViewItem(LibraryWindow *library, QListView *parent,
                                   QString text)
    : QListViewItem(parent, text), library_(library)
{
    setRenameEnabled(0, true);
}

void TypeListViewItem::okRename(int col)
{
    if (col == 0) {
        QString oldName = text(0);
        QListViewItem::okRename(col);

        library_->renameTypeItem(this, oldName, text(0));
    }
}

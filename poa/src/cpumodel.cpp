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
 * $Id: cpumodel.cpp,v 1.24 2003/11/19 16:18:06 squig Exp $
 *
 *****************************************************************************/

#include "blockview.h"
#include "cpumodel.h"
#include "cpuview.h"
#include "codemanager.h"

#include <qdom.h> // used to provide serialization
#include <qcanvas.h>
#include <qtextstream.h>

#include "pinmodel.h"

CpuModel::CpuModel(QString type, QString description)
    : BlockModel(type, description)
{
    cpuId_ = -1;
    autoExecTime_ = 0;
    clock_ = 0;
    offset_ = 0;
    autoOffset_ = true;
    isProducer_ = true;
}

CpuModel::CpuModel(QDomElement cpuElem)
    : BlockModel(QString::null, QString::null)
{
    deserialize(cpuElem);

    isProducer_ = false;

    connect(this, SIGNAL(serialized(CpuModel *)),
            CodeManager::instance(), SLOT(save(CpuModel*)));
}

CpuModel::~CpuModel()
{
}

bool CpuModel::isProducer()
{
    return isProducer_;
}

int CpuModel::cpuId()
{
    return cpuId_;
}

void CpuModel::setCpuId(const int cpuId)
{
    cpuId_ = cpuId;
}

bool CpuModel::autoExecTime()
{
    return autoExecTime_;
}

void CpuModel::setAutoExecTime(const bool autoExecTime)
{
    autoExecTime_ = autoExecTime;
}

void CpuModel::setOffset(unsigned int offset)
{
    offset_ = offset;
}

unsigned int CpuModel::offset()
{
    return offset_;
}

void CpuModel::setAutoOffset(bool autoOffset)
{
    autoOffset_ = autoOffset;
}

bool CpuModel::autoOffset()
{
    return autoOffset_;
}

QCanvasItemList CpuModel::createView(QCanvas *canvas)
{
    QCanvasItemList list;
    CpuView *view = new CpuView (this, canvas);
    list.append(view);
    view->addPinViewsTo(list);
    return list;
}

/**
 * Produces the XML representation of this instance
 */
QDomElement CpuModel::serialize(QDomDocument *document)
{
    QDomElement root = BlockModel::serialize(document);
    root.setAttribute("block-type", "cpu");
    if (!isProducer_) {
        root.setAttribute("srcfile",
                CodeManager::instance()->sourceFilePath(this));
    }
    root.setAttribute("cpuid", cpuId_);
    root.setAttribute("autotime", autoExecTime_ ? "true" : "false");
    root.setAttribute("auto-offset", autoOffset_? "true":"false");
    root.setAttribute("offset", (unsigned int)offset_);
    root.setAttribute("clock", (unsigned int)clock_);

    emit serialized(this);

    return root;
}

void CpuModel::deserialize(QDomElement element) {
    BlockModel::deserialize(element);
    cpuId_ = element.attribute("cpuid", "0").toInt();


    setAutoOffset((element.attribute("auto-offset","true") == "true"));
    setOffset((unsigned int)element.attribute("offset","0").toUInt());
    setClock((unsigned int)element.attribute("clock","0").toUInt());

    // TRUE if value of autotime contains "TRUE" (case insensitive),
    // FALSE otherwise.
    autoExecTime_ =
        element.attribute("autotime", "true").contains("true", false);
}

void CpuModel::setProjectPath(QString *path)
{
    path_ = path;
}

QString *CpuModel::projectPath()
{
    return path_;
}

QString CpuModel::tip()
{
    CodeManager *codeManager = CodeManager::instance();
    return QString("<b>CPU</b><br><u>%1</u> (%2)<br><i>%3</i><hr>" \
                   "<b>Id on CPLD:</b> %4<br>" \
                   "<b>Clock:</b> %5 ms<br>" \
                   "<b>Offset:</b> %6<br>" \
                   "<b>Execution time:</b> %7<br>" \
                   "<b>Source:</b> %8")
        .arg(this->name())
        .arg(this->type())
        .arg(this->description())
        //        .arg((this->cpuId()==-1)?"not defined":QString::number(this->cpuId()))
        .arg((this->cpuId()==-1)?"not defined":QString::number(this->cpuId()))
        .arg(this->clock())
        .arg((this->autoOffset())?"auto":QString::number(this->offset())+" ms")
        .arg((this->autoExecTime())?"auto":QString::number(this->execTime())+" ms")
        .arg(codeManager->sourceFilePath(this));
}

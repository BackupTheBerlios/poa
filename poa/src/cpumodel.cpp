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
 * $Id: cpumodel.cpp,v 1.32 2004/01/17 17:35:39 squig Exp $
 *
 *****************************************************************************/

#include "blockview.h"
#include "cpumodel.h"
#include "codemanager.h"

#include <qdom.h> // used to provide serialization
#include <qcanvas.h>
#include <qtextstream.h>

#include "pinmodel.h"

CpuModel::CpuModel(QString type, QString description)
    : BlockModel(type, description)
{
    autoRuntime_ = 0;
    cpuId_ = -1;

    saveSource_ = true;
}

CpuModel::CpuModel(QDomElement element)
    : BlockModel(QString::null, QString::null)
{
    deserialize(element);

    saveSource_ = true;
}

CpuModel::~CpuModel()
{
}

bool CpuModel::autoRuntime() const
{
    return autoRuntime_;
}

int CpuModel::cpuId()
{
    return cpuId_;
}

void CpuModel::setCpuId(const int cpuId)
{
    cpuId_ = cpuId;
}

/**
 * Produces the XML representation of this instance
 */
QDomElement CpuModel::serialize(QDomDocument *document)
{
    QDomElement root = BlockModel::serialize(document);
    root.setAttribute("auto-runtime", autoRuntime() ? "true" : "false");
    root.setAttribute("block-type", "cpu");
    root.setAttribute("cpuid", cpuId_);

    if (saveSource_) {
        CodeManager::instance()->save(this);
    }

    return root;
}

void CpuModel::deserialize(QDomElement element)
{
    BlockModel::deserialize(element);

    setAutoRuntime((element.attribute("auto-runtime", "true") == "true"));
    setCpuId(element.attribute("cpuid", "0").toInt());
}

void CpuModel::setAutoRuntime(const bool autoRuntime)
{
    autoRuntime_ = autoRuntime;
}

void CpuModel::setProjectPath(QString path)
{
    path_ = path;
}

QString CpuModel::projectPath() const
{
    return path_;
}

void CpuModel::setSaveSource(const bool saveSource)
{
    saveSource_ = saveSource;
}

QString CpuModel::tip()
{
    CodeManager *codeManager = CodeManager::instance();
    return QString("<b>CPU</b><br><u>%1</u> (%2)<br><i>%3</i><hr>" \
                   "<b>Id on CPLD:</b> %4<br>" \
                   "<b>Clock:</b> %5 ns<br>" \
                   "<b>Offset:</b> %6<br>" \
                   "<b>Execution time:</b> %7<br>" \
                   "<b>Source:</b> %8")
        .arg(this->name())
        .arg(this->type())
        .arg(this->description())
        //        .arg((this->cpuId()==-1)?"not defined":QString::number(this->cpuId()))
        // garbeam: patched to work with Qt 3.2.x - ambigious overload
        .arg((this->cpuId()==-1)?QString("not defined"):QString::number(this->cpuId()))
        .arg(this->clock())
        .arg((this->autoOffset())?QString("auto"):QString::number(this->offset())+" ns")
        .arg(QString::number(this->runtime())+" ns")
        .arg(codeManager->sourceFilePath(this));
}

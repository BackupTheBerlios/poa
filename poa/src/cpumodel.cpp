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
 * $Id: cpumodel.cpp,v 1.39 2004/01/28 16:35:51 squig Exp $
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
    autoRuntime_ = true;
    cpuId_ = 1;
    source_ = QString::null;
}

CpuModel::CpuModel(QDomElement element)
    : BlockModel(QString::null, QString::null)
{
    deserialize(element);
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

void CpuModel::deserialize(QDomElement element)
{
    BlockModel::deserialize(element);

    setAutoRuntime((element.attribute("auto-runtime", "true") == "true"));
    setCpuId(element.attribute("cpuid", "1").toInt());

    // fetch sourcecode if available in xml tree
    QDomNodeList mList = element.elementsByTagName("source-code");
    if (mList.count() == 1
        && mList.item(0).toElement().firstChild().isCDATASection()) {

        source_
            = mList.item(0).toElement().firstChild().toCDATASection().data();
    }
    else {
        source_ = QString::null;
    }
}

QDomElement CpuModel::serialize(QDomDocument *document, QString source)
{
    QDomElement root = BlockModel::serialize(document);
    root.setAttribute("auto-runtime", autoRuntime() ? "true" : "false");
    root.setAttribute("block-type", "cpu");
    root.setAttribute("cpuid", cpuId_);
    if (!source.isEmpty()) {
        QDomElement element = document->createElement("source-code");
        element.appendChild(document->createCDATASection(source));
        root.appendChild(element);
    }
    return root;
}

QDomElement CpuModel::serialize(QDomDocument *document)
{
    return serialize(document, source_);
}

QDomElement CpuModel::serializeCopy(QDomDocument *document, Project *project)
{
    // read source file from disk in case it is not set
    CodeManager codeManager(project, this);
    return serialize(document, codeManager.sourceCode());
}

void CpuModel::setAutoRuntime(const bool autoRuntime)
{
    autoRuntime_ = autoRuntime;
}

void CpuModel::setCpuId(const int cpuId)
{
    cpuId_ = cpuId;
}

void CpuModel::setSource(QString source)
{
    source_ = source;
}

QString CpuModel::source()
{
    return source_;
}

QString CpuModel::tip()
{
    return QString("<u>%1</u> (%2)<br>"
                   "<i>%3</i><hr>" \
                   "<b>Id on CPLD:</b> %4<br>" \
                   "<b>Clock:</b> %5 ns<br>" \
                   "<b>Offset:</b> %6<br>" \
                   "<b>Execution time:</b> %7")
        .arg(this->name())
        .arg(this->type())
        .arg(this->description())
        .arg(this->cpuId())
        .arg(this->clock())
        .arg(toTip(this->autoOffset(), this->offset()))
        .arg(toTip(this->autoRuntime(), this->runtime()));
}

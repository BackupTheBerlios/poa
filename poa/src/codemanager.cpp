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
 * $Id: codemanager.cpp,v 1.3 2003/09/17 13:08:29 garbeam Exp $
 *
 *****************************************************************************/


#include "codemanager.h"
#include "cpumodel.h"
#include "settings.h"

#include <qfile.h>
#include <qprocess.h>
#include <qdir.h>

/**
 * The singleton instance.
 */
CodeManager* CodeManager::instance_ = 0;

CodeManager::CodeManager()
{
}

CodeManager::~CodeManager()
{
}

CodeManager *CodeManager::instance()
{
    if (instance_ == 0) {
        instance_ = new CodeManager();
    }

    return instance_;
}

QString CodeManager::sourcePath(CpuModel *model)
{
    return QString("%1/CPU_%2_sdk")
        .arg(*(model->projectPath()))
        .arg(model->cpuId());
}

QString CodeManager::sourceFilePath(CpuModel *model)
{
    return QString("%1/src/cpu_%2.c")
        .arg(sourcePath(model))
        .arg(model->cpuId());
}

int CodeManager::compile(CpuModel *model)
{
    Settings* s = Settings::instance();

    QProcess *proc = new QProcess(this);
    proc->addArgument(s->get("Terminal"));
    proc->addArgument(s->get("Compiler"));
    proc->addArgument(sourcePath(model));
    proc->setWorkingDirectory(QDir(*(model->projectPath())));

    proc->launch("");

    return proc->exitStatus();
}

void CodeManager::save(CpuModel *model)
{
    // check directory structure
    QDir cpuDir(sourcePath(model));
    QDir srcDir(sourcePath(model) + QString("/src"));
    QDir libDir(sourcePath(model) + QString("/lib"));
    QDir incDir(sourcePath(model) + QString("/inc"));

    if (!cpuDir.exists()) {
        cpuDir.mkdir(cpuDir.absPath());
    }
    if (!srcDir.exists()) {
        srcDir.mkdir(srcDir.absPath());
    }
    if (!libDir.exists()) {
        libDir.mkdir(libDir.absPath());
    }
    if (!incDir.exists()) {
        incDir.mkdir(incDir.absPath());
    }

    // check source file
    QFile source(sourceFilePath(model));
    if (!source.exists()) {
        // TODO: Copy template.
    }

}

void CodeManager::remove(CpuModel *model)
{

}

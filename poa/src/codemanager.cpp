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
 * $Id: codemanager.cpp,v 1.22 2004/02/11 13:38:48 garbeam Exp $
 *
 *****************************************************************************/


#include "codemanager.h"
#include "cpumodel.h"
#include "pinmodel.h"
#include "poaexception.h"
#include "project.h"
#include "processdialog.h"
#include "settings.h"
#include "util.h"

#include <qapplication.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qprocess.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qvaluelist.h>

CodeManager::CodeManager(Project *project, CpuModel *cpuModel)
{
    project_ = project;
    model_ = cpuModel;
}

CodeManager::~CodeManager()
{
}

QString CodeManager::cpuPath()
{
    return QString("%1/CPU_%2_sdk")
        .arg(project_->projectPath())
        .arg(model_->cpuId());
}

QString CodeManager::sourcePath()
{
    return QString("%1/src").arg(cpuPath());
}

QString CodeManager::libPath()
{
    return QString("%1/lib").arg(cpuPath());
}

QString CodeManager::incPath()
{
    return QString("%1/inc").arg(cpuPath());
}

QString CodeManager::filename(QString extension)
{
    return QString("cpu_%1.%2")
        .arg(model_->cpuId())
        .arg(extension);
}

QString CodeManager::sourceFilePath(QString extension)
{
    return QString("%1/%2")
        .arg(sourcePath())
        .arg(filename(extension));
}

int CodeManager::compile()
{
    // create arguments
    QStringList args
        = QStringList::split(' ', Settings::instance()->compilerCmd());
    args.append(filename());

    ProcessDialog *dialog = ProcessDialog::instance();
    return dialog->run(sourcePath(), args);
}

void CodeManager::createDirectories()
{
    // check directory structure
    createDirectory(QDir(cpuPath()));
    createDirectory(QDir(sourcePath()));
    createDirectory(QDir(libPath()));
    createDirectory(QDir(incPath()));
}

void CodeManager::createDirectory(const QDir &path)
{
    if (!path.exists()) {
        if (!path.mkdir(path.absPath())) {
            throw PoaException
	      (QString(qApp->translate("codemanager","Could not create directory: %1")).arg(path.absPath()));
        }
    }
}

bool CodeManager::edit()
{
    QProcess *proc = new QProcess();
    QStringList args
        = QStringList::split(' ', Settings::instance()->editorCmd());
    args.append(filename());

    proc->setArguments(args);
    proc->setWorkingDirectory(QDir(sourcePath()));

    return proc->launch("");
}

void CodeManager::copyTemplate()
{
    QFile cpuTemplate(Settings::instance()->templatePath());
    if (cpuTemplate.exists()) {
        QFile source(sourceFilePath());
        Util::copyFile(&cpuTemplate, &source);
    }
    else {
        throw PoaException(QString(qApp->translate("codemanager","Could not copy code template: %1 does not exist.")).arg(Settings::instance()->templatePath()));
    }
}

void CodeManager::saveSource()
{
    Q_ASSERT(!model_->source().isNull());

    QFile source(sourceFilePath());
    Util::writeFile(&source, model_->source());
    model_->setSource(QString::null);
}

// void CodeManager::remove()
// {
//     // check directory structure
//     QDir cpuDir(cpuPath(model_));

//     if (!Util::removeDir(&cpuDir)) {
//         // TODO: pop up error dialog.
//     }
// }

QString CodeManager::sourceCode()
{
    if (!model_->source().isNull()) {
        return model_->source();
    }

    QFile source(sourceFilePath());
    if (source.exists()) {
        return Util::readFile(&source);
    }

    return QString::null;
}

bool CodeManager::templateIsSubstitutable() {

    Q_ASSERT(model_->source().isNull());
    QString source = sourceCode();
    // only one block is allowed!
    return (source.find("/*!POA!*/") != -1) &&
           (source.find("/*!AOP!*/") != -1);
}

void CodeManager::prependSubstitutionMarkers() {
    Q_ASSERT(model_->source().isNull());
    QString source = sourceCode();
    source.prepend("\n/*!POA!*/\n\n/*!AOP!*/\n");
    model_->setSource(source);
    saveSource();
}

void CodeManager::substitute() {
    Q_ASSERT(templateIsSubstitutable());

    QString substTxt("/*!POA!*/\n");

    QValueList<PinModel*> pins = model_->pins();
    for (QValueListIterator<PinModel *> it = pins.begin();
         it != pins.end(); ++it)
    {
        PinModel *pin = *it;
        substTxt.append(QString("    np_pio *%1 = 0x%2;\n")
                            .arg(pin->name())
                            .arg(pin->address()));
    }

    QString source = sourceCode();
    // Note: fucking QRegExp don't works over newlines, so
    // we've to double-match... - so QRegExp is useless indeed :((
    int firstIndex = source.find("/*!POA!*/");
    int lastIndex = source.find("/*!AOP!*/");

    Q_ASSERT(firstIndex < lastIndex);
    source.replace(firstIndex, lastIndex - firstIndex, substTxt);
    model_->setSource(source);
    saveSource();
}

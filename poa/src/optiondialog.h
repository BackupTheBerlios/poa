/****************************************************************************
** Form interface generated from reading ui file 'optiondialog.ui'
**
** Created: Tue Aug 19 15:36:27 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;
class QPushButton;

class OptionDialog : public QDialog
{ 
    Q_OBJECT

public:
    OptionDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~OptionDialog();

    QPushButton* downloadPushButton;
    QPushButton* cTemplatePushButton;
    QPushButton* compilerPushButton;
    QPushButton* editorPushButton;
    QLineEdit* compilerLineEdit;
    QLineEdit* cTemplateLineEdit;
    QLineEdit* downloadLineEdit;
    QLineEdit* editorLineEdit;
    QLabel* editorTextLabel;
    QLabel* externalCompilerTextLabel;
    QLabel* externalDownloadTextLabel;
    QLabel* cTemplateTextLabel;
    QPushButton* okPushButton;
    QPushButton* cancelPushButton;
    QPushButton* helpPushButton;


};

#endif // OPTIONDIALOG_H

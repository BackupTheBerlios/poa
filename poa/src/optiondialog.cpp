/****************************************************************************
** Form implementation generated from reading ui file 'optiondialog.ui'
**
** Created: Tue Aug 19 15:38:55 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "optiondialog.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a OptionDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
OptionDialog::OptionDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "OptionDialog" );
    resize( 454, 223 ); 
    setCaption( trUtf8( "Options" ) );

    downloadPushButton = new QPushButton( this, "downloadPushButton" );
    downloadPushButton->setGeometry( QRect( 420, 140, 20, 22 ) ); 
    downloadPushButton->setText( trUtf8( "..." ) );

    cTemplatePushButton = new QPushButton( this, "cTemplatePushButton" );
    cTemplatePushButton->setGeometry( QRect( 420, 100, 20, 22 ) ); 
    cTemplatePushButton->setText( trUtf8( "..." ) );

    compilerPushButton = new QPushButton( this, "compilerPushButton" );
    compilerPushButton->setGeometry( QRect( 420, 60, 20, 22 ) ); 
    compilerPushButton->setText( trUtf8( "..." ) );

    editorPushButton = new QPushButton( this, "editorPushButton" );
    editorPushButton->setGeometry( QRect( 420, 20, 20, 22 ) ); 
    editorPushButton->setText( trUtf8( "..." ) );

    compilerLineEdit = new QLineEdit( this, "compilerLineEdit" );
    compilerLineEdit->setGeometry( QRect( 160, 60, 250, 22 ) ); 

    cTemplateLineEdit = new QLineEdit( this, "cTemplateLineEdit" );
    cTemplateLineEdit->setGeometry( QRect( 160, 100, 250, 22 ) ); 

    downloadLineEdit = new QLineEdit( this, "downloadLineEdit" );
    downloadLineEdit->setGeometry( QRect( 160, 140, 250, 22 ) ); 

    editorLineEdit = new QLineEdit( this, "editorLineEdit" );
    editorLineEdit->setGeometry( QRect( 160, 20, 250, 22 ) ); 

    editorTextLabel = new QLabel( this, "editorTextLabel" );
    editorTextLabel->setGeometry( QRect( 20, 20, 90, 20 ) ); 
    editorTextLabel->setText( trUtf8( "external editor" ) );

    externalCompilerTextLabel = new QLabel( this, "externalCompilerTextLabel" );
    externalCompilerTextLabel->setGeometry( QRect( 20, 60, 100, 20 ) ); 
    externalCompilerTextLabel->setText( trUtf8( "external compiler" ) );

    externalDownloadTextLabel = new QLabel( this, "externalDownloadTextLabel" );
    externalDownloadTextLabel->setGeometry( QRect( 20, 140, 130, 20 ) ); 
    externalDownloadTextLabel->setText( trUtf8( "external download tool" ) );

    cTemplateTextLabel = new QLabel( this, "cTemplateTextLabel" );
    cTemplateTextLabel->setGeometry( QRect( 20, 100, 133, 20 ) ); 
    cTemplateTextLabel->setText( trUtf8( "C source template path" ) );

    okPushButton = new QPushButton( this, "okPushButton" );
    okPushButton->setGeometry( QRect( 250, 182, 91, 30 ) ); 
    okPushButton->setText( trUtf8( "&OK" ) );
    okPushButton->setDefault( TRUE );

    cancelPushButton = new QPushButton( this, "cancelPushButton" );
    cancelPushButton->setGeometry( QRect( 350, 182, 91, 30 ) ); 
    cancelPushButton->setText( trUtf8( "&Cancel" ) );

    helpPushButton = new QPushButton( this, "helpPushButton" );
    helpPushButton->setGeometry( QRect( 20, 182, 91, 30 ) ); 
    helpPushButton->setText( trUtf8( "&Help" ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
OptionDialog::~OptionDialog()
{
    // no need to delete child widgets, Qt does it all for us
}


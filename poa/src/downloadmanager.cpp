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
 * $Id: downloadmanager.cpp,v 1.9 2004/01/29 21:02:52 papier Exp $
 *
 *****************************************************************************/


#include "downloadmanager.h"
#include "cpumodel.h"
#include "settings.h"
#include "srecord.h"
#include "processdialog.h"
#include "poaexception.h"
#include "util.h"
#include "qextserialport/qextserialbase.h"
#include "qextserialport/qextserialport.h"

#include <qfileinfo.h>
#include <qfile.h>
#include <qprogressdialog.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <stdio.h>


/**
 * The singleton instance.
 */
DownloadManager* DownloadManager::instance_ = 0;

DownloadManager::DownloadManager()
{
}

DownloadManager::~DownloadManager()
{

}

DownloadManager *DownloadManager::instance()
{
    if (instance_ == 0) {
        instance_ = new DownloadManager();
    }

    return instance_;
}

bool DownloadManager::readFile(QString filename)
{
  //open file
  QFile file;
  file.setName(filename);
  if ( file.open( IO_ReadOnly )) {
    
    //read data
    QString line;
    int error;
    while (! file.atEnd() ) {
      error = file.readLine(line, 254);
      if (error > 0) {
	if (line.startsWith("S1") ) {

	  srecFile_.append(new SRecord(line.left(2),   //type
				       line.mid(3, 2), //count
				       line.mid(5, 4), //address
				       line.mid(9, (line.length() - 10) ), //data
				       line.right(2)) ); //checksum
	  
	}
      }
      else {
	throw PoaException(tr("Could not read srec file %1.").arg(filename));
	return false;
      }
    }
    file.close();
    return true;
  }
  else {
    throw PoaException(tr("Could not open srec file %1.").arg(filename));
    return false;
  }
}

int DownloadManager::fileSize()
{
  QPtrListIterator<SRecord> it (srecFile_);
  SRecord *srecord;
  int sum = 0;
  bool ok;
  while ((srecord = it.current()) != 0) {
    sum += srecord->count().toInt( &ok, 10) -3;
  } 
  
  return sum;
}

bool DownloadManager::run(const char* portname)
{

  QextSerialPort port;
  port.setName(portname);
  if (! port.open(0)) {
    throw PoaException(tr("Could not open serial port"));
    return false;
  }
  else {

    //send header
    port.putch(0xff);
    port.flush();
    qApp->processEvents(4);
    
    //send adress
    port.putch(0x01);
    port.flush();
    qApp->processEvents(4);
    
    //send size
    port.putch((unsigned char) 0);
    port.flush();
    qApp->processEvents(4);

    port.putch((unsigned char) 0);
    port.flush();
    qApp->processEvents(4);
    
    //send command
    //0x01 = LOAD
    //0x05 = RUN
    port.putch(0x05);
    port.flush();
    qApp->processEvents(4);

    return true;
  }
}

bool DownloadManager::download(QString filename, const char* portname)
{

  //open serial port
  QextSerialPort port;
  port.setName(portname);
  if (! port.open(0)) {
    throw PoaException(tr("Could not open serial port."));
    return false;
  }
  else {

    //open file
    readFile(filename);

    //send header
    port.putch(0xff);
    port.flush();
    qApp->processEvents(4);
    
    //send adress
    port.putch(0x01);
    port.flush();
    qApp->processEvents(4);
    
    //send size
    port.putch((unsigned char)(fileSize()>>8));
    port.flush();
    qApp->processEvents(4);
    
    port.putch((unsigned char)(fileSize()&0xff));
    port.flush();
    qApp->processEvents(4);
    
    //send command
    //0x01 = LOAD
    //0x05 = RUN
    port.putch(0x01);
    port.flush();
    qApp->processEvents(4);
    
    //send data
    // Open Progressbar Dialog
    QProgressDialog progress(tr("Downloading srec file to cpld..."), 
			     tr("Abort"), 
			     fileSize(), 
			     0,
			     "progress", 
			     TRUE);
    QPtrListIterator<SRecord> it (srecFile_);
    SRecord *srecord;
    while ((srecord = it.current()) !=0) {
      const QString data = srecord->data();
      for (int i=0; i < (data.length()); i++) {

	port.putch( data.at(i) );
	port.flush();

	progress.setProgress(progress.progress() +1);
	qApp->processEvents(4);
      
	if ( progress.wasCancelled() ){
	  return false;
	}
      }
    }
    port.close();
    return true;
  }
}


/* Original code by Christian Kramer to convert .srec files
   into memory maps "byte by byte"


#include <stdio.h>
#include <io.h>
#include <time.h>

#define PAUSE   300//  1/PAUSE Sekunden Pause

int main (int argc, const char * argv[] )
{
        FILE    *ifile,*ofile,*com1;
        int     zeilenanzahl=0,err=0,laenge,addr;
        char    zeile[255],zeile_tmp[255],data[255],dummy,ofilename[255];
        unsigned char zeichen,zeichen1,zeichen2;
        int     dummy_int,i,j;
        long    dateigroesse=0,delay;
        float progress=0.0125;
        int     pos;
        clock_t c1;

        if (argc<=1) {
                printf("\nMissing filename.\n");
                printf("Usage: isw_download <filename.srec> \n\n");
                return (-1);
        }

        printf("Open %s \n",argv[1]);

        ifile=fopen(argv[1],"r");
        com1=fopen("com1:","wb");
        if (com1==-1) printf("Please close all connections to COM1 !\n");

        //Sende zunächst die Dateigrösse in zwei Bytes
        dateigroesse=0;
        do
        {
                err=fgets(zeile ,254, ifile);

                if (err!=0) {

                        if (zeile[1]=='1') {
                                sscanf(zeile,"%c%1i%2x%4x%s",&dummy,&dummy_int,&laenge,&addr,zeile_tmp);
                                dateigroesse+=(laenge-3);
                        }
                }
        }while (err!=0);

        printf("Filelength: %i Bytes\n",dateigroesse);
        // Header senden
        fputc(0xff,com1);fflush(com1);
                                        c1=clock();
                                        while(clock()<c1+CLOCKS_PER_SEC/PAUSE);

        // Adresse senden
        fputc(0x01,com1);fflush(com1);
                                        c1=clock();
                                        while(clock()<c1+CLOCKS_PER_SEC/PAUSE);
        // Groesse senden
        fputc((unsigned char)(dateigroesse>>8),com1);fflush(com1);
                                        c1=clock();
                                        while(clock()<c1+CLOCKS_PER_SEC/PAUSE);
        fputc((unsigned char)(dateigroesse&0xff),com1);fflush(com1);
                                        c1=clock();
                                        while(clock()<c1+CLOCKS_PER_SEC/PAUSE);

        // Command senden 0x01 = LOAD, 0x05 = RUN
        fputc(0x01,com1);fflush(com1);
                                        c1=clock();
                                        while(clock()<c1+CLOCKS_PER_SEC/PAUSE);



        // Jetzt sende die Daten
        fseek(ifile,0,SEEK_SET);
        do
        {
                err=fgets(zeile ,254, ifile);

                if (err!=0) {

                        if (zeile[1]=='1') {
                                sscanf(zeile,"%c%1i%2x%4x%s",&dummy,&dummy_int,&laenge,&addr,zeile_tmp);
                                // schneide Checksum ab
                                strncpy(data,zeile_tmp,2*(laenge-3));
                                for (i=0;i<(laenge-3);i++) {

                                        sscanf(data,"%2x%s",&zeichen,data);
                                        //fprintf(ofile,"%c",zeichen);fflush(0);
                                        fputc(zeichen,com1);fflush(com1);
                                        pos++;
                                        c1=clock();
                                        while(clock()<c1+CLOCKS_PER_SEC/PAUSE);
                                        if (((double) pos/dateigroesse) > progress) {
                                                progress+=(0.0125);
                                                printf("#");fflush(0);
                                        }


                                }
                        }
                }
        }while (err!=0);

        fclose(ifile);
        fclose(com1);
}

*/

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
 * $Id: downloadmanager.cpp,v 1.7 2004/01/26 09:10:48 papier Exp $
 *
 *****************************************************************************/


#include "downloadmanager.h"
#include "cpumodel.h"
#include "settings.h"
#include "processdialog.h"
#include "util.h"
#include "qextserialport/qextserialbase.h"
#include "qextserialport/qextserialport.h"

#include <qfileinfo.h>
#include <qfile.h>
#include <qprocess.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qthread.h>
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

bool DownloadManager::run(const char* portname)
{

  //TODO: return false if an error occurs e.g. Port is already opened
  //      Any check nesessary if there is a file on the CPLD??

  QextSerialPort port;
  port.setName(portname);
  port.open(0);

  //send header
  port.putch(0xff);
  port.flush();
  usleep(300);
  
  //send adress
  port.putch(0x01);
  port.flush();
  usleep(300);

  //send size
  port.putch((unsigned char) 0);
  port.flush();
  usleep(300);

  port.putch((unsigned char) 0);
  port.flush();
  usleep(300);

  //send command
  //0x01 = LOAD
  //0x05 = RUN
  port.putch(0x05);
  port.flush();
  usleep(300);

  return true;
  
}

bool DownloadManager::download(QString filename, const char* portname)
{

  //TODO: return false if an error occurs e.g. Port is already opened

  long fileSize = 0;
  Q_LONG err = 0;
  int lenght = 0;
  int address = 0;
  char line[255];
  char line_tmp[255];
  char data[255];
  char dummy;
  int dummy_int=0;
  unsigned char character;
  int pos = 0;




  //open serial port
  QextSerialPort port;
  port.setName(portname);
  port.open(0);


  //open file
  QFile file;
  file.setName(filename);
  file.open( IO_ReadOnly );

  //get size of data to send
  while (file.atEnd() == false) {
    err = file.readLine(line, 254);

    if (err != 0) {
      if (line[1]=='1') {
        sscanf(line, "%c%1i%2x%4x%s", &dummy, &dummy_int,
               &lenght, &address, line_tmp);
        fileSize += (lenght -3 );
      }
    }
  }
  //set number of steps in DeployProject Download Progressbar
  emit setProgressBarLength(fileSize);
  file.close();

  //send header
  port.putch(0xff);
  port.flush();
  usleep(300);

  //send adress
  port.putch(0x01);
  port.flush();
  usleep(300);

  //send size
  port.putch((unsigned char)(fileSize>>8));
  port.flush();
  usleep(300);

  port.putch((unsigned char)(fileSize&0xff));
  port.flush();
  usleep(300);

  //send command
  //0x01 = LOAD
  //0x05 = RUN
  port.putch(0x01);
  port.flush();
  usleep(300);

  //send data
  file.open( IO_ReadOnly );
  while (file.atEnd() == false) {
    err = file.readLine(line, 254);

    if (err!=0) {
      if (line[1] == '1') {
        sscanf(line, "%c%1i%2x%4x%s", &dummy, &dummy_int,
               &lenght, &address, line_tmp);
        //remove checksum
        strncpy( data, line_tmp, 2* (lenght-3) );
        for (int i=0; i < (lenght-3); i++) {
          sscanf(data, "%2x%s", &character, data);
          port.putch(character);
          port.flush();
          pos++;
	  usleep(300);
          //increase Download Progressbar in DeployProjectWizard
          emit increaseProgressBar();
        }
      }
    }
  }
  file.close();
  port.close();
  return true;
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

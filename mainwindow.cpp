//gui_tool v0014

/*
 * gui_tool - GUI framework for batch processing
 *
 * Copyright (c) 2021 A.D. Klumpp
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

//Gui framework for batch processing e.g. for batch coversion of multiple files inside of folders and their sub folders
//example: (1) jpg to baseline jpg (non progressive) via jpegtran (2) png to jpg via imagemagick (convert)

//required:
//jpegtran
//imagemagick (convert)


//Example functions:

//1)
//jpegtran -perfect in.jpg > out.jpg
//for one file per folder, including 3 levels of subfolders, e.g. to display album art for sony walkman, which only supports baseline.jpg

//2)
//
//convert cover.png cover.jpg
//for one file per folder, including 3 levels of subfolders, e.g. as preparation of 1)

//3)
//convert anyname.png anyname.pngconvert.jpg
//converts all png files in a selected folder into jpg files



#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
//#include <QThread>
#include <unistd.h>
#include <QFileInfoList>

#include <cstring>
#include <iostream>



#include <QFileInfo>
#include <QDirIterator>
#include <QStringList>
#include <QProcess>

#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}


//for one file per folder, e.g. to display album art for sony walkman, which only supports baseline.jpg
//jpegtran -perfect non-baseline.jpg > baseline+name-of-folder.jpg
void MainWindow::openfile()
{

   QString hpath = QDir::homePath();

   QString savefilePath = QFileDialog::getExistingDirectory(this, tr("Select folder"));

 //    savefileName = QFileDialog::getSaveFileName(this, tr("Save File"));



    qDebug() << "--------------savefileName ----------- " << savefilePath ;

    ui->plainTextEdit->setPlainText(savefilePath);

    QFileInfoList FileInfoList;

    //main msg

    if(savefilePath!="")
        {

    QMessageBox msgBox;
    msgBox.setWindowTitle("Are you sure?");
    msgBox.setText("Batch processing for the selected folder including sub folders?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {






    //msg

   //  oppath = QFileInfo(vfilePath).path();

    QDir dir(savefilePath);


//Level 1 +++++++++++++++++++++++++

    dir.setNameFilters(QStringList("*.jpg"));
       dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

       qDebug() << "Scanning FOLDER LEVEL 1 " << dir.path();

       QStringList fileList = dir.entryList();
       for (int i=0; i<fileList.count(); i++)
       {


               qDebug() << "Found file LEVEL 1: " << fileList[i];

                qDebug() << "name of folder LEVEL 1 " << savefilePath;

                             qDebug() << "<<<<<<<<<<<<<<<< warning: image is inside of the root folder - not an album folder - no changes!";

                             QString warn = "LEVEL 1: image is inside of the root folder - no changes!";


                             ui->textEdit->insertPlainText(warn+"\n");

                             ui->textEdit->moveCursor (QTextCursor::End);



       }


//Lelvel 2 ++++++++++++++++++++++++

       dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
       QStringList dirList = dir.entryList();
       for (int i2=0; i2<dirList.size(); ++i2)
       {
           QString newPathL2 = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i2));
           //scanDir(QDir(newPath));

            qDebug() << "Level 2 Path  " << newPathL2 ;



             QString albumnameL2 =QFileInfo(newPathL2).fileName();

             qDebug() << "++++++++++++++++ name of album LEVEL 2 " << albumnameL2;

            //

            QDir newdirL2(newPathL2);


            newdirL2.setNameFilters(QStringList("*.jpg"));
               newdirL2.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

               qDebug() << "Scanning FOLDER LEVEL 2: " << newdirL2.path();


               QStringList newfileListL2 = newdirL2.entryList();


              for (int i22=0; i22<newfileListL2.count(); i22++)
               {
                   //- only the first jpg is used as cover jpg
                   if(i22==0)
                   {

                       QString namefirstfileL2 = newfileListL2[i22];
                       QString albumplusjpgL2 = albumnameL2 + ".jpg";

                       qDebug() << "LEVEL 2 ??????????????????????????????????????????????????????????????????????????";

                                   qDebug() << "newfileList[i]   " << namefirstfileL2;

                                  qDebug() << "albumname   " << albumplusjpgL2;



                        qDebug() << "??????????????????????????????????????????????????????????????????????????";



                        //if file with the name of the folder is not the first one
                                 if(namefirstfileL2!=albumplusjpgL2)

                         {

//
                       qDebug() << "Found jpg file LEVEL 2: " << newfileListL2[i22] << " in " << albumnameL2;

                       QString msg1L2 =  "LEVEL 2: Found jpg file: " + newfileListL2[i22] + " in " + albumnameL2;




//                       ui->textEdit->insertPlainText(msg1+"\n");

//                       ui->textEdit->moveCursor (QTextCursor::End);



                       //QString commandLvl2 = "jpegtran -perfect '" + newPath + newfileList[i] + "' > '" + newPath + "'" ;

                        QString commandLvl2 = "jpegtran -perfect '"  + newPathL2 + "/" + newfileListL2[i22] + "' > '" + newPathL2 + "/" + albumnameL2 + ".jpg" + "'" ;

                       ui->textEdit->insertPlainText(commandLvl2+"\n");

                       ui->textEdit->moveCursor (QTextCursor::End);


                       //creating string for system

                       QByteArray arrayL2;
                                char*  newPathL2_;

                       arrayL2 = newPathL2.toLocal8Bit();
                                      newPathL2_ = arrayL2.data();

                      QByteArray array2L2;
                               char*  albumnameL2_;

                      array2L2 = albumnameL2.toLocal8Bit();
                                     albumnameL2_ = array2L2.data();



                        QString firstjpgL2 =  newfileListL2[i22];

                        QByteArray array3L2;
                                 char*  firstjpgL2_;

                        array3L2 = firstjpgL2.toLocal8Bit();
                                       firstjpgL2_ = array3L2.data();



//  QString commandLvl2 = "jpegtran -perfect '"  + newPath + "/" + newfileList[i] + "' > '" + newPath + "/" + albumname + ".jpg" + "'" ;


                       char result2L2[50000];

                                                   strcpy(result2L2,"jpegtran -perfect '");
                                                   strcat(result2L2,newPathL2_);
                                                    strcat(result2L2,"/");
                                                    strcat(result2L2,firstjpgL2_);
                                                    strcat(result2L2,"' > '");
                                                    strcat(result2L2,newPathL2_);
                                                    strcat(result2L2,"/");
                                                     strcat(result2L2,albumnameL2_);
                                                    strcat(result2L2,".jpg");
                                                    strcat(result2L2,"'");


//                                                    ui->textEdit->insertPlainText(result2+"\n");

//                                                    ui->textEdit->moveCursor (QTextCursor::End);






                                                            qDebug() << "-------crop------------result2--------------- " << result2L2;


                                                            if(system(result2L2) != 0)
                                                           {
                                                               qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++jpegtran failed..." ;

//                                                               ui->statusBar->showMessage("");
//                                                               QMessageBox msgBox;
//                                                               msgBox.setText("Error Level 2: jpegtran failed: " + albumnameL2);
//                                                               msgBox.exec();

                                                               ui->textEdit->insertPlainText("ERROR Level 2: " + commandLvl2 + "\n");

                                                               ui->textEdit->moveCursor (QTextCursor::End);
                                                           }
                                                           else
                                                           {


                                                                ui->textEdit->insertPlainText("LEVEL 2: " + commandLvl2+"\n");

                                                                ui->textEdit->moveCursor (QTextCursor::End);
//                                                               ui->statusBar->showMessage("");
//                                                               QMessageBox msgBox;
//                                                               msgBox.setText("File saved.");
//                                                               msgBox.exec();
                                                           }






                            }



                   }




               }


//Level 3 ++++++++++++++++++++++

               newdirL2.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
               QStringList dirList2L3 = newdirL2.entryList();
               for (int i3=0; i3<dirList2L3.size(); ++i3)
               {

                   //if(i3==0) - last edit
                   //{
                   QString newPath2L3 = QString("%1/%2").arg(newdirL2.absolutePath()).arg(dirList2L3.at(i3));
                   //scanDir(QDir(newPath));

                    qDebug() << "Level 3 Path +++++++++++++++++++++ " << newPath2L3 ;

                    QString albumname2L3 =QFileInfo(newPath2L3).fileName();

                    qDebug() << "++++++++++++++++ name of album " << albumname2L3;


                    QDir newdir2L3(newPath2L3);

                    newdir2L3.setNameFilters(QStringList("*.jpg"));
                       newdir2L3.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

                       qDebug() << "Scanning FOLDER LEVEL 3: " << newdir2L3.path();


                       QStringList newfileList2L3 = newdir2L3.entryList();
                       for (int i33=0; i33<newfileList2L3.count(); i33++)
                       {


                           //if(i3==0) - last edit - moved here
                           //{
                           if(i33==0)
                           {

                           QString namefirstfile3L3 = newfileList2L3[i33];
                           QString albumplusjpgL3 = albumname2L3 + ".jpg";


                           qDebug() << "LEVEL 3 ##################??????????????????????????????????????????????????????????????????????????";

                                       qDebug() << "newfileList[i]   " << namefirstfile3L3;

                                      qDebug() << "albumname + jpg  " << albumplusjpgL3;



                            qDebug() << "#######################??????????????????????????????????????????????????????????????????????????";



                   //if file with the name of the folder is not existing already:
                            if(namefirstfile3L3!=albumplusjpgL3)

                    {
        //
                               qDebug() << "Found jpg file LEVEL 3: " << newfileList2L3[i33] << " in " << albumname2L3;


                               //00000000000


                               QString commandLvl3 = "jpegtran -perfect '"  + newPath2L3 + "/" + newfileList2L3[i33] + "' > '" + newPath2L3 + "/" + albumname2L3 + ".jpg" + "'" ;

//                              ui->textEdit->insertPlainText(commandLvl3+"\n");

//                              ui->textEdit->moveCursor (QTextCursor::End);


                              //creating string for system

                              QByteArray array11L3;
                                       char*  newPath_2L3;

                              array11L3 = newPath2L3.toLocal8Bit();
                                             newPath_2L3 = array11L3.data();

                             QByteArray array22L3;
                                      char*  albumname_2L3;

                             array22L3 = albumname2L3.toLocal8Bit();
                                            albumname_2L3 = array22L3.data();



                               QString firstjpg2L3 =  newfileList2L3[i33];

                               QByteArray array3L3;
                                        char*  firstjpg_2L3;

                               array3L3 = firstjpg2L3.toLocal8Bit();
                                              firstjpg_2L3 = array3L3.data();



       //  QString commandLvl2 = "jpegtran -perfect '"  + newPath + "/" + newfileList[i] + "' > '" + newPath + "/" + albumname + ".jpg" + "'" ;


                              char result3L3[50000];

                                                          strcpy(result3L3,"jpegtran -perfect '");
                                                          strcat(result3L3,newPath_2L3);
                                                           strcat(result3L3,"/");
                                                           strcat(result3L3,firstjpg_2L3);
                                                           strcat(result3L3,"' > '");
                                                           strcat(result3L3,newPath_2L3);
                                                           strcat(result3L3,"/");
                                                            strcat(result3L3,albumname_2L3);
                                                           strcat(result3L3,".jpg");
                                                           strcat(result3L3,"'");


       //                                                    ui->textEdit->insertPlainText(result2+"\n");

       //                                                    ui->textEdit->moveCursor (QTextCursor::End);






                                                                   qDebug() << "-------crop------------result2--------------- " << result3L3;


                                                                   if(system(result3L3) != 0)
                                                                  {
                                                                      qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++jpegtran failed...";

//                                                                      ui->statusBar->showMessage("");
//                                                                      QMessageBox msgBox;
//                                                                      msgBox.setText("Error Level 3: jpegtran failed: " + albumname2);
//                                                                      msgBox.exec();

                                                                      ui->textEdit->insertPlainText("ERROR Level 3: " + commandLvl3 + "\n");

                                                                      ui->textEdit->moveCursor (QTextCursor::End);
                                                                  }
                                                                  else
                                                                  {
                                                                       ui->textEdit->insertPlainText("LEVEL 3: " + commandLvl3+"\n");

                                                                       ui->textEdit->moveCursor (QTextCursor::End);

       //                                                               ui->statusBar->showMessage("");
       //                                                               QMessageBox msgBox;
       //                                                               msgBox.setText("File saved.");
       //                                                               msgBox.exec();
                                                                  }




                    }


                               //00000000000


                      }






//Level 4+++++++++++++++++++++++++


                    newdir2L3.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
                    QStringList dirList3L4 = newdir2L3.entryList();
                    for (int i4=0; i4<dirList3L4.size(); ++i4)
                    {
                        QString newPath3L4 = QString("%1/%2").arg(newdir2L3.absolutePath()).arg(dirList3L4.at(i4));
                        //scanDir(QDir(newPath));

                         qDebug() << "Level 4 Path +++++++++++++++++++++ " << newPath3L4 ;

                         //

                         QString albumname3L4 =QFileInfo(newPath3L4).fileName();

                         qDebug() << "++++++++++++++++ name of album " << albumname3L4;


                         QDir newdir3L4(newPath3L4);

                         newdir3L4.setNameFilters(QStringList("*.jpg"));
                            newdir3L4.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

                            qDebug() << "Scanning FOLDER LEVEL 4: " << newdir3L4.path();


                            QStringList newfileList3L4 = newdir3L4.entryList();
                            for (int i44=0; i44<newfileList3L4.count(); i44++)
                            {
             //
                                //if

                                if(i44==0)
                                {

                                QString namefirstfile3L4 = newfileList3L4[i44];
                                QString albumplusjpgL4 = albumname3L4 + ".jpg";


                                qDebug() << "LEVEL 4 ##################??????????????????????????????????????????????????????????????????????????";

                                            qDebug() << "newfileList[i]   " << namefirstfile3L4;

                                           qDebug() << "albumname + jpg  " << albumplusjpgL4;



                                 qDebug() << "#######################??????????????????????????????????????????????????????????????????????????";



                        //if file with the name of the folder is not existing already:
                                 if(namefirstfile3L4!=albumplusjpgL4)

                         {









                                    qDebug() << "Found jpg file in LEVEL 4: " << newfileList3L4[i44] << " in " << albumname3L4;


                                    //111111111111111




                                    //00000000000


                                    QString commandLvl4 = "jpegtran -perfect '"  + newPath3L4 + "/" + newfileList3L4[i44] + "' > '" + newPath3L4 + "/" + albumname3L4 + ".jpg" + "'" ;

//                                   ui->textEdit->insertPlainText(commandLvl4+"\n");

//                                   ui->textEdit->moveCursor (QTextCursor::End);


                                   //creating string for system

                                   QByteArray array111L4;
                                            char*  newPath_3L4;

                                   array111L4 = newPath3L4.toLocal8Bit();
                                                  newPath_3L4 = array111L4.data();

                                  QByteArray array222L4;
                                           char*  albumname_3L4;

                                  array222L4 = albumname3L4.toLocal8Bit();
                                                 albumname_3L4 = array222L4.data();



                                    QString firstjpg3L4 =  newfileList3L4[i44];

                                    QByteArray array33L4;
                                             char*  firstjpg_3L4;

                                    array33L4 = firstjpg3L4.toLocal8Bit();
                                                   firstjpg_3L4 = array33L4.data();



            //  QString commandLvl2 = "jpegtran -perfect '"  + newPath + "/" + newfileList[i] + "' > '" + newPath + "/" + albumname + ".jpg" + "'" ;


                                   char result2L4[50000];

                                                               strcpy(result2L4,"jpegtran -perfect '");
                                                               strcat(result2L4,newPath_3L4);
                                                                strcat(result2L4,"/");
                                                                strcat(result2L4,firstjpg_3L4);
                                                                strcat(result2L4,"' > '");
                                                                strcat(result2L4,newPath_3L4);
                                                                strcat(result2L4,"/");
                                                                 strcat(result2L4,albumname_3L4);
                                                                strcat(result2L4,".jpg");
                                                                strcat(result2L4,"'");


            //                                                    ui->textEdit->insertPlainText(result2+"\n");

            //                                                    ui->textEdit->moveCursor (QTextCursor::End);






                                                                        qDebug() << "-------crop------------result2--------------- " << result2L4;


                                                                        if(system(result2L4) != 0)
                                                                       {
                                                                           qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++jpegtran failed..." ;

//                                                                           ui->statusBar->showMessage("");
//                                                                           QMessageBox msgBox;
//                                                                           msgBox.setText("Error Level 4: jpegtran failed: " + albumname3L4);
//                                                                           msgBox.exec();


                                                                           ui->textEdit->insertPlainText("ERROR Level 4: " + commandLvl4 + "\n");

                                                                           ui->textEdit->moveCursor (QTextCursor::End);
                                                                       }
                                                                       else
                                                                       {
            //                                                               ui->statusBar->showMessage("");
            //                                                               QMessageBox msgBox;
            //                                                               msgBox.setText("File saved.");
            //                                                               msgBox.exec();

                                                                            ui->textEdit->insertPlainText("LEVEL 4: " + commandLvl4 +"\n");

                                                                            ui->textEdit->moveCursor (QTextCursor::End);
                                                                       }







                                    //00000000000




                                    //111111111111111

}

                           }

                    }



               }


}



}








       }

//main msg

    }else
    {
     // do nothing
    }






}else
{
 // do nothing
}

}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




//convert anyname.png convert.jpg
//for one file per folder, including 3 levels of subfolders, e.g. as preparation of 1)
void MainWindow::openpng()
{

   QString hpath = QDir::homePath();

   QString savefilePath = QFileDialog::getExistingDirectory(this, tr("Select folder"));

 //    savefileName = QFileDialog::getSaveFileName(this, tr("Save File"));



   // qDebug() << "--------------savefileName ----------- " << savefilePath ;

    ui->plainTextEdit->setPlainText(savefilePath);


    if(savefilePath!="")
        {

    QFileInfoList FileInfoList;

    //main msg

    QMessageBox msgBox;
    msgBox.setWindowTitle("Are you sure?");
    msgBox.setText("Batch processing for the selected folder including sub folders?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {






    //msg

   //  oppath = QFileInfo(vfilePath).path();

    QDir dir(savefilePath);


//Level 1 +++++++++++++++++++++++++

    dir.setNameFilters(QStringList("*.png"));
       dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

      // qDebug() << "Scanning FOLDER LEVEL 1 " << dir.path();

       QStringList fileList = dir.entryList();
       for (int i=0; i<fileList.count(); i++)
       {


              qDebug() << "cccccccccccccccccccccconvert --------- Found file LEVEL 1: fileList[i] " << savefilePath << fileList[i] << " number  "  << i;

//                qDebug() << "name of folder LEVEL 1 " << savefilePath;

//                             qDebug() << "<<<<<<<<<<<<<<<< warning: image is inside of the root folder - not an album folder - no changes!";

                             QString warn = "LEVEL 1: png image is inside of the root folder - no changes!";


                             ui->textEdit->insertPlainText(warn+"\n");

                             ui->textEdit->moveCursor (QTextCursor::End);



       }


//Lelvel 2 ++++convert++++++++++++++++++++

       dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
       QStringList dirList = dir.entryList();
       for (int i2=0; i2<dirList.size(); ++i2)
       {
           QString newPathL2 = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i2));
           //scanDir(QDir(newPath));

            qDebug() << "Level 2 Path newPathL2  " << newPathL2 << " i2 " << i2 << " <dirList.size() " << dirList.size();



             QString albumnameL2 =QFileInfo(newPathL2).fileName();

            // qDebug() << "++++++++++++++++ name of album LEVEL 2 " << albumnameL2;

            //

            QDir newdirL2(newPathL2);


            newdirL2.setNameFilters(QStringList("*.png"));
               newdirL2.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

             //  qDebug() << "Scanning FOLDER LEVEL 2: " << newdirL2.path();


               QStringList newfileListL2 = newdirL2.entryList();


              for (int i22=0; i22<newfileListL2.count(); i22++)
               {
                   //- only the first jpg is used as cover jpg
                   if(i22==0)
                   {

                       QString namefirstfileL2 = newfileListL2[i22];
                      // QString albumplusjpgL2 = albumnameL2 + ".png";
                         QString albumplusjpgL2 =  "convert.jpg";

//                       qDebug() << "LEVEL 2 ??????????????????????????????????????????????????????????????????????????";

//                                   qDebug() << "newfileList[i]   " << namefirstfileL2;

//                                  qDebug() << "albumname   " << albumplusjpgL2;



                       // qDebug() << "??????????????????????????????????????????????????????????????????????????";



                        //if file with the name of the folder is not the first one
                                 if(namefirstfileL2!=albumplusjpgL2)

                         {

//
                      // qDebug() << "Found png file LEVEL 2: " << newfileListL2[i] << " in " << albumnameL2;

                     //  QString msg1L2 =  "LEVEL 2: Found png file: " + newfileListL2[i] + " in " + albumnameL2;




//                       ui->textEdit->insertPlainText(msg1+"\n");

//                       ui->textEdit->moveCursor (QTextCursor::End);

                       //jpegtran -perfect cover.jpg > "Missing Persons - Rhyme & Reason.jpg"

                       //QString commandLvl2 = "jpegtran -perfect '" + newPath + newfileList[i] + "' > '" + newPath + "'" ;

                       // QString commandLvl2 = "jpegtran -perfect '"  + newPathL2 + "/" + newfileListL2[i] + "' > '" + newPathL2 + "/" + albumnameL2 + ".jpg" + "'" ;

                        // QString commandLvl2 = "convert '"  + newPathL2 + "/" + newfileListL2[i] + "' '" + newPathL2 + "/" + albumnameL2 + ".jpg" + "'" ;

                         QString commandLvl2 = "convert '"  + newPathL2 + "/" + newfileListL2[i22] + "' '" + newPathL2 + "/" +  "convert.jpg" + "'" ;


                          // qDebug() << "NEW-------------------------------------------------- commandLvl2 : " << commandLvl2 ;

                        //convert cover.png cover.jpg

//                       ui->textEdit->insertPlainText(commandLvl2+"\n");

//                       ui->textEdit->moveCursor (QTextCursor::End);


                       //creating string for system

                       QByteArray arrayL2;
                                char*  newPathL2_;

                       arrayL2 = newPathL2.toLocal8Bit();
                                      newPathL2_ = arrayL2.data();

                      QByteArray array2L2;
                               char*  albumnameL2_;

                      array2L2 = albumnameL2.toLocal8Bit();
                                     albumnameL2_ = array2L2.data();



                        QString firstjpgL2 =  newfileListL2[i22];

                        QByteArray array3L2;
                                 char*  firstjpgL2_;

                        array3L2 = firstjpgL2.toLocal8Bit();
                                       firstjpgL2_ = array3L2.data();



//  QString commandLvl2 = "jpegtran -perfect '"  + newPath + "/" + newfileList[i] + "' > '" + newPath + "/" + albumname + ".jpg" + "'" ;


                       char result2L2[50000];

                                                   strcpy(result2L2,"convert '");
                                                   strcat(result2L2,newPathL2_);
                                                    strcat(result2L2,"/");
                                                    strcat(result2L2,firstjpgL2_);
                                                    strcat(result2L2,"' '");
                                                    strcat(result2L2,newPathL2_);
                                                    strcat(result2L2,"/");
                                                     //strcat(result2L2,albumnameL2_);
                                                    strcat(result2L2,"convert.jpg");
                                                    strcat(result2L2,"'");


//                                                    ui->textEdit->insertPlainText(result2+"\n");

//                                                    ui->textEdit->moveCursor (QTextCursor::End);






                                                           // qDebug() << "-------crop------------result2--------------- " << result2L2;


                                                            if(system(result2L2) != 0)
                                                           {
                                                              // qDebug() << "++++++++++++++++++++++++++++++++++++++++++++png+++++++convert failed..." ;

//                                                               ui->statusBar->showMessage("");
//                                                               QMessageBox msgBox;
//                                                               msgBox.setText("Error Level 2: jpegtran failed: " + albumnameL2);
//                                                               msgBox.exec();

                                                               ui->textEdit->insertPlainText("ERROR Level 2: " + commandLvl2 + "\n");

                                                               ui->textEdit->moveCursor (QTextCursor::End);
                                                           }
                                                           else
                                                           {


                                                                ui->textEdit->insertPlainText("LEVEL 2: " + commandLvl2+"\n");

                                                                ui->textEdit->moveCursor (QTextCursor::End);
//                                                               ui->statusBar->showMessage("");
//                                                               QMessageBox msgBox;
//                                                               msgBox.setText("File saved.");
//                                                               msgBox.exec();
                                                           }






                            }



                   }




               }


//Level 3 +++convert+++++++++++++++++++








               newdirL2.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
               QStringList dirList2L3 = newdirL2.entryList();
               for (int i3=0; i3<dirList2L3.size(); ++i3)
               {




                   //88888 if(i==0)
                  //88888 {
                   QString newPath2L3 = QString("%1/%2").arg(newdirL2.absolutePath()).arg(dirList2L3.at(i3));
                   //scanDir(QDir(newPath));



      qDebug() << "Level 3 Path newPath2L3  " << newPath2L3 << " i3 " << i3 << " <dirList.size() " << dirList2L3.size();

                  //  qDebug() << "Level 3 Path +++++++++++++++++++++ " << newPath2L3 ;

                    QString albumname2L3 =QFileInfo(newPath2L3).fileName();

                    //qDebug() << "++++++++++++++++ name of album " << albumname2L3;


                    QDir newdir2L3(newPath2L3);

                    newdir2L3.setNameFilters(QStringList("*.png"));
                       newdir2L3.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

                       //qDebug() << "Scanning FOLDER LEVEL 3: " << newdir2L3.path();


                       QStringList newfileList2L3 = newdir2L3.entryList();
                       for (int i33=0; i33<newfileList2L3.count(); i33++)
                       {


                           //24444


                                //- only the first png is used
                               if(i33==0)
                               {



                           //24444
                           QString namefirstfile3L3 = newfileList2L3[i33];
                         //  QString albumplusjpgL3 = albumname2L3 + ".jpg";
                             QString albumplusjpgL3 =  "convert.jpg";


                           qDebug() << "LEVEL 3 ##################??????????????????????????????????????????????????????????????????????????";

                                       qDebug() << "newfileList[i33]   " << namefirstfile3L3 << " i33 " << i33;

                                      qDebug() << "albumname + jpg  " << albumplusjpgL3;



                            qDebug() << "#######################??????????????????????????????????????????????????????????????????????????";



                   //if file with the name of the folder is not existing already:
                            if(namefirstfile3L3!=albumplusjpgL3)

                    {
        //
                             //  qDebug() << "§§§§§§§§§§§§§§§§§§§§§§§§§§§§ Found png file LEVEL 3: " << newfileList2L3[i] << " in " << albumname2L3;


                               //00000000000


                              // QString commandLvl3 = "jpegtran -perfect '"  + newPath2L3 + "/" + newfileList2L3[i] + "' > '" + newPath2L3 + "/" + albumname2L3 + ".jpg" + "'" ;


                                 QString commandLvl3 = "convert '"  + newPath2L3 + "/" + newfileList2L3[i33] + "' '" + newPath2L3 + "/" +  "convert.jpg" + "'" ;
//                              ui->textEdit->insertPlainText(commandLvl3+"\n");


                                 // qDebug() << "///////////////////§§§§§§§§§§§§§§§§§§§§§§§§§§§§ commandLvl3  LEVEL 3: " << commandLvl3 ;

//                              ui->textEdit->moveCursor (QTextCursor::End);


                              //creating string for system

                              QByteArray array11L3;
                                       char*  newPath_2L3;

                              array11L3 = newPath2L3.toLocal8Bit();
                                             newPath_2L3 = array11L3.data();

                             QByteArray array22L3;
                                      char*  albumname_2L3;

                             array22L3 = albumname2L3.toLocal8Bit();
                                            albumname_2L3 = array22L3.data();



                               QString firstjpg2L3 =  newfileList2L3[i33];

                               QByteArray array3L3;
                                        char*  firstjpg_2L3;

                               array3L3 = firstjpg2L3.toLocal8Bit();
                                              firstjpg_2L3 = array3L3.data();



       //  QString commandLvl2 = "jpegtran -perfect '"  + newPath + "/" + newfileList[i] + "' > '" + newPath + "/" + albumname + ".jpg" + "'" ;


                              char result3L3[50000];

                                                          strcpy(result3L3,"convert '");
                                                          strcat(result3L3,newPath_2L3);
                                                           strcat(result3L3,"/");
                                                           strcat(result3L3,firstjpg_2L3);
                                                           strcat(result3L3,"' '");
                                                           strcat(result3L3,newPath_2L3);
                                                           strcat(result3L3,"/");
                                                           // strcat(result3L3,albumname_2L3);
                                                           strcat(result3L3,"convert.jpg");
                                                           strcat(result3L3,"'");


       //                                                    ui->textEdit->insertPlainText(result2+"\n");

       //                                                    ui->textEdit->moveCursor (QTextCursor::End);






                                                                 //  qDebug() << "-------crop------------result2--------------- " << result3L3;


                                                                   if(system(result3L3) != 0)
                                                                  {
                                                                      //qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++jpegtran failed...";

//                                                                      ui->statusBar->showMessage("");
//                                                                      QMessageBox msgBox;
//                                                                      msgBox.setText("Error Level 3: jpegtran failed: " + albumname2);
//                                                                      msgBox.exec();

                                                                      ui->textEdit->insertPlainText("ERROR Level 3: " + commandLvl3 + "\n");

                                                                      ui->textEdit->moveCursor (QTextCursor::End);
                                                                  }
                                                                  else
                                                                  {
                                                                       ui->textEdit->insertPlainText("LEVEL 3: " + commandLvl3+"\n");

                                                                       ui->textEdit->moveCursor (QTextCursor::End);

       //                                                               ui->statusBar->showMessage("");
       //                                                               QMessageBox msgBox;
       //                                                               msgBox.setText("File saved.");
       //                                                               msgBox.exec();
                                                                  }

}


                    }


                               //00000000000


                      }






//Level 4+++++convert++++++++++++++++++++


                    newdir2L3.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
                    QStringList dirList3L4 = newdir2L3.entryList();
                    for (int i4=0; i4<dirList3L4.size(); ++i4)
                    {
                        QString newPath3L4 = QString("%1/%2").arg(newdir2L3.absolutePath()).arg(dirList3L4.at(i4));
                        //scanDir(QDir(newPath));

                        // qDebug() << "Level 4 Path +++++++++++++++++++++ " << newPath3L4 ;

                         //

                         QString albumname3L4 =QFileInfo(newPath3L4).fileName();

                       //  qDebug() << "++++++++++++++++ name of album " << albumname3L4;


                         QDir newdir3L4(newPath3L4);

                         //
                         QStringList filters;
                         // filters << "*.png" << "*.jpg";


                         //

                         newdir3L4.setNameFilters(QStringList("*.png"));
                        // newdir3L4.setNameFilters(QStringList(filters));
                            newdir3L4.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

                          //  qDebug() << "Scanning FOLDER LEVEL 4 for png: " << newdir3L4.path();


                            QStringList newfileList3L4 = newdir3L4.entryList();
                            for (int i44=0; i44<newfileList3L4.count(); i44++)
                            {
             //
                                //if - only first file is used

                                if(i44==0)
                                {

                                QString namefirstfile3L4 = newfileList3L4[i44];
                               // QString albumplusjpgL4 = albumname3L4 + ".png";

                                 QString albumplusjpgL4 = "convert.jpg";


                                qDebug() << "LEVEL 4 ##################??????????????????????????????????????????????????????????????????????????";

                                            qDebug() << i44 << "  newfileList[i44]   " << namefirstfile3L4 ;

                                           qDebug() << "albumname + jpg  " << albumplusjpgL4;



                                 qDebug() << "#######################??????????????????????????????????????????????????????????????????????????";



                        //if file with the name of the folder is not existing already:
                                 if(namefirstfile3L4!=albumplusjpgL4)

                         {









                                 //   qDebug() << "Found png file in LEVEL 4: " << newfileList3L4[i] << " in " << albumname3L4;


                                    //111111111111111




                                    //00000000000


                                    //QString commandLvl4 = "jpegtran -perfect '"  + newPath3L4 + "/" + newfileList3L4j[i] + "' > '" + newPath3L4 + "/" + albumname3L4 + ".jpg" + "'" ;

                                        QString commandLvl4 = "convert '"  + newPath3L4 + "/" + newfileList3L4[i44] + "' '" + newPath3L4 + "/" + "convert.jpg" + "'" ;

//                                   ui->textEdit->insertPlainText(commandLvl4+"\n");

//                                   ui->textEdit->moveCursor (QTextCursor::End);


                                   //creating string for system

                                   QByteArray array111L4;
                                            char*  newPath_3L4;

                                   array111L4 = newPath3L4.toLocal8Bit();
                                                  newPath_3L4 = array111L4.data();

                                  QByteArray array222L4;
                                           char*  albumname_3L4;

                                  array222L4 = albumname3L4.toLocal8Bit();
                                                 albumname_3L4 = array222L4.data();



                                    QString firstjpg3L4 =  newfileList3L4[i44];

                                    QByteArray array33L4;
                                             char*  firstjpg_3L4;

                                    array33L4 = firstjpg3L4.toLocal8Bit();
                                                   firstjpg_3L4 = array33L4.data();



            //  QString commandLvl2 = "jpegtran -perfect '"  + newPath + "/" + newfileList[i] + "' > '" + newPath + "/" + albumname + ".jpg" + "'" ;


                                   char result2L4[50000];

                                                               strcpy(result2L4,"convert '");
                                                               strcat(result2L4,newPath_3L4);
                                                                strcat(result2L4,"/");
                                                                strcat(result2L4,firstjpg_3L4);
                                                                strcat(result2L4,"' '");
                                                                strcat(result2L4,newPath_3L4);
                                                                strcat(result2L4,"/");
                                                                 //strcat(result2L4,albumname_3L4);
                                                                strcat(result2L4,"convert.jpg");
                                                                strcat(result2L4,"'");


            //                                                    ui->textEdit->insertPlainText(result2+"\n");

            //                                                    ui->textEdit->moveCursor (QTextCursor::End);






                                                                       // qDebug() << "-------crop------------result2--------------- " << result2L4;


                                                                        if(system(result2L4) != 0)
                                                                       {
                                                                           qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++jpegtran failed..." ;

//                                                                           ui->statusBar->showMessage("");
//                                                                           QMessageBox msgBox;
//                                                                           msgBox.setText("Error Level 4: jpegtran failed: " + albumname3L4);
//                                                                           msgBox.exec();


                                                                           ui->textEdit->insertPlainText("ERROR Level 4: " + commandLvl4 + "\n");

                                                                           ui->textEdit->moveCursor (QTextCursor::End);
                                                                       }
                                                                       else
                                                                       {
            //                                                               ui->statusBar->showMessage("");
            //                                                               QMessageBox msgBox;
            //                                                               msgBox.setText("File saved.");
            //                                                               msgBox.exec();

                                                                            ui->textEdit->insertPlainText("LEVEL 4: " + commandLvl4 +"\n");

                                                                            ui->textEdit->moveCursor (QTextCursor::End);
                                                                       }







                                    //00000000000




                                    //111111111111111

}

                           }

                    }

}

               //8888}


}












       }

//main msg

    }else
    {
     // do nothing
    }




}

}


//convert end

//##########################################





//convert all png files in a selected folder into jpg
//convert anyname.png anyname.pngconvert.jpg
void MainWindow::openpng_all()
{

   QString hpath = QDir::homePath();

   QString savefilePath = QFileDialog::getExistingDirectory(this, tr("Select folder"));

 //    savefileName = QFileDialog::getSaveFileName(this, tr("Save File"));



   // qDebug() << "--------------savefileName ----------- " << savefilePath ;

    ui->plainTextEdit->setPlainText(savefilePath);


    if(savefilePath!="")
        {

    QFileInfoList FileInfoList;

    //main msg

    QMessageBox msgBox;
    msgBox.setWindowTitle("Are you sure?");
    msgBox.setText("Batch processing for the selected folder?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {






    //msg

   //  oppath = QFileInfo(vfilePath).path();

    QDir dir(savefilePath);




    dir.setNameFilters(QStringList("*.png"));





//Only Lelvel  ++++convert++++++++++++++++++++

       dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

//       QStringList dirList = dir.entryList();
//       for (int i2=0; i2<dirList.size(); ++i2)
//       {
//           QString newPathL2 = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i2));
//           //scanDir(QDir(newPath));

//            qDebug() << "Level 2 Path newPathL2  " << newPathL2 << " i2 " << i2 << " <dirList.size() " << dirList.size();


       //changed:
       QString newPathL2=savefilePath;

             QString albumnameL2 =QFileInfo(newPathL2).fileName();

            // qDebug() << "++++++++++++++++ name of album LEVEL 2 " << albumnameL2;

            //

            QDir newdirL2(newPathL2);


            newdirL2.setNameFilters(QStringList("*.png"));
               newdirL2.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

             qDebug() << "Scanning FOLDER: " << newdirL2.path();


               QStringList newfileListL2 = newdirL2.entryList();


              for (int i22=0; i22<newfileListL2.count(); i22++)
               {
                   //- only the first jpg is used as cover jpg
                  // if(i22==0)
                  // {

                       QString namefirstfileL2 = newfileListL2[i22];
                      // QString albumplusjpgL2 = albumnameL2 + ".png";
                         QString albumplusjpgL2 =  "convert.jpg";

//                       qDebug() << "Only LEVEL ??????????????????????????????????????????????????????????????????????????";

//                                   qDebug() << "newfileList[i]   " << namefirstfileL2;

//                                  qDebug() << "albumname   " << albumplusjpgL2;



//                        qDebug() << "??????????????????????????????????????????????????????????????????????????";



                        //if file with the name of the folder is not the first one
                                 if(namefirstfileL2!=albumplusjpgL2)

                         {

//
                      // qDebug() << "Found png file LEVEL 2: " << newfileListL2[i] << " in " << albumnameL2;

                     //  QString msg1L2 =  "LEVEL 2: Found png file: " + newfileListL2[i] + " in " + albumnameL2;




//                       ui->textEdit->insertPlainText(msg1+"\n");

//                       ui->textEdit->moveCursor (QTextCursor::End);

                       //jpegtran -perfect cover.jpg > "Missing Persons - Rhyme & Reason.jpg"

                       //QString commandLvl2 = "jpegtran -perfect '" + newPath + newfileList[i] + "' > '" + newPath + "'" ;

                       // QString commandLvl2 = "jpegtran -perfect '"  + newPathL2 + "/" + newfileListL2[i] + "' > '" + newPathL2 + "/" + albumnameL2 + ".jpg" + "'" ;

                        // QString commandLvl2 = "convert '"  + newPathL2 + "/" + newfileListL2[i] + "' '" + newPathL2 + "/" + albumnameL2 + ".jpg" + "'" ;

                         QString commandLvl2 = "convert '"  + newPathL2 + "/" + newfileListL2[i22] + "' '" + newPathL2 + "/" + newfileListL2[i22] + "convert.jpg" + "'" ;


                          // qDebug() << "NEW-------------------------------------------------- commandLvl2 : " << commandLvl2 ;

                        //convert cover.png cover.jpg

//                       ui->textEdit->insertPlainText(commandLvl2+"\n");

//                       ui->textEdit->moveCursor (QTextCursor::End);


                       //creating string for system

                       QByteArray arrayL2;
                                char*  newPathL2_;

                       arrayL2 = newPathL2.toLocal8Bit();
                                      newPathL2_ = arrayL2.data();

                      QByteArray array2L2;
                               char*  albumnameL2_;

                      array2L2 = albumnameL2.toLocal8Bit();
                                     albumnameL2_ = array2L2.data();



                        QString firstjpgL2 =  newfileListL2[i22];

                        QByteArray array3L2;
                                 char*  firstjpgL2_;

                        array3L2 = firstjpgL2.toLocal8Bit();
                                       firstjpgL2_ = array3L2.data();



//  QString commandLvl2 = "jpegtran -perfect '"  + newPath + "/" + newfileList[i] + "' > '" + newPath + "/" + albumname + ".jpg" + "'" ;


                       char result2L2[50000];

                                                   strcpy(result2L2,"convert '");
                                                   strcat(result2L2,newPathL2_);
                                                    strcat(result2L2,"/");
                                                    strcat(result2L2,firstjpgL2_);
                                                    strcat(result2L2,"' '");
                                                    strcat(result2L2,newPathL2_);
                                                    strcat(result2L2,"/");
                                                     //strcat(result2L2,albumnameL2_);
                                                     strcat(result2L2,firstjpgL2_);
                                                    strcat(result2L2,"convert.jpg");
                                                    // strcat(result2L2,"convert.jpg");
                                                    strcat(result2L2,"'");


//                                                    ui->textEdit->insertPlainText(result2+"\n");

//                                                    ui->textEdit->moveCursor (QTextCursor::End);






                                                         qDebug() << "-------crop------------result2--------------- " << result2L2;


                                                            if(system(result2L2) != 0)
                                                           {
                                                              // qDebug() << "++++++++++++++++++++++++++++++++++++++++++++png+++++++convert failed..." ;

//                                                               ui->statusBar->showMessage("");
//                                                               QMessageBox msgBox;
//                                                               msgBox.setText("Error Level 2: jpegtran failed: " + albumnameL2);
//                                                               msgBox.exec();

                                                               ui->textEdit->insertPlainText("ERROR: " + commandLvl2 + "\n");

                                                               ui->textEdit->moveCursor (QTextCursor::End);
                                                           }
                                                           else
                                                           {


                                                                ui->textEdit->insertPlainText(commandLvl2+"\n");

                                                                ui->textEdit->moveCursor (QTextCursor::End);
//                                                               ui->statusBar->showMessage("");
//                                                               QMessageBox msgBox;
//                                                               msgBox.setText("File saved.");
//                                                               msgBox.exec();
                                                           }






                            }








               }








//}

















       }

//main msg

    }else
    {
     // do nothing
    }




}




//convert end


//all png end





MainWindow::~MainWindow()
{
    delete ui;
}

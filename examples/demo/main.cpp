/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of a Qt Solutions component.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#include <QApplication>
#include <QMessageBox>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QMap>
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "DataBall.h"


int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  QWidget *w = new QWidget();

  DataBall db = new DataBall();
  db.loadData();
  
  QString xCenter = "xCenter";
  QString yCenter = "yCenter";
  QString zCenter = "zCenter";
  
  // Save Load Functionality ------------------------------------------- START
  double x = loadData(xCenter);
  double y = loadData(yCenter);
  double z = loadData(zCenter);
  // Save Load Functionality --------------------------------------------- END
  
  qDebug() << "Loaded Data is: " << x << ", " << y << ", " << z;

  // These are the controls for the double and the group
  QtDoublePropertyManager *doubleManager = new QtDoublePropertyManager(w);
  QtGroupPropertyManager *groupManager = new QtGroupPropertyManager(w);

  // This is a top-level group field. We may want to make a mid-level group field
  QtProperty *item0 = groupManager->addProperty("Double Land");
  
  // These are data fields
  QtProperty *item1 = doubleManager->addProperty("X");
  // Set the value via the manager - pass in the property and the value
  doubleManager->setValue(item1, x);
  // You can name your fields- this will be important so you know where
  // the data is coming from when you deal with signals- it's also important
  // to have unique names!
  item1->setPropertyName(xCenter);
  item0->addSubProperty(item1);
  
  
  QtProperty *item2 = doubleManager->addProperty("Y");
  doubleManager->setValue(item2, y);
  item2->setPropertyName(yCenter);
  item0->addSubProperty(item2);
  
  
  QtProperty *item3 = doubleManager->addProperty("Z");
  doubleManager->setValue(item3, z);
  item3->setPropertyName(zCenter);
  item0->addSubProperty(item3);

  // This is how we edit the field
  QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(w);
  
  QtAbstractPropertyBrowser *editor1 = new QtTreePropertyBrowser();
  // Here we define the controls for the object- adding a factory to the object means that we can 
  // control this field and make changes to it.
  editor1->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
  editor1->addProperty(item0);

  // Notice no factory here - This becomes read-only.
  QtAbstractPropertyBrowser *editor2 = new QtTreePropertyBrowser();
  editor2->addProperty(item0);

  // This is editable as well. We're using the doubleManager variable as the point of connection with
  // the factory. Anything with an attachment to that mananger obj means that it should react the same way
  // as everything else with that same factory. 
  // Factory located inside fo qteditorfactory.cpp
  QtAbstractPropertyBrowser *editor3 = new QtGroupBoxPropertyBrowser();
  editor3->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
  editor3->addProperty(item0);

  QScrollArea *scroll3 = new QScrollArea();
  scroll3->setWidgetResizable(true);
  scroll3->setWidget(editor3);

  // Graphics Icky-stickity
  // We're laying everything out in a grid
  QGridLayout *layout = new QGridLayout(w);
  
  // labels
  QLabel *label1 = new QLabel("Editable Tree Property Browser");
  QLabel *label2 = new QLabel("Read Only Tree Property Browser, editor factories are not set");
  QLabel *label3 = new QLabel("Group Box Property Browser");
  
  // looks
  label1->setWordWrap(true);
  label2->setWordWrap(true);
  label3->setWordWrap(true);

  // looks
  label1->setFrameShadow(QFrame::Sunken);
  label2->setFrameShadow(QFrame::Sunken);
  label3->setFrameShadow(QFrame::Sunken);

  // looks
  label1->setFrameShape(QFrame::Panel);
  label2->setFrameShape(QFrame::Panel);
  label3->setFrameShape(QFrame::Panel);
  
  // looks
  label1->setAlignment(Qt::AlignCenter);
  label2->setAlignment(Qt::AlignCenter);
  label3->setAlignment(Qt::AlignCenter);
  
  // add labels to layout scheme. 
  //                  OBJ  ROW,COL
  layout->addWidget(label1, 0, 0);
  layout->addWidget(label2, 0, 1);
  layout->addWidget(label3, 0, 2);
  
  // add editor objects to layout scheme. 
  //                  OBJ  ROW,COL    
  layout->addWidget(editor1, 1, 0);
  layout->addWidget(editor2, 1, 1);
  layout->addWidget(scroll3, 1, 2);
  
  w->show();

  int ret = app.exec();
  delete w;
  return ret;
}

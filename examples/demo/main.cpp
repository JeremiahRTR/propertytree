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
#include "QtPropertyManager.h"
#include "QtEditorFactory.h"
#include "QtTreePropertyBrowser.h"
#include "QtButtonPropertyBrowser.h"
#include "QtGroupBoxPropertyBrowser.h"
#include "DataBall.h"
#include "FilePathManager.h"
#include "FileEditFactory.h"
#include "FileEdit.h"


int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  QWidget *w = new QWidget();

  DataBall *db = new DataBall();
  // get the data from a file
  db->LoadData();
  
  QString fileRobotURDF = "fileRobotURDF";
  QString fileNodes = "fileNodes";
  QString fileEdges = "fileEdges";
  
  QString enumViewMode = "enumViewMode";
  QString enumLinkMeshMode = "enumLinkMeshMode";
  QString boolvoxelVisiblity = "boolVoxelVisiblity";
  QString intVoxelIndex = "intVoxelIndex";
  QString roStringEntry = "roStringEntry";
  
  QString xPosition = "doubleXPosition";
  QString yPosition = "doubleYPosition";
  QString zPosition = "doubleZPosition";
  QString xDimension = "doubleXDimension";
  QString yDimension = "doubleYDimension";
  QString zDimension = "doubleZDimension";  
  
  QString fileLoadRegion = "fileLoadRegion";
  QString boolRegionVisible = "boolRegionVisible";
  QString enumVoxelFrame = "enumVoxelFrame";
  QString roStringVoxDim = "roStringVoxDim";
  
  QString enumBaseLinkFrame = "enumBaseLinkFrame";
  QString enumEndEffectorFrame = "enumEndEffectorFrame";
  
  QString intThreads = "intThreads";
  
  // These are the controls for the double and the group
  QtBoolPropertyManager   *boolManager  = new QtBoolPropertyManager(w);
  QtIntPropertyManager    *intManager   = new QtIntPropertyManager(w);
  QtStringPropertyManager *stringManager= new QtStringPropertyManager(w);
  QtDoublePropertyManager *doubleManager= new QtDoublePropertyManager(w);
  QtEnumPropertyManager   *enumManager  = new QtEnumPropertyManager(w);
  QtGroupPropertyManager  *groupManager = new QtGroupPropertyManager(w);  
  FilePathManager *filePathManager = new FilePathManager;

  // ---------------------------------------------------------------------------
  QtProperty *fileInputsRegion = groupManager->addProperty("File Inputs");
  
  QtProperty *robotURDF = filePathManager->addProperty("Robot URDF");
  filePathManager->setValue(robotURDF, "main.urdf");
  filePathManager->setInternalName(fileRobotURDF);
  filePathManager->setFilter(robotURDF, "Robot files (*.urdf)");
  fileInputsRegion->addSubProperty(robotURDF);
  
  QtProperty *robotNodes = filePathManager->addProperty("Nodes File");
  filePathManager->setValue(robotNodes, "nodes.txt");
  //~ filePathManager->setInternalName(fileNodes);
  filePathManager->setFilter(robotNodes, "Nodes files (*.txt)");
  fileInputsRegion->addSubProperty(robotNodes);
  
  QtProperty *robotEdges = filePathManager->addProperty("Edges File");
  filePathManager->setValue(robotEdges, "edges.txt");
  //~ filePathManager->setInternalName(fileNodes);
  filePathManager->setFilter(robotEdges, "Edges files (*.txt)");
  fileInputsRegion->addSubProperty(robotEdges);

  // ---------------------------------------------------------------------------
  QtProperty *viewingRegion = groupManager->addProperty("Viewing");
  
  QtProperty *viewMode = enumManager->addProperty("View Mode");
  viewMode->setInternalName(enumViewMode);
  enumManager->setEnumNames(viewMode, db->viewMode);
  viewingRegion->addSubProperty(viewMode);
  
  QtProperty *lMMode = enumManager->addProperty("Link Mesh Mode");
  lMMode->setInternalName(enumLinkMeshMode);
  enumManager->setEnumNames(lMMode, db->linkMeshMode);
  viewingRegion->addSubProperty(lMMode);
  
  QtProperty *voxelVis = boolManager->addProperty("Voxel Visiblity");
  boolManager->setValue(voxelVis, db->voxelVisible);
  voxelVis->setInternalName(boolvoxelVisiblity);
  viewingRegion->addSubProperty(voxelVis);
  
  QtProperty *voxelVisibility = intManager->addProperty("Index");
  intManager->setValue(voxelVisibility, db->voxelVisible);
  voxelVisibility->setInternalName(intVoxelIndex);
  viewingRegion->addSubProperty(voxelVisibility);
  
  QtProperty *entry = stringManager->addProperty("Entry");
  stringManager->setValue(entry, db->entryReadOnly);
  entry->setInternalName(roStringEntry);
  viewingRegion->addSubProperty(entry);
  
  // ---------------------------------------------------------------------------  
  QtProperty *voxelRegion = groupManager->addProperty("Voxel Region");
  
  QtProperty *loadRegion = filePathManager->addProperty("Edges File");
  filePathManager->setValue(loadRegion, "region.rgn");
  filePathManager->setInternalName(fileLoadRegion);
  //~ filePathManager->setFilter(loadRegion, "Edges files (*.txt)");
  voxelRegion->addSubProperty(loadRegion);

  QtProperty *regionVis = boolManager->addProperty("Visiblity");
  boolManager->setValue(regionVis, db->regionVisible);
  regionVis->setInternalName(boolRegionVisible);
  voxelRegion->addSubProperty(regionVis);
  
  QtProperty *voxelFrame = enumManager->addProperty("Frame");
  voxelFrame->setInternalName(enumVoxelFrame);
  enumManager->setEnumNames(voxelFrame, db->voxelFrame);
  voxelRegion->addSubProperty(voxelFrame);
  
  ///------ VECTOR3
  // This is a top-level group field. We may want to make a mid-level group field
  QtProperty *position = groupManager->addProperty("Position");
  
  // These are data fields
  QtProperty *positionX = doubleManager->addProperty("X");
  // Set the value via the manager - pass in the property and the value
  doubleManager->setValue(positionX, db->positionVec3.x);
  // You can name your fields- this will be important so you know where
  // the data is coming from when you deal with signals- it's also important
  // to have unique names!
  positionX->setInternalName(xPosition);
  position->addSubProperty(positionX);
  
  QtProperty *positionY = doubleManager->addProperty("Y");
  doubleManager->setValue(positionY, db->positionVec3.y);
  positionY->setInternalName(yPosition);
  position->addSubProperty(positionY);
  
  QtProperty *positionZ = doubleManager->addProperty("Z");
  doubleManager->setValue(positionZ, db->positionVec3.z);
  positionZ->setInternalName(zPosition);
  position->addSubProperty(positionZ);

  voxelRegion->addSubProperty(position);

  ///------ VECTOR3
  // This is a top-level group field. We may want to make a mid-level group field
  QtProperty *dim = groupManager->addProperty("Dimensions");
  
  // These are data fields
  QtProperty *dimX = doubleManager->addProperty("X");
  // Set the value via the manager - pass in the property and the value
  doubleManager->setValue(dimX, db->dimensionsVec3.x);
  // You can name your fields- this will be important so you know where
  // the data is coming from when you deal with signals- it's also important
  // to have unique names!
  dimX->setInternalName(xDimension);
  dim->addSubProperty(dimX);
  
  QtProperty *dimY = doubleManager->addProperty("Y");
  doubleManager->setValue(dimY, db->dimensionsVec3.y);
  dimY->setInternalName(yDimension);
  dim->addSubProperty(dimY);
  
  QtProperty *dimZ = doubleManager->addProperty("Z");
  doubleManager->setValue(dimZ, db->dimensionsVec3.z);
  dimZ->setInternalName(zDimension);
  dim->addSubProperty(dimZ);
  
  voxelRegion->addSubProperty(dim);

  QtProperty *voxDim = stringManager->addProperty("Voxel Dimensions");
  QString output = QObject::tr("%1, %2, %3")
                      .arg(QString::number(db->dimensionsVec3.x, 'f', 2))
                      .arg(QString::number(db->dimensionsVec3.y, 'f', 2))
                      .arg(QString::number(db->dimensionsVec3.z, 'f', 2));
  stringManager->setValue(voxDim, output);
  entry->setInternalName(roStringVoxDim);
  voxelRegion->addSubProperty(voxDim);

  // ---------------------------------------------------------------------------
  QtProperty *linkRegion = groupManager->addProperty("LinkPoses");

  QtProperty *blFrame  = enumManager->addProperty("Base Link Frame");
  viewMode->setInternalName(enumBaseLinkFrame);
  enumManager->setEnumNames(blFrame, db->baseLinkFrame);
  linkRegion->addSubProperty(blFrame);
  
  QtProperty *eeFrame = enumManager->addProperty("End-effector Frame");
  eeFrame->setInternalName(enumEndEffectorFrame);
  enumManager->setEnumNames(eeFrame, db->endEffectorFrame);
  linkRegion->addSubProperty(eeFrame);

  // ---------------------------------------------------------------------------
  // int number of threads
  QtProperty *threads = intManager->addProperty("Threads");
  intManager->setValue(threads, db->threads);
  threads->setInternalName(intThreads);


  // This is how we edit the field
  QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(w);
  QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(w);
  QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(w);
  //~ QtLineEditFactory *lineEditFactory = new QtLineEditFactory(w);  
  QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(w);
  FileEditFactory *fileEditFactory = new FileEditFactory;
  
  
  QtAbstractPropertyBrowser *editor1 = new QtTreePropertyBrowser();
  // Here we define the controls for the object- adding a factory to the object means that we can 
  // control this field and make changes to it.
  editor1->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
  editor1->setFactoryForManager(intManager, spinBoxFactory);
  editor1->setFactoryForManager(enumManager, comboBoxFactory);
  //~ editor1->setFactoryForManager(stringManager, lineEditFactory);
  editor1->setFactoryForManager(boolManager, checkBoxFactory);
  editor1->setFactoryForManager(filePathManager, fileEditFactory);
  
  editor1->addProperty(fileInputsRegion);
  editor1->addProperty(viewingRegion);
  editor1->addProperty(voxelRegion);
  editor1->addProperty(linkRegion);
  editor1->addProperty(threads);
  
  // Notice no factory here - This becomes read-only.
  QtAbstractPropertyBrowser *editor2 = new QtTreePropertyBrowser();
  editor2->addProperty(fileInputsRegion);
  editor2->addProperty(viewingRegion);
  editor2->addProperty(voxelRegion);
  editor2->addProperty(linkRegion);
  editor2->addProperty(threads);
  
  // This is editable as well. We're using the doubleManager variable as the point of connection with
  // the factory. Anything with an attachment to that mananger obj means that it should react the same way
  // as everything else with that same factory. 
  // Factory located inside fo qteditorfactory.cpp
  QtAbstractPropertyBrowser *editor3 = new QtGroupBoxPropertyBrowser();
  editor3->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
  editor3->setFactoryForManager(intManager, spinBoxFactory);
  editor3->setFactoryForManager(enumManager, comboBoxFactory);
  //~ editor3->setFactoryForManager(stringManager, lineEditFactory);
  editor3->setFactoryForManager(boolManager, checkBoxFactory);
  
  editor3->addProperty(fileInputsRegion);
  editor3->addProperty(viewingRegion);
  editor3->addProperty(voxelRegion);
  editor3->addProperty(linkRegion); 
  editor3->addProperty(threads);  
  
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

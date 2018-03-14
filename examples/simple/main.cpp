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
#include <QDate>
#include <QLocale>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();

    QLatin1String offset = QLatin1String("  ");
  
    //---- File Inputs
    QtProperty *fileInputs = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String("File Inputs"));
  
    //~ // Robot URDF - FILEPATH - @TODO
    //~ QtVariantProperty *fItem = variantManager->addProperty(QVariant::String, offset + QLatin1String("Robot URDF"));
    //~ fItem->setValue("FILEPATH GOES HERE");
    //~ fItem->setAttribute(QLatin1String("readOnly"), true);
    //~ fileInputs->addSubProperty(fItem);
  
    //~ // Nodes File - FILEPATH - @TODO
    //~ fItem = variantManager->addProperty(QVariant::String, offset + QLatin1String("Nodes File"));
    //~ fItem->setValue("FILEPATH GOES HERE");
    //~ fItem->setAttribute(QLatin1String("readOnly"), true);
    //~ fileInputs->addSubProperty(fItem);
  
    //~ // Edges File - FILEPATH - @TODO
    //~ fItem = variantManager->addProperty(QVariant::String, offset + QLatin1String("Edges File"));
    //~ fItem->setValue("FILEPATH GOES HERE");
    //~ fItem->setAttribute(QLatin1String("readOnly"), true);
    //~ fileInputs->addSubProperty(fItem);

    //~ //---- Viewing -------------------------------------------------------------
    //~ QtProperty *viewInputs = variantManager->addProperty(
            //~ QtVariantPropertyManager::groupTypeId(), QLatin1String("Viewing"));

    //~ // View mode - ENUM
    //~ QtVariantProperty *viewItem = variantManager->addProperty(
            //~ QtVariantPropertyManager::enumTypeId(), offset + QLatin1String("View Mode"));
    //~ QStringList enumViews;
    //~ enumViews << "Enum0" << "Enum1" << "Enum2";
    //~ viewItem->setAttribute(QLatin1String("enumNames"), enumViews);
    //~ viewItem->setValue(2);
    //~ viewInputs->addSubProperty(viewItem);
  
    //~ // Link Mesh mode - ENUM
    //~ viewItem = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), 
        //~ offset + QLatin1String("Link Mesh Mode"));
    //~ QStringList enumNamesMesh;
    //~ enumNamesMesh << "Mesh A" << "Mesh B" << "Mesh C";
    //~ viewItem->setAttribute(QLatin1String("enumNames"), enumNamesMesh);
    //~ viewItem->setValue(1);
    //~ viewInputs->addSubProperty(viewItem);
   
    //~ // Voxel Visibility - BOOL
    //~ viewItem = variantManager->addProperty(QVariant::Bool, offset + QLatin1String("Voxel Visibility"));
    //~ viewItem->setValue(false);
    //~ viewInputs->addSubProperty(viewItem);
  
    //~ // Index - INT
    //~ viewItem = variantManager->addProperty(QVariant::Int, offset + QLatin1String("Index"));
    //~ viewItem->setValue(20);
    //~ viewItem->setAttribute(QLatin1String("minimum"), 0);
    //~ viewItem->setAttribute(QLatin1String("maximum"), 100);
    //~ viewItem->setAttribute(QLatin1String("singleStep"), 1);
    //~ viewInputs->addSubProperty(viewItem);
  
    //~ // Entry - READ ONLY STRING
    //~ viewItem = variantManager->addProperty(QVariant::String, offset + QLatin1String("Entry"));
    //~ viewItem->setValue("Read-only string");
    //~ viewItem->setAttribute(QLatin1String("readOnly"), true);
    //~ viewInputs->addSubProperty(viewItem);
  
  
  
  
    //~ //---- Voxel Region
    //~ QtProperty *voxelInputs = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                //~ QLatin1String("Voxel Region"));

    //~ // Load Region - FILEPATH
    //~ QtVariantProperty *voxelItem = variantManager->addProperty(QVariant::String, offset + QLatin1String("Load region"));
    //~ voxelItem->setValue("FILEPATH GOES HERE");
    //~ voxelItem->setAttribute(QLatin1String("readOnly"), true);
    //~ voxelInputs->addSubProperty(voxelItem);
    
    //~ // Visibility - BOOL
    //~ voxelItem = variantManager->addProperty(QVariant::Bool, offset + QLatin1String("Visibility"));
    //~ voxelItem->setValue(false);
    //~ voxelInputs->addSubProperty(voxelItem);
  
    //~ // Frame - ENUM
    //~ voxelItem = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(),
                    //~ offset + QLatin1String("Frame"));
    //~ QStringList enumFrame;
    //~ enumFrame << "Frame A" << "Frame B" << "Frame C";
    //~ voxelItem->setAttribute(QLatin1String("enumFrame"), enumFrame);
    //~ voxelItem->setValue(1);
    //~ voxelInputs->addSubProperty(voxelItem);
  
    //~ //Center - VECTOR
  
  
    //~ //Dimensions - VECTOR
  
  
    //~ //Voxel Dimensions - READ ONLY STRING
    //~ voxelItem = variantManager->addProperty(QVariant::String, offset + QLatin1String("Voxel Dimensions"));
    //~ voxelItem->setValue("Read-only string");
    //~ voxelItem->setAttribute(QLatin1String("readOnly"), true);
    //~ voxelInputs->addSubProperty(voxelItem);
  
  
  
  //~ //---- Voxel Region
    //~ QtProperty *linkPoseInputs = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                //~ QLatin1String("Link Poses"));

    //~ // Base Link Frame - ENUM
    //~ QtVariantProperty *linkItem = variantManager->addProperty(
            //~ QtVariantPropertyManager::enumTypeId(), offset + QLatin1String("Base Link Frame"));
    //~ QStringList blfNames;
    //~ blfNames << "Enum0" << "Enum1" << "Enum2";
    //~ linkItem->setAttribute(QLatin1String("enumNames"), blfNames);
    //~ linkItem->setValue(2);
    //~ linkPoseInputs->addSubProperty(linkItem);
  
    //~ // End-effector Frame - ENUM
    //~ linkItem = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), 
        //~ offset + QLatin1String("End-effector Frame"));
    //~ QStringList eefNames;
    //~ eefNames << "Mesh A" << "Mesh B" << "Mesh C";
    //~ linkItem->setAttribute(QLatin1String("enumNames"), eefNames);
    //~ linkItem->setValue(1);
    //~ linkPoseInputs->addSubProperty(linkItem);
  
  
  
    //~ // Everything else
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

    QtTreePropertyBrowser *variantEditor = new QtTreePropertyBrowser();
    variantEditor->setFactoryForManager(variantManager, variantFactory);
  
  
    //~ // Add each section - order matters!
    variantEditor->addProperty(fileInputs);
    //~ variantEditor->addProperty(viewInputs);
    //~ variantEditor->addProperty(voxelInputs);
    //~ variantEditor->addProperty(linkPoseInputs);
    
    variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(false);

    variantEditor->show();

    int ret = app.exec();

    delete variantManager;
    delete variantFactory;
    delete variantEditor;

    return ret;
}

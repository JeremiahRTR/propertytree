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


#ifndef QTEDITORFACTORY_H
#define QTEDITORFACTORY_H

#include "qtpropertymanager.h"

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class QtDoubleSpinBoxFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtDoubleSpinBoxFactory : public QtAbstractEditorFactory<QtDoublePropertyManager>
{
  Q_OBJECT
public:
  QtDoubleSpinBoxFactory(QObject *parent = 0);
  ~QtDoubleSpinBoxFactory();

  // this is here so we can set values.
  //I think this should be done with an emit instead, but I'm not certain what that would look like right now
  // @todo - remove this, and replace with an emit of a signal to a slot in the main class.
  //~ void setValue(QtProperty *property, double value);
protected:
  void connectPropertyManager(QtDoublePropertyManager *manager);
  QWidget *createEditor(QtDoublePropertyManager *manager, QtProperty *property,
              QWidget *parent);
  void disconnectPropertyManager(QtDoublePropertyManager *manager);
  
private:
  QtDoubleSpinBoxFactoryPrivate *d_ptr;
  Q_DECLARE_PRIVATE(QtDoubleSpinBoxFactory)
  Q_DISABLE_COPY(QtDoubleSpinBoxFactory)
  Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, double))
  Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, double, double))
  Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
  Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
  Q_PRIVATE_SLOT(d_func(), void slotReadOnlyChanged(QtProperty *, bool))
  Q_PRIVATE_SLOT(d_func(), void slotSetValue(double))
  Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif

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


#include "qteditorfactory.h"
#include "qtpropertybrowserutils_p.h"
#include <QSpinBox>
#include <QScrollBar>
#include <QComboBox>
#include <QAbstractItemView>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QMenu>
#include <QKeyEvent>
#include <QApplication>
#include <QLabel>
#include <QToolButton>
#include <QColorDialog>
#include <QFontDialog>
#include <QSpacerItem>
#include <QStyleOption>
#include <QPainter>
#include <QtCore/QMap>

#include "fileHandler.h"

// Debug stuff
#include <QMessageBox>
#include <QObject>
#include <QDebug>

#if defined(Q_CC_MSVC)
#    pragma warning(disable: 4786) /* MS VS 6: truncating debug info after 255 characters */
#endif

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

// Set a hard coded left margin to account for the indentation
// of the tree view icon when switching to an editor

static inline void setupTreeViewEditorMargin(QLayout *lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    else
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
}

// ---------- EditorFactoryPrivate :
// Base class for editor factory private classes. Manages mapping of properties to editors and vice versa.

template <class Editor>
class EditorFactoryPrivate
{
public:

    typedef QList<Editor *> EditorList;
    typedef QMap<QtProperty *, EditorList> PropertyToEditorListMap;
    typedef QMap<Editor *, QtProperty *> EditorToPropertyMap;

    Editor *createEditor(QtProperty *property, QWidget *parent);
    void initializeEditor(QtProperty *property, Editor *e);
    void slotEditorDestroyed(QObject *object);

    PropertyToEditorListMap  m_createdEditors;
    EditorToPropertyMap m_editorToProperty;
};

template <class Editor>
Editor *EditorFactoryPrivate<Editor>::createEditor(QtProperty *property, QWidget *parent)
{
    Editor *editor = new Editor(parent);
    initializeEditor(property, editor);
    return editor;
}

template <class Editor>
void EditorFactoryPrivate<Editor>::initializeEditor(QtProperty *property, Editor *editor)
{
    typename PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(property, EditorList());
    it.value().append(editor);
    m_editorToProperty.insert(editor, property);
}

template <class Editor>
void EditorFactoryPrivate<Editor>::slotEditorDestroyed(QObject *object)
{
    const typename EditorToPropertyMap::iterator ecend = m_editorToProperty.end();
    for (typename EditorToPropertyMap::iterator itEditor = m_editorToProperty.begin(); itEditor !=  ecend; ++itEditor) {
        if (itEditor.key() == object) {
            Editor *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            const typename PropertyToEditorListMap::iterator pit = m_createdEditors.find(property);
            if (pit != m_createdEditors.end()) {
                pit.value().removeAll(editor);
                if (pit.value().empty())
                    m_createdEditors.erase(pit);
            }
            m_editorToProperty.erase(itEditor);
            return;
        }
    }
}

// QtDoubleSpinBoxFactory

class QtDoubleSpinBoxFactoryPrivate : public EditorFactoryPrivate<QDoubleSpinBox>
{
    QtDoubleSpinBoxFactory *q_ptr;
    Q_DECLARE_PUBLIC(QtDoubleSpinBoxFactory)
public:

    void slotPropertyChanged(QtProperty *property, double value);
    void slotRangeChanged(QtProperty *property, double min, double max);
    void slotSingleStepChanged(QtProperty *property, double step);
    void slotDecimalsChanged(QtProperty *property, int prec);
    void slotReadOnlyChanged(QtProperty *property, bool readOnly);
    void slotSetValue(double value);
};

void QtDoubleSpinBoxFactoryPrivate::slotPropertyChanged(QtProperty *property, double value)
{
    QList<QDoubleSpinBox *> editors = m_createdEditors[property];
    QListIterator<QDoubleSpinBox *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) {
        QDoubleSpinBox *editor = itEditor.next();
        if (editor->value() != value) {
            editor->blockSignals(true);
            editor->setValue(value);
            editor->blockSignals(false);
        }
    }
    
    qDebug() << "slotPropertyChanged - " << property->internalName();
    
    // This would be where we hand off the data to the rest of the world
    // right now it just dumps it to a file on the hard drive so it can be "remembered"
    //~ saveData(property->propertyName(), value);
    
}

void QtDoubleSpinBoxFactoryPrivate::slotRangeChanged(QtProperty *property,
            double min, double max)
{
    if (!m_createdEditors.contains(property))
        return;

    QtDoublePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QList<QDoubleSpinBox *> editors = m_createdEditors[property];
    QListIterator<QDoubleSpinBox *> itEditor(editors);
    while (itEditor.hasNext()) {
        QDoubleSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setRange(min, max);
        editor->setValue(manager->value(property));
        editor->blockSignals(false);
    }
    
    qDebug() << "slotRangeChanged - " << property;
}

void QtDoubleSpinBoxFactoryPrivate::slotSingleStepChanged(QtProperty *property, double step)
{
    if (!m_createdEditors.contains(property))
        return;

    QtDoublePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QList<QDoubleSpinBox *> editors = m_createdEditors[property];
    QListIterator<QDoubleSpinBox *> itEditor(editors);
    while (itEditor.hasNext()) {
        QDoubleSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setSingleStep(step);
        editor->blockSignals(false);
    }
    
    qDebug() << "slotSingleStepChanged - " << property;
}

void QtDoubleSpinBoxFactoryPrivate::slotReadOnlyChanged( QtProperty *property, bool readOnly)
{
    if (!m_createdEditors.contains(property))
        return;

    QtDoublePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QListIterator<QDoubleSpinBox *> itEditor(m_createdEditors[property]);
    while (itEditor.hasNext()) {
        QDoubleSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setReadOnly(readOnly);
        editor->blockSignals(false);
    }
    
    qDebug() << "slotReadOnlyChanged - " << property->propertyName();
    
}

void QtDoubleSpinBoxFactoryPrivate::slotDecimalsChanged(QtProperty *property, int prec)
{
    if (!m_createdEditors.contains(property))
        return;

    QtDoublePropertyManager *manager = q_ptr->propertyManager(property);
    if (!manager)
        return;

    QList<QDoubleSpinBox *> editors = m_createdEditors[property];
    QListIterator<QDoubleSpinBox *> itEditor(editors);
    while (itEditor.hasNext()) {
        QDoubleSpinBox *editor = itEditor.next();
        editor->blockSignals(true);
        editor->setDecimals(prec);
        editor->setValue(manager->value(property));
        editor->blockSignals(false);
    }
    
    qDebug() << "slotDecimalsChanged - " << property->propertyName();
    
}

void QtDoubleSpinBoxFactoryPrivate::slotSetValue(double value)
{
    QObject *object = q_ptr->sender();
    const QMap<QDoubleSpinBox *, QtProperty *>::ConstIterator itcend = m_editorToProperty.constEnd();
    for (QMap<QDoubleSpinBox *, QtProperty *>::ConstIterator itEditor = m_editorToProperty.constBegin(); itEditor != itcend; ++itEditor) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            QtDoublePropertyManager *manager = q_ptr->propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
    }
    qDebug() << "slotSetValue - ";
    
}

/*! \class QtDoubleSpinBoxFactory

    \brief The QtDoubleSpinBoxFactory class provides QDoubleSpinBox
    widgets for properties created by QtDoublePropertyManager objects.

    \sa QtAbstractEditorFactory, QtDoublePropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtDoubleSpinBoxFactory::QtDoubleSpinBoxFactory(QObject *parent)
    : QtAbstractEditorFactory<QtDoublePropertyManager>(parent)
{
    d_ptr = new QtDoubleSpinBoxFactoryPrivate();
    d_ptr->q_ptr = this;

}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtDoubleSpinBoxFactory::~QtDoubleSpinBoxFactory()
{
    qDeleteAll(d_ptr->m_editorToProperty.keys());
    delete d_ptr;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDoubleSpinBoxFactory::connectPropertyManager(QtDoublePropertyManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, double)),
                this, SLOT(slotPropertyChanged(QtProperty *, double)));
    
    connect(manager, SIGNAL(rangeChanged(QtProperty *, double, double)),
                this, SLOT(slotRangeChanged(QtProperty *, double, double)));
    
    connect(manager, SIGNAL(singleStepChanged(QtProperty *, double)),
                this, SLOT(slotSingleStepChanged(QtProperty *, double)));
    
    connect(manager, SIGNAL(decimalsChanged(QtProperty *, int)),
                this, SLOT(slotDecimalsChanged(QtProperty *, int)));
    
    connect(manager, SIGNAL(readOnlyChanged(QtProperty *, bool)),
                this, SLOT(slotReadOnlyChanged(QtProperty *, bool)));
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget *QtDoubleSpinBoxFactory::createEditor(QtDoublePropertyManager *manager,
        QtProperty *property, QWidget *parent)
{
    QDoubleSpinBox *editor = d_ptr->createEditor(property, parent);
    editor->setSingleStep(manager->singleStep(property));
    editor->setDecimals(manager->decimals(property));
    editor->setRange(manager->minimum(property), manager->maximum(property));
    editor->setValue(manager->value(property));
    editor->setKeyboardTracking(false);
    editor->setReadOnly(manager->isReadOnly(property));
  
  
    connect(editor, SIGNAL(valueChanged(double)), this, SLOT(slotSetValue(double)));
    connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDoubleSpinBoxFactory::disconnectPropertyManager(QtDoublePropertyManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, double)),
                this, SLOT(slotPropertyChanged(QtProperty *, double)));
  
    disconnect(manager, SIGNAL(rangeChanged(QtProperty *, double, double)),
                this, SLOT(slotRangeChanged(QtProperty *, double, double)));
  
    disconnect(manager, SIGNAL(singleStepChanged(QtProperty *, double)),
                this, SLOT(slotSingleStepChanged(QtProperty *, double)));

    disconnect(manager, SIGNAL(decimalsChanged(QtProperty *, int)),
                this, SLOT(slotDecimalsChanged(QtProperty *, int)));

    disconnect(manager, SIGNAL(readOnlyChanged(QtProperty *, bool)),
                this, SLOT(slotReadOnlyChanged(QtProperty *, bool)));
}


#include "moc_qteditorfactory.cpp"
#include "qteditorfactory.moc"

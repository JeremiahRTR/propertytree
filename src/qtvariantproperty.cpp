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


#include "qtvariantproperty.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtCore/QDate>
#include <QtCore/QLocale>

#if defined(Q_CC_MSVC)
#    pragma warning(disable: 4786) /* MS VS 6: truncating debug info after 255 characters */
#endif

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class QtEnumPropertyType
{
};


class QtFlagPropertyType
{
};


class QtGroupPropertyType
{
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

Q_DECLARE_METATYPE(QtEnumPropertyType)
Q_DECLARE_METATYPE(QtFlagPropertyType)
Q_DECLARE_METATYPE(QtGroupPropertyType)

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

/*!
    Returns the type id for an enum property.

    Note that the property's value type can be retrieved using the
    valueType() function (which is QVariant::Int for the enum property
    type).

    \sa propertyType(), valueType()
*/
int QtVariantPropertyManager::enumTypeId()
{
    return qMetaTypeId<QtEnumPropertyType>();
}

/*!
    Returns the type id for a flag property.

    Note that the property's value type can be retrieved using the
    valueType() function (which is QVariant::Int for the flag property
    type).

    \sa propertyType(), valueType()
*/
int QtVariantPropertyManager::flagTypeId()
{
    return qMetaTypeId<QtFlagPropertyType>();
}

/*!
    Returns the type id for a group property.

    Note that the property's value type can be retrieved using the
    valueType() function (which is QVariant::Invalid for the group
    property type, since it doesn't provide any value).

    \sa propertyType(), valueType()
*/
int QtVariantPropertyManager::groupTypeId()
{
    return qMetaTypeId<QtGroupPropertyType>();
}

/*!
    Returns the type id for a icon map attribute.

    Note that the property's attribute type can be retrieved using the
    attributeType() function.

    \sa attributeType(), QtEnumPropertyManager::enumIcons()
*/
int QtVariantPropertyManager::iconMapTypeId()
{
    return qMetaTypeId<QtIconMap>();
}

typedef QMap<const QtProperty *, QtProperty *> PropertyMap;
Q_GLOBAL_STATIC(PropertyMap, propertyToWrappedProperty)

static QtProperty *wrappedProperty(QtProperty *property)
{
    return propertyToWrappedProperty()->value(property, 0);
}

class QtVariantPropertyPrivate
{
    QtVariantProperty *q_ptr;
public:
    QtVariantPropertyPrivate(QtVariantPropertyManager *m) : manager(m) {}

    QtVariantPropertyManager *manager;
};

/*!
    \class QtVariantProperty

    \brief The QtVariantProperty class is a convenience class handling
    QVariant based properties.

    QtVariantProperty provides additional API: A property's type,
    value type, attribute values and current value can easily be
    retrieved using the propertyType(), valueType(), attributeValue()
    and value() functions respectively. In addition, the attribute
    values and the current value can be set using the corresponding
    setValue() and setAttribute() functions.

    For example, instead of writing:

    \code
        QtVariantPropertyManager *variantPropertyManager;
        QtProperty *property;

        variantPropertyManager->setValue(property, 10);
    \endcode

    you can write:

    \code
        QtVariantPropertyManager *variantPropertyManager;
        QtVariantProperty *property;

        property->setValue(10);
    \endcode

    QtVariantProperty instances can only be created by the
    QtVariantPropertyManager class.

    \sa QtProperty, QtVariantPropertyManager, QtVariantEditorFactory
*/

/*!
    Creates a variant property using the given \a manager.

    Do not use this constructor to create variant property instances;
    use the QtVariantPropertyManager::addProperty() function
    instead.  This constructor is used internally by the
    QtVariantPropertyManager::createProperty() function.

    \sa QtVariantPropertyManager
*/
QtVariantProperty::QtVariantProperty(QtVariantPropertyManager *manager)
    : QtProperty(manager),  d_ptr(new QtVariantPropertyPrivate(manager))
{

}

/*!
    Destroys this property.

    \sa QtProperty::~QtProperty()
*/
QtVariantProperty::~QtVariantProperty()
{
    delete d_ptr;
}

/*!
    Returns the property's current value.

    \sa valueType(), setValue()
*/
QVariant QtVariantProperty::value() const
{
    return d_ptr->manager->value(this);
}

/*!
    Returns this property's value for the specified \a attribute.

    QtVariantPropertyManager provides a couple of related functions:
    \l{QtVariantPropertyManager::attributes()}{attributes()} and
    \l{QtVariantPropertyManager::attributeType()}{attributeType()}.

    \sa setAttribute()
*/
QVariant QtVariantProperty::attributeValue(const QString &attribute) const
{
    return d_ptr->manager->attributeValue(this, attribute);
}

/*!
    Returns the type of this property's value.

    \sa propertyType()
*/
int QtVariantProperty::valueType() const
{
    return d_ptr->manager->valueType(this);
}

/*!
    Returns this property's type.

    QtVariantPropertyManager provides several related functions:
    \l{QtVariantPropertyManager::enumTypeId()}{enumTypeId()},
    \l{QtVariantPropertyManager::flagTypeId()}{flagTypeId()} and
    \l{QtVariantPropertyManager::groupTypeId()}{groupTypeId()}.

    \sa valueType()
*/
int QtVariantProperty::propertyType() const
{
    return d_ptr->manager->propertyType(this);
}

/*!
    Sets the value of this property to \a value.

    The specified \a value must be of the type returned by
    valueType(), or of a type that can be converted to valueType()
    using the QVariant::canConvert() function; otherwise this function
    does nothing.

    \sa value()
*/
void QtVariantProperty::setValue(const QVariant &value)
{
    d_ptr->manager->setValue(this, value);
}

/*!
    Sets the \a attribute of property to \a value.

    QtVariantPropertyManager provides the related
    \l{QtVariantPropertyManager::setAttribute()}{setAttribute()}
    function.

    \sa attributeValue()
*/
void QtVariantProperty::setAttribute(const QString &attribute, const QVariant &value)
{
    d_ptr->manager->setAttribute(this, attribute, value);
}

class QtVariantPropertyManagerPrivate
{
    QtVariantPropertyManager *q_ptr;
    Q_DECLARE_PUBLIC(QtVariantPropertyManager)
public:
    QtVariantPropertyManagerPrivate();

    bool m_creatingProperty;
    bool m_creatingSubProperties;
    bool m_destroyingSubProperties;
    int m_propertyType;

    void slotValueChanged(QtProperty *property, double val);
    void slotRangeChanged(QtProperty *property, double min, double max);
    void slotSingleStepChanged(QtProperty *property, double step);
    void slotDecimalsChanged(QtProperty *property, int prec);
    void valueChanged(QtProperty *property, const QVariant &val);

    int internalPropertyToType(QtProperty *property) const;
    QtVariantProperty *createSubProperty(QtVariantProperty *parent, QtVariantProperty *after,
            QtProperty *internal);
    void removeSubProperty(QtVariantProperty *property);

    QMap<int, QtAbstractPropertyManager *> m_typeToPropertyManager;
    QMap<int, QMap<QString, int> > m_typeToAttributeToAttributeType;

    QMap<const QtProperty *, QPair<QtVariantProperty *, int> > m_propertyToType;

    QMap<int, int> m_typeToValueType;


    QMap<QtProperty *, QtVariantProperty *> m_internalToProperty;

    const QString m_constraintAttribute;
    const QString m_singleStepAttribute;
    const QString m_decimalsAttribute;
    const QString m_enumIconsAttribute;
    const QString m_enumNamesAttribute;
    const QString m_flagNamesAttribute;
    const QString m_maximumAttribute;
    const QString m_minimumAttribute;
    const QString m_regExpAttribute;
    const QString m_echoModeAttribute;
    const QString m_readOnlyAttribute;
    const QString m_textVisibleAttribute;
};

QtVariantPropertyManagerPrivate::QtVariantPropertyManagerPrivate() :
    m_constraintAttribute(QLatin1String("constraint")),
    m_singleStepAttribute(QLatin1String("singleStep")),
    m_decimalsAttribute(QLatin1String("decimals")),
    m_enumIconsAttribute(QLatin1String("enumIcons")),
    m_enumNamesAttribute(QLatin1String("enumNames")),
    m_flagNamesAttribute(QLatin1String("flagNames")),
    m_maximumAttribute(QLatin1String("maximum")),
    m_minimumAttribute(QLatin1String("minimum")),
    m_regExpAttribute(QLatin1String("regExp")),
    m_echoModeAttribute(QLatin1String("echoMode")),
    m_readOnlyAttribute(QLatin1String("readOnly")),
    m_textVisibleAttribute(QLatin1String("textVisible"))
{
}

int QtVariantPropertyManagerPrivate::internalPropertyToType(QtProperty *property) const
{
    int type = 0;
    QtAbstractPropertyManager *internPropertyManager = property->propertyManager();
    if (qobject_cast<QtDoublePropertyManager *>(internPropertyManager))
        type = QVariant::Double;
    return type;
}

QtVariantProperty *QtVariantPropertyManagerPrivate::createSubProperty(QtVariantProperty *parent,
            QtVariantProperty *after, QtProperty *internal)
{
    int type = internalPropertyToType(internal);
    if (!type)
        return 0;

    bool wasCreatingSubProperties = m_creatingSubProperties;
    m_creatingSubProperties = true;

    QtVariantProperty *varChild = q_ptr->addProperty(type, internal->propertyName());

    m_creatingSubProperties = wasCreatingSubProperties;

    varChild->setPropertyName(internal->propertyName());
    varChild->setToolTip(internal->toolTip());
    varChild->setStatusTip(internal->statusTip());
    varChild->setWhatsThis(internal->whatsThis());

    parent->insertSubProperty(varChild, after);

    m_internalToProperty[internal] = varChild;
    propertyToWrappedProperty()->insert(varChild, internal);
    return varChild;
}

void QtVariantPropertyManagerPrivate::removeSubProperty(QtVariantProperty *property)
{
    QtProperty *internChild = wrappedProperty(property);
    bool wasDestroyingSubProperties = m_destroyingSubProperties;
    m_destroyingSubProperties = true;
    delete property;
    m_destroyingSubProperties = wasDestroyingSubProperties;
    m_internalToProperty.remove(internChild);
    propertyToWrappedProperty()->remove(property);
}

void QtVariantPropertyManagerPrivate::valueChanged(QtProperty *property, const QVariant &val)
{
    QtVariantProperty *varProp = m_internalToProperty.value(property, 0);
    if (!varProp)
        return;
    emit q_ptr->valueChanged(varProp, val);
    emit q_ptr->propertyChanged(varProp);
}

void QtVariantPropertyManagerPrivate::slotValueChanged(QtProperty *property, double val)
{
    valueChanged(property, QVariant(val));
}

void QtVariantPropertyManagerPrivate::slotRangeChanged(QtProperty *property, double min, double max)
{
    if (QtVariantProperty *varProp = m_internalToProperty.value(property, 0)) {
        emit q_ptr->attributeChanged(varProp, m_minimumAttribute, QVariant(min));
        emit q_ptr->attributeChanged(varProp, m_maximumAttribute, QVariant(max));
    }
}

void QtVariantPropertyManagerPrivate::slotSingleStepChanged(QtProperty *property, double step)
{
    if (QtVariantProperty *varProp = m_internalToProperty.value(property, 0))
        emit q_ptr->attributeChanged(varProp, m_singleStepAttribute, QVariant(step));
}

void QtVariantPropertyManagerPrivate::slotDecimalsChanged(QtProperty *property, int prec)
{
    if (QtVariantProperty *varProp = m_internalToProperty.value(property, 0))
        emit q_ptr->attributeChanged(varProp, m_decimalsAttribute, QVariant(prec));
}


/*!
    \class QtVariantPropertyManager

    \brief The QtVariantPropertyManager class provides and manages QVariant based properties.

    QtVariantPropertyManager provides the addProperty() function which
    creates QtVariantProperty objects. The QtVariantProperty class is
    a convenience class handling QVariant based properties inheriting
    QtProperty. A QtProperty object created by a
    QtVariantPropertyManager instance can be converted into a
    QtVariantProperty object using the variantProperty() function.

    The property's value can be retrieved using the value(), and set
    using the setValue() slot. In addition the property's type, and
    the type of its value, can be retrieved using the propertyType()
    and valueType() functions respectively.

    A property's type is a QVariant::Type enumerator value, and
    usually a property's type is the same as its value type. But for
    some properties the types differ, for example for enums, flags and
    group types in which case QtVariantPropertyManager provides the
    enumTypeId(), flagTypeId() and groupTypeId() functions,
    respectively, to identify their property type (the value types are
    QVariant::Int for the enum and flag types, and QVariant::Invalid
    for the group type).

    Use the isPropertyTypeSupported() function to check if a particular
    property type is supported. The currently supported property types
    are:

    \table
    \header
        \o Property Type
        \o Property Type Id
    \row
        \o int
        \o QVariant::Int
    \row
        \o double
        \o QVariant::Double
    \row
        \o bool
        \o QVariant::Bool
    \row
        \o QString
        \o QVariant::String
    \row
        \o QDate
        \o QVariant::Date
    \row
        \o QTime
        \o QVariant::Time
    \row
        \o QDateTime
        \o QVariant::DateTime
    \row
        \o QKeySequence
        \o QVariant::KeySequence
    \row
        \o QChar
        \o QVariant::Char
    \row
        \o QLocale
        \o QVariant::Locale
    \row
        \o QPoint
        \o QVariant::Point
    \row
        \o QPointF
        \o QVariant::PointF
    \row
        \o QSize
        \o QVariant::Size
    \row
        \o QSizeF
        \o QVariant::SizeF
    \row
        \o QRect
        \o QVariant::Rect
    \row
        \o QRectF
        \o QVariant::RectF
    \row
        \o QColor
        \o QVariant::Color
    \row
        \o QSizePolicy
        \o QVariant::SizePolicy
    \row
        \o QFont
        \o QVariant::Font
    \row
        \o QCursor
        \o QVariant::Cursor
    \row
        \o enum
        \o enumTypeId()
    \row
        \o flag
        \o flagTypeId()
    \row
        \o group
        \o groupTypeId()
    \endtable

    Each property type can provide additional attributes,
    e.g. QVariant::Int and QVariant::Double provides minimum and
    maximum values. The currently supported attributes are:

    \table
    \header
        \o Property Type
        \o Attribute Name
        \o Attribute Type
    \row
        \o \c int
        \o minimum
        \o QVariant::Int
    \row
        \o
        \o maximum
        \o QVariant::Int
    \row
        \o
        \o singleStep
        \o QVariant::Int
    \row
        \o \c double
        \o minimum
        \o QVariant::Double
    \row
        \o
        \o maximum
        \o QVariant::Double
    \row
        \o
        \o singleStep
        \o QVariant::Double
    \row
        \o
        \o decimals
        \o QVariant::Int
    \row
        \o \c bool
        \o textVisible
        \o QVariant::Bool
    \row
        \o QString
        \o regExp
        \o QVariant::RegExp
    \row
        \o
        \o echoMode
        \o QVariant::Int
    \row
        \o QDate
        \o minimum
        \o QVariant::Date
    \row
        \o
        \o maximum
        \o QVariant::Date
    \row
        \o QPointF
        \o decimals
        \o QVariant::Int
    \row
        \o QSize
        \o minimum
        \o QVariant::Size
    \row
        \o
        \o maximum
        \o QVariant::Size
    \row
        \o QSizeF
        \o minimum
        \o QVariant::SizeF
    \row
        \o
        \o maximum
        \o QVariant::SizeF
    \row
        \o
        \o decimals
        \o QVariant::Int
    \row
        \o QRect
        \o constraint
        \o QVariant::Rect
    \row
        \o QRectF
        \o constraint
        \o QVariant::RectF
    \row
        \o
        \o decimals
        \o QVariant::Int
    \row
        \o \c enum
        \o enumNames
        \o QVariant::StringList
    \row
        \o
        \o enumIcons
        \o iconMapTypeId()
    \row
        \o \c flag
        \o flagNames
        \o QVariant::StringList
    \endtable

    The attributes for a given property type can be retrieved using
    the attributes() function. Each attribute has a value type which
    can be retrieved using the attributeType() function, and a value
    accessible through the attributeValue() function. In addition, the
    value can be set using the setAttribute() slot.

    QtVariantManager also provides the valueChanged() signal which is
    emitted whenever a property created by this manager change, and
    the attributeChanged() signal which is emitted whenever an
    attribute of such a property changes.

    \sa QtVariantProperty, QtVariantEditorFactory
*/

/*!
    \fn void QtVariantPropertyManager::valueChanged(QtProperty *property, const QVariant &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtVariantPropertyManager::attributeChanged(QtProperty *property,
                const QString &attribute, const QVariant &value)

    This signal is emitted whenever an attribute of a property created
    by this manager changes its value, passing a pointer to the \a
    property, the \a attribute and the new \a value as parameters.

    \sa setAttribute()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtVariantPropertyManager::QtVariantPropertyManager(QObject *parent)
    : QtAbstractPropertyManager(parent)
{
    d_ptr = new QtVariantPropertyManagerPrivate;
    d_ptr->q_ptr = this;

    d_ptr->m_creatingProperty = false;
    d_ptr->m_creatingSubProperties = false;
    d_ptr->m_destroyingSubProperties = false;
    d_ptr->m_propertyType = 0;

    // DoublePropertyManager
    QtDoublePropertyManager *doublePropertyManager = new QtDoublePropertyManager(this);
    d_ptr->m_typeToPropertyManager[QVariant::Double] = doublePropertyManager;
    d_ptr->m_typeToAttributeToAttributeType[QVariant::Double][d_ptr->m_minimumAttribute] =
            QVariant::Double;
    d_ptr->m_typeToAttributeToAttributeType[QVariant::Double][d_ptr->m_maximumAttribute] =
            QVariant::Double;
    d_ptr->m_typeToAttributeToAttributeType[QVariant::Double][d_ptr->m_singleStepAttribute] =
            QVariant::Double;
    d_ptr->m_typeToAttributeToAttributeType[QVariant::Double][d_ptr->m_decimalsAttribute] =
            QVariant::Int;
    d_ptr->m_typeToAttributeToAttributeType[QVariant::Double][d_ptr->m_readOnlyAttribute] =
        QVariant::Bool;
    d_ptr->m_typeToValueType[QVariant::Double] = QVariant::Double;
    connect(doublePropertyManager, SIGNAL(valueChanged(QtProperty *, double)),
                this, SLOT(slotValueChanged(QtProperty *, double)));
    connect(doublePropertyManager, SIGNAL(rangeChanged(QtProperty *, double, double)),
                this, SLOT(slotRangeChanged(QtProperty *, double, double)));
    connect(doublePropertyManager, SIGNAL(singleStepChanged(QtProperty *, double)),
                this, SLOT(slotSingleStepChanged(QtProperty *, double)));
    connect(doublePropertyManager, SIGNAL(decimalsChanged(QtProperty *, int)),
                this, SLOT(slotDecimalsChanged(QtProperty *, int)));
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtVariantPropertyManager::~QtVariantPropertyManager()
{
    clear();
    delete d_ptr;
}

/*!
    Returns the given \a property converted into a QtVariantProperty.

    If the \a property was not created by this variant manager, the
    function returns 0.

    \sa createProperty()
*/
QtVariantProperty *QtVariantPropertyManager::variantProperty(const QtProperty *property) const
{
    const QMap<const QtProperty *, QPair<QtVariantProperty *, int> >::const_iterator it = d_ptr->m_propertyToType.constFind(property);
    if (it == d_ptr->m_propertyToType.constEnd())
        return 0;
    return it.value().first;
}

/*!
    Returns true if the given \a propertyType is supported by this
    variant manager; otherwise false.

    \sa propertyType()
*/
bool QtVariantPropertyManager::isPropertyTypeSupported(int propertyType) const
{
    if (d_ptr->m_typeToValueType.contains(propertyType))
        return true;
    return false;
}

/*!
   Creates and returns a variant property of the given \a propertyType
   with the given \a name.

   If the specified \a propertyType is not supported by this variant
   manager, this function returns 0.

   Do not use the inherited
   QtAbstractPropertyManager::addProperty() function to create a
   variant property (that function will always return 0 since it will
   not be clear what type the property should have).

    \sa isPropertyTypeSupported()
*/
QtVariantProperty *QtVariantPropertyManager::addProperty(int propertyType, const QString &name)
{
    if (!isPropertyTypeSupported(propertyType))
        return 0;

    bool wasCreating = d_ptr->m_creatingProperty;
    d_ptr->m_creatingProperty = true;
    d_ptr->m_propertyType = propertyType;
    QtProperty *property = QtAbstractPropertyManager::addProperty(name);
    d_ptr->m_creatingProperty = wasCreating;
    d_ptr->m_propertyType = 0;

    if (!property)
        return 0;

    return variantProperty(property);
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an invalid variant.

    \sa setValue()
*/
QVariant QtVariantPropertyManager::value(const QtProperty *property) const
{
    QtProperty *internProp = propertyToWrappedProperty()->value(property, 0);
    if (internProp == 0)
        return QVariant();

    QtAbstractPropertyManager *manager = internProp->propertyManager();
    if (QtDoublePropertyManager *doubleManager = qobject_cast<QtDoublePropertyManager *>(manager)) {
        return doubleManager->value(internProp);
    }
    return QVariant();
}

/*!
    Returns the given \a property's value type.

    \sa propertyType()
*/
int QtVariantPropertyManager::valueType(const QtProperty *property) const
{
    int propType = propertyType(property);
    return valueType(propType);
}

/*!
    \overload

    Returns the value type associated with the given \a propertyType.
*/
int QtVariantPropertyManager::valueType(int propertyType) const
{
    if (d_ptr->m_typeToValueType.contains(propertyType))
        return d_ptr->m_typeToValueType[propertyType];
    return 0;
}

/*!
    Returns the given \a property's type.

    \sa valueType()
*/
int QtVariantPropertyManager::propertyType(const QtProperty *property) const
{
    const QMap<const QtProperty *, QPair<QtVariantProperty *, int> >::const_iterator it = d_ptr->m_propertyToType.constFind(property);
    if (it == d_ptr->m_propertyToType.constEnd())
        return 0;
    return it.value().second;
}

/*!
    Returns the given \a property's value for the specified \a
    attribute

    If the given \a property was not created by \e this manager, or if
    the specified \a attribute does not exist, this function returns
    an invalid variant.

    \sa attributes(), attributeType(), setAttribute()
*/
QVariant QtVariantPropertyManager::attributeValue(const QtProperty *property, const QString &attribute) const
{
    int propType = propertyType(property);
    if (!propType)
        return QVariant();

    QMap<int, QMap<QString, int> >::ConstIterator it =
            d_ptr->m_typeToAttributeToAttributeType.find(propType);
    if (it == d_ptr->m_typeToAttributeToAttributeType.constEnd())
        return QVariant();

    QMap<QString, int> attributes = it.value();
    QMap<QString, int>::ConstIterator itAttr = attributes.find(attribute);
    if (itAttr == attributes.constEnd())
        return QVariant();

    QtProperty *internProp = propertyToWrappedProperty()->value(property, 0);
    if (internProp == 0)
        return QVariant();

    QtAbstractPropertyManager *manager = internProp->propertyManager();
    if (QtDoublePropertyManager *doubleManager = qobject_cast<QtDoublePropertyManager *>(manager)) {
        if (attribute == d_ptr->m_maximumAttribute)
            return doubleManager->maximum(internProp);
        if (attribute == d_ptr->m_minimumAttribute)
            return doubleManager->minimum(internProp);
        if (attribute == d_ptr->m_singleStepAttribute)
            return doubleManager->singleStep(internProp);
        if (attribute == d_ptr->m_decimalsAttribute)
            return doubleManager->decimals(internProp);
        if (attribute == d_ptr->m_readOnlyAttribute)
            return doubleManager->isReadOnly(internProp);
        return QVariant();
    }
    return QVariant();
}

/*!
    Returns a list of the given \a propertyType 's attributes.

    \sa attributeValue(), attributeType()
*/
QStringList QtVariantPropertyManager::attributes(int propertyType) const
{
    QMap<int, QMap<QString, int> >::ConstIterator it =
            d_ptr->m_typeToAttributeToAttributeType.find(propertyType);
    if (it == d_ptr->m_typeToAttributeToAttributeType.constEnd())
        return QStringList();
    return it.value().keys();
}

/*!
    Returns the type of the specified \a attribute of the given \a
    propertyType.

    If the given \a propertyType is not supported by \e this manager,
    or if the given \a propertyType does not possess the specified \a
    attribute, this function returns QVariant::Invalid.

    \sa attributes(), valueType()
*/
int QtVariantPropertyManager::attributeType(int propertyType, const QString &attribute) const
{
    QMap<int, QMap<QString, int> >::ConstIterator it =
            d_ptr->m_typeToAttributeToAttributeType.find(propertyType);
    if (it == d_ptr->m_typeToAttributeToAttributeType.constEnd())
        return 0;

    QMap<QString, int> attributes = it.value();
    QMap<QString, int>::ConstIterator itAttr = attributes.find(attribute);
    if (itAttr == attributes.constEnd())
        return 0;
    return itAttr.value();
}

/*!
    \fn void QtVariantPropertyManager::setValue(QtProperty *property, const QVariant &value)

    Sets the value of the given \a property to \a value.

    The specified \a value must be of a type returned by valueType(),
    or of type that can be converted to valueType() using the
    QVariant::canConvert() function, otherwise this function does
    nothing.

    \sa value(), QtVariantProperty::setValue(), valueChanged()
*/
void QtVariantPropertyManager::setValue(QtProperty *property, const QVariant &val)
{
    int propType = val.userType();
    if (!propType)
        return;

    int valType = valueType(property);

    if (propType != valType && !val.canConvert(static_cast<QVariant::Type>(valType)))
        return;

    QtProperty *internProp = propertyToWrappedProperty()->value(property, 0);
    if (internProp == 0)
        return;



    QtAbstractPropertyManager *manager = internProp->propertyManager();
    if (QtDoublePropertyManager *doubleManager = qobject_cast<QtDoublePropertyManager *>(manager)) {
        doubleManager->setValue(internProp, val.value<double>());
        return;
    }
}

/*!
    Sets the value of the specified \a attribute of the given \a
    property, to \a value.

    The new \a value's type must be of the type returned by
    attributeType(), or of a type that can be converted to
    attributeType() using the QVariant::canConvert() function,
    otherwise this function does nothing.

    \sa attributeValue(), QtVariantProperty::setAttribute(), attributeChanged()
*/
void QtVariantPropertyManager::setAttribute(QtProperty *property,
        const QString &attribute, const QVariant &val)
{
    QVariant oldAttr = attributeValue(property, attribute);
    if (!oldAttr.isValid())
        return;

    int attrType = val.userType();
    if (!attrType)
        return;

    if (attrType != attributeType(propertyType(property), attribute) &&
                !val.canConvert((QVariant::Type)attrType))
        return;

    QtProperty *internProp = propertyToWrappedProperty()->value(property, 0);
    if (internProp == 0)
        return;

    QtAbstractPropertyManager *manager = internProp->propertyManager();
    if (QtDoublePropertyManager *doubleManager = qobject_cast<QtDoublePropertyManager *>(manager)) {
        if (attribute == d_ptr->m_maximumAttribute)
            doubleManager->setMaximum(internProp, val.value<double>());
        if (attribute == d_ptr->m_minimumAttribute)
            doubleManager->setMinimum(internProp, val.value<double>());
        if (attribute == d_ptr->m_singleStepAttribute)
            doubleManager->setSingleStep(internProp, val.value<double>());
        if (attribute == d_ptr->m_decimalsAttribute)
            doubleManager->setDecimals(internProp, val.value<int>());
        if (attribute == d_ptr->m_readOnlyAttribute)
            doubleManager->setReadOnly(internProp, val.value<bool>());
        return;
    }
}

/*!
    \reimp
*/
bool QtVariantPropertyManager::hasValue(const QtProperty *property) const
{
    if (propertyType(property) == groupTypeId())
        return false;
    return true;
}

/*!
    \reimp
*/
QString QtVariantPropertyManager::valueText(const QtProperty *property) const
{
    const QtProperty *internProp = propertyToWrappedProperty()->value(property, 0);
    return internProp ? !internProp->displayText().isEmpty() ? internProp->displayText() : internProp->valueText() : QString();
}


/*!
    \reimp
*/
QIcon QtVariantPropertyManager::valueIcon(const QtProperty *property) const
{
    const QtProperty *internProp = propertyToWrappedProperty()->value(property, 0);
    return internProp ? internProp->valueIcon() : QIcon();
}

/*!
    \reimp
*/
void QtVariantPropertyManager::initializeProperty(QtProperty *property)
{
    QtVariantProperty *varProp = variantProperty(property);
    if (!varProp)
        return;

    QMap<int, QtAbstractPropertyManager *>::ConstIterator it =
            d_ptr->m_typeToPropertyManager.find(d_ptr->m_propertyType);
    if (it != d_ptr->m_typeToPropertyManager.constEnd()) {
        QtProperty *internProp = 0;
        if (!d_ptr->m_creatingSubProperties) {
            QtAbstractPropertyManager *manager = it.value();
            internProp = manager->addProperty();
            d_ptr->m_internalToProperty[internProp] = varProp;
        }
        propertyToWrappedProperty()->insert(varProp, internProp);
        if (internProp) {
            QList<QtProperty *> children = internProp->subProperties();
            QListIterator<QtProperty *> itChild(children);
            QtVariantProperty *lastProperty = 0;
            while (itChild.hasNext()) {
                QtVariantProperty *prop = d_ptr->createSubProperty(varProp, lastProperty, itChild.next());
                lastProperty = prop ? prop : lastProperty;
            }
        }
    }
}

/*!
    \reimp
*/
void QtVariantPropertyManager::uninitializeProperty(QtProperty *property)
{
    const QMap<const QtProperty *, QPair<QtVariantProperty *, int> >::iterator type_it = d_ptr->m_propertyToType.find(property);
    if (type_it == d_ptr->m_propertyToType.end())
        return;

    PropertyMap::iterator it = propertyToWrappedProperty()->find(property);
    if (it != propertyToWrappedProperty()->end()) {
        QtProperty *internProp = it.value();
        if (internProp) {
            d_ptr->m_internalToProperty.remove(internProp);
            if (!d_ptr->m_destroyingSubProperties) {
                delete internProp;
            }
        }
        propertyToWrappedProperty()->erase(it);
    }
    d_ptr->m_propertyToType.erase(type_it);
}

/*!
    \reimp
*/
QtProperty *QtVariantPropertyManager::createProperty()
{
    if (!d_ptr->m_creatingProperty)
        return 0;

    QtVariantProperty *property = new QtVariantProperty(this);
    d_ptr->m_propertyToType.insert(property, qMakePair(property, d_ptr->m_propertyType));

    return property;
}

/////////////////////////////

class QtVariantEditorFactoryPrivate
{
    QtVariantEditorFactory *q_ptr;
    Q_DECLARE_PUBLIC(QtVariantEditorFactory)
public:

    QtDoubleSpinBoxFactory     *m_doubleSpinBoxFactory;

    QMap<QtAbstractEditorFactoryBase *, int> m_factoryToType;
    QMap<int, QtAbstractEditorFactoryBase *> m_typeToFactory;
};

/*!
    \class QtVariantEditorFactory

    \brief The QtVariantEditorFactory class provides widgets for properties
    created by QtVariantPropertyManager objects.

    The variant factory provides the following widgets for the
    specified property types:

    \table
    \header
        \o Property Type
        \o Widget
    \row
        \o \c int
        \o QSpinBox
    \row
        \o \c double
        \o QDoubleSpinBox
    \row
        \o \c bool
        \o QCheckBox
    \row
        \o QString
        \o QLineEdit
    \row
        \o QDate
        \o QDateEdit
    \row
        \o QTime
        \o QTimeEdit
    \row
        \o QDateTime
        \o QDateTimeEdit
    \row
        \o QKeySequence
        \o customized editor
    \row
        \o QChar
        \o customized editor
    \row
        \o \c enum
        \o QComboBox
    \row
        \o QCursor
        \o QComboBox
    \endtable

    Note that QtVariantPropertyManager supports several additional property
    types for which the QtVariantEditorFactory class does not provide
    editing widgets, e.g. QPoint and QSize. To provide widgets for other
    types using the variant approach, derive from the QtVariantEditorFactory
    class.

    \sa QtAbstractEditorFactory, QtVariantPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtVariantEditorFactory::QtVariantEditorFactory(QObject *parent)
    : QtAbstractEditorFactory<QtVariantPropertyManager>(parent)
{
    d_ptr = new QtVariantEditorFactoryPrivate();
    d_ptr->q_ptr = this;

    d_ptr->m_doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
    d_ptr->m_factoryToType[d_ptr->m_doubleSpinBoxFactory] = QVariant::Double;
    d_ptr->m_typeToFactory[QVariant::Double] = d_ptr->m_doubleSpinBoxFactory;
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtVariantEditorFactory::~QtVariantEditorFactory()
{
    delete d_ptr;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtVariantEditorFactory::connectPropertyManager(QtVariantPropertyManager *manager)
{
    QList<QtDoublePropertyManager *> doublePropertyManagers = manager->findChildren<QtDoublePropertyManager *>();
    QListIterator<QtDoublePropertyManager *> itDouble(doublePropertyManagers);
    while (itDouble.hasNext())
        d_ptr->m_doubleSpinBoxFactory->addPropertyManager(itDouble.next());
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget *QtVariantEditorFactory::createEditor(QtVariantPropertyManager *manager, QtProperty *property,
        QWidget *parent)
{
    const int propType = manager->propertyType(property);
    QtAbstractEditorFactoryBase *factory = d_ptr->m_typeToFactory.value(propType, 0);
    if (!factory)
        return 0;
    return factory->createEditor(wrappedProperty(property), parent);
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtVariantEditorFactory::disconnectPropertyManager(QtVariantPropertyManager *manager)
{
    QList<QtDoublePropertyManager *> doublePropertyManagers = manager->findChildren<QtDoublePropertyManager *>();
    QListIterator<QtDoublePropertyManager *> itDouble(doublePropertyManagers);
    while (itDouble.hasNext())
        d_ptr->m_doubleSpinBoxFactory->removePropertyManager(itDouble.next());
}

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#include "moc_qtvariantproperty.cpp"

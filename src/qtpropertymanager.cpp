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


#include "qtpropertymanager.h"
#include "qtpropertybrowserutils_p.h"
#include <QtCore/QDateTime>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtCore/QMetaEnum>
#include <QIcon>
#include <QFontDatabase>
#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <QLabel>
#include <QCheckBox>
#include <QApplication>
#include <QLineEdit>

#include <limits.h>
#include <float.h>

#if defined(Q_CC_MSVC)
#    pragma warning(disable: 4786) /* MS VS 6: truncating debug info after 255 characters */
#endif

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

template <class PrivateData, class Value>
static void setSimpleMinimumData(PrivateData *data, const Value &minVal)
{
    data->minVal = minVal;
    if (data->maxVal < data->minVal)
        data->maxVal = data->minVal;

    if (data->val < data->minVal)
        data->val = data->minVal;
}

template <class PrivateData, class Value>
static void setSimpleMaximumData(PrivateData *data, const Value &maxVal)
{
    data->maxVal = maxVal;
    if (data->minVal > data->maxVal)
        data->minVal = data->maxVal;

    if (data->val > data->maxVal)
        data->val = data->maxVal;
}

template <class PrivateData, class Value>
static void setSizeMinimumData(PrivateData *data, const Value &newMinVal)
{
    data->minVal = newMinVal;
    if (data->maxVal.width() < data->minVal.width())
        data->maxVal.setWidth(data->minVal.width());
    if (data->maxVal.height() < data->minVal.height())
        data->maxVal.setHeight(data->minVal.height());

    if (data->val.width() < data->minVal.width())
        data->val.setWidth(data->minVal.width());
    if (data->val.height() < data->minVal.height())
        data->val.setHeight(data->minVal.height());
}

template <class PrivateData, class Value>
static void setSizeMaximumData(PrivateData *data, const Value &newMaxVal)
{
    data->maxVal = newMaxVal;
    if (data->minVal.width() > data->maxVal.width())
        data->minVal.setWidth(data->maxVal.width());
    if (data->minVal.height() > data->maxVal.height())
        data->minVal.setHeight(data->maxVal.height());

    if (data->val.width() > data->maxVal.width())
        data->val.setWidth(data->maxVal.width());
    if (data->val.height() > data->maxVal.height())
        data->val.setHeight(data->maxVal.height());
}

template <class SizeValue>
static SizeValue qBoundSize(const SizeValue &minVal, const SizeValue &val, const SizeValue &maxVal)
{
    SizeValue croppedVal = val;
    if (minVal.width() > val.width())
        croppedVal.setWidth(minVal.width());
    else if (maxVal.width() < val.width())
        croppedVal.setWidth(maxVal.width());

    if (minVal.height() > val.height())
        croppedVal.setHeight(minVal.height());
    else if (maxVal.height() < val.height())
        croppedVal.setHeight(maxVal.height());

    return croppedVal;
}

// Match the exact signature of qBound for VS 6.
QSize qBound(QSize minVal, QSize val, QSize maxVal)
{
    return qBoundSize(minVal, val, maxVal);
}

QSizeF qBound(QSizeF minVal, QSizeF val, QSizeF maxVal)
{
    return qBoundSize(minVal, val, maxVal);
}

namespace {

namespace {
template <class Value>
void orderBorders(Value &minVal, Value &maxVal)
{
    if (minVal > maxVal)
        qSwap(minVal, maxVal);
}

template <class Value>
static void orderSizeBorders(Value &minVal, Value &maxVal)
{
    Value fromSize = minVal;
    Value toSize = maxVal;
    if (fromSize.width() > toSize.width()) {
        fromSize.setWidth(maxVal.width());
        toSize.setWidth(minVal.width());
    }
    if (fromSize.height() > toSize.height()) {
        fromSize.setHeight(maxVal.height());
        toSize.setHeight(minVal.height());
    }
    minVal = fromSize;
    maxVal = toSize;
}

void orderBorders(QSize &minVal, QSize &maxVal)
{
    orderSizeBorders(minVal, maxVal);
}

void orderBorders(QSizeF &minVal, QSizeF &maxVal)
{
    orderSizeBorders(minVal, maxVal);
}

}
}
////////

template <class Value, class PrivateData>
static Value getData(const QMap<const QtProperty *, PrivateData> &propertyMap,
            Value PrivateData::*data,
            const QtProperty *property, const Value &defaultValue = Value())
{
    typedef QMap<const QtProperty *, PrivateData> PropertyToData;
    typedef typename PropertyToData::const_iterator PropertyToDataConstIterator;
    const PropertyToDataConstIterator it = propertyMap.constFind(property);
    if (it == propertyMap.constEnd())
        return defaultValue;
    return it.value().*data;
}

template <class Value, class PrivateData>
static Value getValue(const QMap<const QtProperty *, PrivateData> &propertyMap,
            const QtProperty *property, const Value &defaultValue = Value())
{
    return getData<Value>(propertyMap, &PrivateData::val, property, defaultValue);
}

template <class Value, class PrivateData>
static Value getMinimum(const QMap<const QtProperty *, PrivateData> &propertyMap,
            const QtProperty *property, const Value &defaultValue = Value())
{
    return getData<Value>(propertyMap, &PrivateData::minVal, property, defaultValue);
}

template <class Value, class PrivateData>
static Value getMaximum(const QMap<const QtProperty *, PrivateData> &propertyMap,
            const QtProperty *property, const Value &defaultValue = Value())
{
    return getData<Value>(propertyMap, &PrivateData::maxVal, property, defaultValue);
}

template <class ValueChangeParameter, class Value, class PropertyManager>
static void setSimpleValue(QMap<const QtProperty *, Value> &propertyMap,
            PropertyManager *manager,
            void (PropertyManager::*propertyChangedSignal)(QtProperty *),
            void (PropertyManager::*valueChangedSignal)(QtProperty *, ValueChangeParameter),
            QtProperty *property, const Value &val)
{
    typedef QMap<const QtProperty *, Value> PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = propertyMap.find(property);
    if (it == propertyMap.end())
        return;

    if (it.value() == val)
        return;

    it.value() = val;

    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value>
static void setValueInRange(PropertyManager *manager, PropertyManagerPrivate *managerPrivate,
            void (PropertyManager::*propertyChangedSignal)(QtProperty *),
            void (PropertyManager::*valueChangedSignal)(QtProperty *, ValueChangeParameter),
            QtProperty *property, const Value &val,
            void (PropertyManagerPrivate::*setSubPropertyValue)(QtProperty *, ValueChangeParameter))
{
    typedef typename PropertyManagerPrivate::Data PrivateData;
    typedef QMap<const QtProperty *, PrivateData> PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = managerPrivate->m_values.find(property);
    if (it == managerPrivate->m_values.end())
        return;

    PrivateData &data = it.value();

    if (data.val == val)
        return;

    const Value oldVal = data.val;

    data.val = qBound(data.minVal, val, data.maxVal);

    if (data.val == oldVal)
        return;

    if (setSubPropertyValue)
        (managerPrivate->*setSubPropertyValue)(property, data.val);

    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, data.val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value>
static void setBorderValues(PropertyManager *manager, PropertyManagerPrivate *managerPrivate,
            void (PropertyManager::*propertyChangedSignal)(QtProperty *),
            void (PropertyManager::*valueChangedSignal)(QtProperty *, ValueChangeParameter),
            void (PropertyManager::*rangeChangedSignal)(QtProperty *, ValueChangeParameter, ValueChangeParameter),
            QtProperty *property, const Value &minVal, const Value &maxVal,
            void (PropertyManagerPrivate::*setSubPropertyRange)(QtProperty *,
                    ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
{
    typedef typename PropertyManagerPrivate::Data PrivateData;
    typedef QMap<const QtProperty *, PrivateData> PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = managerPrivate->m_values.find(property);
    if (it == managerPrivate->m_values.end())
        return;

    Value fromVal = minVal;
    Value toVal = maxVal;
    orderBorders(fromVal, toVal);

    PrivateData &data = it.value();

    if (data.minVal == fromVal && data.maxVal == toVal)
        return;

    const Value oldVal = data.val;

    data.setMinimumValue(fromVal);
    data.setMaximumValue(toVal);

    emit (manager->*rangeChangedSignal)(property, data.minVal, data.maxVal);

    if (setSubPropertyRange)
        (managerPrivate->*setSubPropertyRange)(property, data.minVal, data.maxVal, data.val);

    if (data.val == oldVal)
        return;

    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, data.val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData>
static void setBorderValue(PropertyManager *manager, PropertyManagerPrivate *managerPrivate,
            void (PropertyManager::*propertyChangedSignal)(QtProperty *),
            void (PropertyManager::*valueChangedSignal)(QtProperty *, ValueChangeParameter),
            void (PropertyManager::*rangeChangedSignal)(QtProperty *, ValueChangeParameter, ValueChangeParameter),
            QtProperty *property,
            Value (PrivateData::*getRangeVal)() const,
            void (PrivateData::*setRangeVal)(ValueChangeParameter), const Value &borderVal,
            void (PropertyManagerPrivate::*setSubPropertyRange)(QtProperty *,
                    ValueChangeParameter, ValueChangeParameter, ValueChangeParameter))
{
    typedef QMap<const QtProperty *, PrivateData> PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = managerPrivate->m_values.find(property);
    if (it == managerPrivate->m_values.end())
        return;

    PrivateData &data = it.value();

    if ((data.*getRangeVal)() == borderVal)
        return;

    const Value oldVal = data.val;

    (data.*setRangeVal)(borderVal);

    emit (manager->*rangeChangedSignal)(property, data.minVal, data.maxVal);

    if (setSubPropertyRange)
        (managerPrivate->*setSubPropertyRange)(property, data.minVal, data.maxVal, data.val);

    if (data.val == oldVal)
        return;

    emit (manager->*propertyChangedSignal)(property);
    emit (manager->*valueChangedSignal)(property, data.val);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData>
static void setMinimumValue(PropertyManager *manager, PropertyManagerPrivate *managerPrivate,
            void (PropertyManager::*propertyChangedSignal)(QtProperty *),
            void (PropertyManager::*valueChangedSignal)(QtProperty *, ValueChangeParameter),
            void (PropertyManager::*rangeChangedSignal)(QtProperty *, ValueChangeParameter, ValueChangeParameter),
            QtProperty *property, const Value &minVal)
{
    void (PropertyManagerPrivate::*setSubPropertyRange)(QtProperty *,
                    ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
    setBorderValue<ValueChangeParameter, PropertyManagerPrivate, PropertyManager, Value, PrivateData>(manager, managerPrivate,
            propertyChangedSignal, valueChangedSignal, rangeChangedSignal,
            property, &PropertyManagerPrivate::Data::minimumValue, &PropertyManagerPrivate::Data::setMinimumValue, minVal, setSubPropertyRange);
}

template <class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData>
static void setMaximumValue(PropertyManager *manager, PropertyManagerPrivate *managerPrivate,
            void (PropertyManager::*propertyChangedSignal)(QtProperty *),
            void (PropertyManager::*valueChangedSignal)(QtProperty *, ValueChangeParameter),
            void (PropertyManager::*rangeChangedSignal)(QtProperty *, ValueChangeParameter, ValueChangeParameter),
            QtProperty *property, const Value &maxVal)
{
    void (PropertyManagerPrivate::*setSubPropertyRange)(QtProperty *,
                    ValueChangeParameter, ValueChangeParameter, ValueChangeParameter) = 0;
    setBorderValue<ValueChangeParameter, PropertyManagerPrivate, PropertyManager, Value, PrivateData>(manager, managerPrivate,
            propertyChangedSignal, valueChangedSignal, rangeChangedSignal,
            property, &PropertyManagerPrivate::Data::maximumValue, &PropertyManagerPrivate::Data::setMaximumValue, maxVal, setSubPropertyRange);
}

class QtMetaEnumWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSizePolicy::Policy policy READ policy)
public:
    QSizePolicy::Policy policy() const { return QSizePolicy::Ignored; }
private:
    QtMetaEnumWrapper(QObject *parent) : QObject(parent) {}
};

class QtMetaEnumProvider
{
public:
    QtMetaEnumProvider();

    QStringList policyEnumNames() const { return m_policyEnumNames; }
    QStringList languageEnumNames() const { return m_languageEnumNames; }
    QStringList countryEnumNames(QLocale::Language language) const { return m_countryEnumNames.value(language); }

    QSizePolicy::Policy indexToSizePolicy(int index) const;
    int sizePolicyToIndex(QSizePolicy::Policy policy) const;

    void indexToLocale(int languageIndex, int countryIndex, QLocale::Language *language, QLocale::Country *country) const;
    void localeToIndex(QLocale::Language language, QLocale::Country country, int *languageIndex, int *countryIndex) const;

private:
    void initLocale();

    QStringList m_policyEnumNames;
    QStringList m_languageEnumNames;
    QMap<QLocale::Language, QStringList> m_countryEnumNames;
    QMap<int, QLocale::Language> m_indexToLanguage;
    QMap<QLocale::Language, int> m_languageToIndex;
    QMap<int, QMap<int, QLocale::Country> > m_indexToCountry;
    QMap<QLocale::Language, QMap<QLocale::Country, int> > m_countryToIndex;
    QMetaEnum m_policyEnum;
};

#if QT_VERSION < 0x040300

static QList<QLocale::Country> countriesForLanguage(QLocale::Language language)
{
    QList<QLocale::Country> countries;
    QLocale::Country country = QLocale::AnyCountry;
    while (country <= QLocale::LastCountry) {
        QLocale locale(language, country);
        if (locale.language() == language && !countries.contains(locale.country()))
            countries << locale.country();
        country = (QLocale::Country)((uint)country + 1); // ++country
    }
    return countries;
}

#endif

static QList<QLocale::Country> sortCountries(const QList<QLocale::Country> &countries)
{
    QMultiMap<QString, QLocale::Country> nameToCountry;
    QListIterator<QLocale::Country> itCountry(countries);
    while (itCountry.hasNext()) {
        QLocale::Country country = itCountry.next();
        nameToCountry.insert(QLocale::countryToString(country), country);
    }
    return nameToCountry.values();
}

void QtMetaEnumProvider::initLocale()
{
    QMultiMap<QString, QLocale::Language> nameToLanguage;
    QLocale::Language language = QLocale::C;
    while (language <= QLocale::LastLanguage) {
        QLocale locale(language);
        if (locale.language() == language)
            nameToLanguage.insert(QLocale::languageToString(language), language);
        language = (QLocale::Language)((uint)language + 1); // ++language
    }

    const QLocale system = QLocale::system();
    if (!nameToLanguage.contains(QLocale::languageToString(system.language())))
        nameToLanguage.insert(QLocale::languageToString(system.language()), system.language());

    QList<QLocale::Language> languages = nameToLanguage.values();
    QListIterator<QLocale::Language> itLang(languages);
    while (itLang.hasNext()) {
        QLocale::Language language = itLang.next();
        QList<QLocale::Country> countries;
#if QT_VERSION < 0x040300
        countries = countriesForLanguage(language);
#else
        countries = QLocale::countriesForLanguage(language);
#endif
        if (countries.isEmpty() && language == system.language())
            countries << system.country();

        if (!countries.isEmpty() && !m_languageToIndex.contains(language)) {
            countries = sortCountries(countries);
            int langIdx = m_languageEnumNames.count();
            m_indexToLanguage[langIdx] = language;
            m_languageToIndex[language] = langIdx;
            QStringList countryNames;
            QListIterator<QLocale::Country> it(countries);
            int countryIdx = 0;
            while (it.hasNext()) {
                QLocale::Country country = it.next();
                countryNames << QLocale::countryToString(country);
                m_indexToCountry[langIdx][countryIdx] = country;
                m_countryToIndex[language][country] = countryIdx;
                ++countryIdx;
            }
            m_languageEnumNames << QLocale::languageToString(language);
            m_countryEnumNames[language] = countryNames;
        }
    }
}

QtMetaEnumProvider::QtMetaEnumProvider()
{
    QMetaProperty p;

    p = QtMetaEnumWrapper::staticMetaObject.property(
                QtMetaEnumWrapper::staticMetaObject.propertyOffset() + 0);
    m_policyEnum = p.enumerator();
    const int keyCount = m_policyEnum.keyCount();
    for (int i = 0; i < keyCount; i++)
        m_policyEnumNames << QLatin1String(m_policyEnum.key(i));

    initLocale();
}

QSizePolicy::Policy QtMetaEnumProvider::indexToSizePolicy(int index) const
{
    return static_cast<QSizePolicy::Policy>(m_policyEnum.value(index));
}

int QtMetaEnumProvider::sizePolicyToIndex(QSizePolicy::Policy policy) const
{
     const int keyCount = m_policyEnum.keyCount();
    for (int i = 0; i < keyCount; i++)
        if (indexToSizePolicy(i) == policy)
            return i;
    return -1;
}

void QtMetaEnumProvider::indexToLocale(int languageIndex, int countryIndex, QLocale::Language *language, QLocale::Country *country) const
{
    QLocale::Language l = QLocale::C;
    QLocale::Country c = QLocale::AnyCountry;
    if (m_indexToLanguage.contains(languageIndex)) {
        l = m_indexToLanguage[languageIndex];
        if (m_indexToCountry.contains(languageIndex) && m_indexToCountry[languageIndex].contains(countryIndex))
            c = m_indexToCountry[languageIndex][countryIndex];
    }
    if (language)
        *language = l;
    if (country)
        *country = c;
}

void QtMetaEnumProvider::localeToIndex(QLocale::Language language, QLocale::Country country, int *languageIndex, int *countryIndex) const
{
    int l = -1;
    int c = -1;
    if (m_languageToIndex.contains(language)) {
        l = m_languageToIndex[language];
        if (m_countryToIndex.contains(language) && m_countryToIndex[language].contains(country))
            c = m_countryToIndex[language][country];
    }

    if (languageIndex)
        *languageIndex = l;
    if (countryIndex)
        *countryIndex = c;
}

Q_GLOBAL_STATIC(QtMetaEnumProvider, metaEnumProvider)

// QtGroupPropertyManager

/*!
    \class QtGroupPropertyManager

    \brief The QtGroupPropertyManager provides and manages group properties.

    This class is intended to provide a grouping element without any value.

    \sa QtAbstractPropertyManager
*/

/*!
    Creates a manager with the given \a parent.
*/
QtGroupPropertyManager::QtGroupPropertyManager(QObject *parent)
    : QtAbstractPropertyManager(parent)
{

}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtGroupPropertyManager::~QtGroupPropertyManager()
{

}

/*!
    \reimp
*/
bool QtGroupPropertyManager::hasValue(const QtProperty *property) const
{
    Q_UNUSED(property)
    return false;
}

/*!
    \reimp
*/
void QtGroupPropertyManager::initializeProperty(QtProperty *property)
{
    Q_UNUSED(property)
}

/*!
    \reimp
*/
void QtGroupPropertyManager::uninitializeProperty(QtProperty *property)
{
    Q_UNUSED(property)
}

// QtDoublePropertyManager

class QtDoublePropertyManagerPrivate
{
    QtDoublePropertyManager *q_ptr;
    Q_DECLARE_PUBLIC(QtDoublePropertyManager)
public:

    struct Data
    {
        Data() : val(0), minVal(-INT_MAX), maxVal(INT_MAX), singleStep(1), decimals(2), readOnly(false) {}
        double val;
        double minVal;
        double maxVal;
        double singleStep;
        int decimals;
        bool readOnly;
        double minimumValue() const { return minVal; }
        double maximumValue() const { return maxVal; }
        void setMinimumValue(double newMinVal) { setSimpleMinimumData(this, newMinVal); }
        void setMaximumValue(double newMaxVal) { setSimpleMaximumData(this, newMaxVal); }
    };

    typedef QMap<const QtProperty *, Data> PropertyValueMap;
    PropertyValueMap m_values;
};

/*!
    \class QtDoublePropertyManager

    \brief The QtDoublePropertyManager provides and manages double properties.

    A double property has a current value, and a range specifying the
    valid values. The range is defined by a minimum and a maximum
    value.

    The property's value and range can be retrieved using the value(),
    minimum() and maximum() functions, and can be set using the
    setValue(), setMinimum() and setMaximum() slots.
    Alternatively, the range can be defined in one go using the
    setRange() slot.

    In addition, QtDoublePropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the rangeChanged() signal which is emitted whenever
    such a property changes its range of valid values.

    \sa QtAbstractPropertyManager, QtDoubleSpinBoxFactory
*/

/*!
    \fn void QtDoublePropertyManager::valueChanged(QtProperty *property, double value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtDoublePropertyManager::rangeChanged(QtProperty *property, double minimum, double maximum)

    This signal is emitted whenever a property created by this manager
    changes its range of valid values, passing a pointer to the
    \a property and the new \a minimum and \a maximum values

    \sa setRange()
*/

/*!
    \fn void QtDoublePropertyManager::decimalsChanged(QtProperty *property, int prec)

    This signal is emitted whenever a property created by this manager
    changes its precision of value, passing a pointer to the
    \a property and the new \a prec value

    \sa setDecimals()
*/

/*!
    \fn void QtDoublePropertyManager::singleStepChanged(QtProperty *property, double step)

    This signal is emitted whenever a property created by this manager
    changes its single step property, passing a pointer to the
    \a property and the new \a step value

    \sa setSingleStep()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtDoublePropertyManager::QtDoublePropertyManager(QObject *parent)
    : QtAbstractPropertyManager(parent)
{
    d_ptr = new QtDoublePropertyManagerPrivate;
    d_ptr->q_ptr = this;
}

/*!
    Destroys  this manager, and all the properties it has created.
*/
QtDoublePropertyManager::~QtDoublePropertyManager()
{
    clear();
    delete d_ptr;
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns 0.

    \sa setValue()
*/
double QtDoublePropertyManager::value(const QtProperty *property) const
{
    return getValue<double>(d_ptr->m_values, property, 0.0);
}

/*!
    Returns the given \a property's minimum value.

    \sa maximum(), setRange()
*/
double QtDoublePropertyManager::minimum(const QtProperty *property) const
{
    return getMinimum<double>(d_ptr->m_values, property, 0.0);
}

/*!
    Returns the given \a property's maximum value.

    \sa minimum(), setRange()
*/
double QtDoublePropertyManager::maximum(const QtProperty *property) const
{
    return getMaximum<double>(d_ptr->m_values, property, 0.0);
}

/*!
    Returns the given \a property's step value.

    The step is typically used to increment or decrement a property value while pressing an arrow key.

    \sa setSingleStep()
*/
double QtDoublePropertyManager::singleStep(const QtProperty *property) const
{
    return getData<double>(d_ptr->m_values, &QtDoublePropertyManagerPrivate::Data::singleStep, property, 0);
}

/*!
    Returns the given \a property's precision, in decimals.

    \sa setDecimals()
*/
int QtDoublePropertyManager::decimals(const QtProperty *property) const
{
    return getData<int>(d_ptr->m_values, &QtDoublePropertyManagerPrivate::Data::decimals, property, 0);
}

/*!
    Returns read-only status of the property.

    When property is read-only it's value can be selected and copied from editor but not modified.

    \sa QtDoublePropertyManager::setReadOnly
*/
bool QtDoublePropertyManager::isReadOnly(const QtProperty *property) const
{
    return getData<bool>(d_ptr->m_values, &QtDoublePropertyManagerPrivate::Data::readOnly, property, false);
}

/*!
    \reimp
*/
QString QtDoublePropertyManager::valueText(const QtProperty *property) const
{
    const QtDoublePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->m_values.constFind(property);
    if (it == d_ptr->m_values.constEnd())
        return QString();
    return QLocale::system().toString(it.value().val, 'f', it.value().decimals);
}

/*!
    \fn void QtDoublePropertyManager::setValue(QtProperty *property, double value)

    Sets the value of the given \a property to \a value.

    If the specified \a value is not valid according to the given
    \a property's range, the \a value is adjusted to the nearest valid value
    within the range.

    \sa value(), setRange(), valueChanged()
*/
void QtDoublePropertyManager::setValue(QtProperty *property, double val)
{
    void (QtDoublePropertyManagerPrivate::*setSubPropertyValue)(QtProperty *, double) = 0;
    setValueInRange<double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double>(this, d_ptr,
                &QtDoublePropertyManager::propertyChanged,
                &QtDoublePropertyManager::valueChanged,
                property, val, setSubPropertyValue);
}

/*!
    Sets the step value for the given \a property to \a step.

    The step is typically used to increment or decrement a property value while pressing an arrow key.

    \sa singleStep()
*/
void QtDoublePropertyManager::setSingleStep(QtProperty *property, double step)
{
    const QtDoublePropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    QtDoublePropertyManagerPrivate::Data data = it.value();

    if (step < 0)
        step = 0;

    if (data.singleStep == step)
        return;

    data.singleStep = step;

    it.value() = data;

    emit singleStepChanged(property, data.singleStep);
}

/*!
    Sets read-only status of the property.

    \sa QtDoublePropertyManager::setReadOnly
*/
void QtDoublePropertyManager::setReadOnly(QtProperty *property, bool readOnly)
{
    const QtDoublePropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    QtDoublePropertyManagerPrivate::Data data = it.value();

    if (data.readOnly == readOnly)
        return;

    data.readOnly = readOnly;
    it.value() = data;

    emit propertyChanged(property);
    emit readOnlyChanged(property, data.readOnly);
}

/*!
    \fn void QtDoublePropertyManager::setDecimals(QtProperty *property, int prec)

    Sets the precision of the given \a property to \a prec.

    The valid decimal range is 0-13. The default is 2.

    \sa decimals()
*/
void QtDoublePropertyManager::setDecimals(QtProperty *property, int prec)
{
    const QtDoublePropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    QtDoublePropertyManagerPrivate::Data data = it.value();

    if (prec > 13)
        prec = 13;
    else if (prec < 0)
        prec = 0;

    if (data.decimals == prec)
        return;

    data.decimals = prec;

    it.value() = data;

    emit decimalsChanged(property, data.decimals);
}

/*!
    Sets the minimum value for the given \a property to \a minVal.

    When setting the minimum value, the maximum and current values are
    adjusted if necessary (ensuring that the range remains valid and
    that the current value is within in the range).

    \sa minimum(), setRange(), rangeChanged()
*/
void QtDoublePropertyManager::setMinimum(QtProperty *property, double minVal)
{
    setMinimumValue<double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double, QtDoublePropertyManagerPrivate::Data>(this, d_ptr,
                &QtDoublePropertyManager::propertyChanged,
                &QtDoublePropertyManager::valueChanged,
                &QtDoublePropertyManager::rangeChanged,
                property, minVal);
}

/*!
    Sets the maximum value for the given \a property to \a maxVal.

    When setting the maximum value, the minimum and current values are
    adjusted if necessary (ensuring that the range remains valid and
    that the current value is within in the range).

    \sa maximum(), setRange(), rangeChanged()
*/
void QtDoublePropertyManager::setMaximum(QtProperty *property, double maxVal)
{
    setMaximumValue<double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double, QtDoublePropertyManagerPrivate::Data>(this, d_ptr,
                &QtDoublePropertyManager::propertyChanged,
                &QtDoublePropertyManager::valueChanged,
                &QtDoublePropertyManager::rangeChanged,
                property, maxVal);
}

/*!
    \fn void QtDoublePropertyManager::setRange(QtProperty *property, double minimum, double maximum)

    Sets the range of valid values.

    This is a convenience function defining the range of valid values
    in one go; setting the \a minimum and \a maximum values for the
    given \a property with a single function call.

    When setting a new range, the current value is adjusted if
    necessary (ensuring that the value remains within range).

    \sa setMinimum(), setMaximum(), rangeChanged()
*/
void QtDoublePropertyManager::setRange(QtProperty *property, double minVal, double maxVal)
{
    void (QtDoublePropertyManagerPrivate::*setSubPropertyRange)(QtProperty *, double, double, double) = 0;
    setBorderValues<double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double>(this, d_ptr,
                &QtDoublePropertyManager::propertyChanged,
                &QtDoublePropertyManager::valueChanged,
                &QtDoublePropertyManager::rangeChanged,
                property, minVal, maxVal, setSubPropertyRange);
}

/*!
    \reimp
*/
void QtDoublePropertyManager::initializeProperty(QtProperty *property)
{
    d_ptr->m_values[property] = QtDoublePropertyManagerPrivate::Data();
}

/*!
    \reimp
*/
void QtDoublePropertyManager::uninitializeProperty(QtProperty *property)
{
    d_ptr->m_values.remove(property);
}


#include "moc_qtpropertymanager.cpp"
#include "qtpropertymanager.moc"

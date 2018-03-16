include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtpropertybrowser-uselib:!qtpropertybrowser-buildlib {
    LIBS += -L$$QTPROPERTYBROWSER_LIBDIR -l$$QTPROPERTYBROWSER_LIBNAME
} else {
    SOURCES += $$PWD/QtPropertyBrowser.cpp \
            $$PWD/QtPropertyManager.cpp \
            $$PWD/QtEditorFactory.cpp \
            $$PWD/QtVariantProperty.cpp \
            $$PWD/QtTreePropertyBrowser.cpp \
            $$PWD/QtButtonPropertyBrowser.cpp \
            $$PWD/QtGroupBoxPropertyBrowser.cpp \
            $$PWD/QtPropertyBrowserUtils.cpp
    HEADERS += $$PWD/QtPropertyBrowser.h \
            $$PWD/QtPropertyManager.h \
            $$PWD/QtEditorFactory.h \
            $$PWD/QtVariantProperty.h \
            $$PWD/QtTreePropertyBrowser.h \
            $$PWD/QtButtonPropertyBrowser.h \
            $$PWD/QtGroupBoxPropertyBrowser.h \
            $$PWD/QtPropertyBrowserUtils_p.h
    RESOURCES += $$PWD/QtPropertyBrowser.qrc
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTPROPERTYBROWSER_EXPORT
    else:qtpropertybrowser-uselib:DEFINES += QT_QTPROPERTYBROWSER_IMPORT
}

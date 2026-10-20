TEMPLATE = app
TARGET = OrarioPlanner
INCLUDEPATH += ./model
QT += widgets printsupport
CONFIG += c++17
CONFIG -= app_bundle
VERSION = 1.0.1
QMAKE_TARGET_PRODUCT = "OrarioPlanner"
QMAKE_TARGET_DESCRIPTION = "Pianificazione e Gestione dei Turni Aziendali"
QMAKE_TARGET_COMPANY = "Samuele Gobbi"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2026"

HEADERS += \
    model/JSON/JsonFactory.h \
    model/JSON/JsonReader.h \
    model/JSON/JsonVisitor.h \
    model/JSON/JsonWriter.h \
    model/employee/employee.h \
    model/observer/observable.h \
    model/observer/observer.h \
    model/shift/shift.h \
    model/shiftmanager/failsafeRules/RulesCard.h \
    model/shiftmanager/shiftmanager.h \
    model/shopconfig/shopconfig.h \
    model/utility/utility.h \
    view/leftpanel/employeedetailwidget.h \
    view/leftpanel/leftpanel.h \
    view/mainwindow/mainwindow.h \
    view/pdfexporter/pdfexporter.h \
    view/rightpanel/employeepanel/editemployeewidget.h \
    view/rightpanel/employeepanel/employeeeditorwidget.h \
    view/rightpanel/employeepanel/employeepanel.h \
    view/rightpanel/employeepanel/employeerowwidget.h \
    view/rightpanel/employeepanel/newemployeewidget.h \
    view/rightpanel/helppanel.h \
    view/rightpanel/noemployeepanel.h \
    view/rightpanel/rightpanel.h \
    view/rightpanel/settingspanel.h \
    view/rightpanel/shiftpanel/shiftdelegate.h \
    view/rightpanel/shiftpanel/shiftpanel.h

SOURCES += \
    main.cpp \
    model/JSON/JsonFactory.cpp \
    model/JSON/JsonReader.cpp \
    model/JSON/JsonVisitor.cpp \
    model/JSON/JsonWriter.cpp \
    model/employee/employee.cpp \
    model/observer/observable.cpp \
    model/shift/shift.cpp \
    model/shiftmanager/failsafeRules/RulesCard.cpp \
    model/shiftmanager/shiftmanager.cpp \
    model/shopconfig/shopconfig.cpp \
    model/utility/utility.cpp \
    view/leftpanel/employeedetailwidget.cpp \
    view/leftpanel/leftpanel.cpp \
    view/mainwindow/mainwindow.cpp \
    view/pdfexporter/pdfexporter.cpp \
    view/rightpanel/employeepanel/editemployeewidget.cpp \
    view/rightpanel/employeepanel/employeeeditorwidget.cpp \
    view/rightpanel/employeepanel/employeepanel.cpp \
    view/rightpanel/employeepanel/employeerowwidget.cpp \
    view/rightpanel/employeepanel/newemployeewidget.cpp \
    view/rightpanel/helppanel.cpp \
    view/rightpanel/noemployeepanel.cpp \
    view/rightpanel/rightpanel.cpp \
    view/rightpanel/settingspanel.cpp \
    view/rightpanel/shiftpanel/shiftdelegate.cpp \
    view/rightpanel/shiftpanel/shiftpanel.cpp

RESOURCES += \
    resources.qrc

win32:RC_ICONS = resources/icon/calendar.ico



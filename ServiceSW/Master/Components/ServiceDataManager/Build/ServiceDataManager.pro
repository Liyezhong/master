!include("ServiceDataManager.pri") {
    error("ServiceDataManager.pri not found")
}

QT += gui

TARGET = ServiceDataManager

INCLUDEPATH += ../../../../../../Platform/Common/Components \
               ../../../../../Shared/Master/Components \
               ../../../../../../Himalaya/HimalayaMain/Master/Include \
               ../../../../../../Himalaya/HimalayaMain/Master/Components

HEADERS +=  ../Include/*.h \
    ../Include/TestCaseParameters.h \
    ../Include/TestCaseGuide.h \
    ../Include/TestCaseFactory.h \
    ../Include/TestCase.h

SOURCES +=  ../Source/*.cpp \
    ../Source/TestCaseGuide.cpp \
    ../Source/CTestCaseFactory.cpp \
    ../Source/TestCase.cpp


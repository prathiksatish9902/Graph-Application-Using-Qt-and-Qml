QT += quick charts

SOURCES += \
    DataPoint.cpp \
    DataProvider.cpp \
    GraphItem.cpp \
    main.cpp

RESOURCES += qml.qrc

# Deployment rules
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DataPoint.h \
    DataProvider.h \
    GraphItem.h

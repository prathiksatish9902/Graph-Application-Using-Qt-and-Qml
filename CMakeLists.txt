cmake_minimum_required(VERSION 3.16)

project(GraphApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt code generators
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages
find_package(Qt6 6.2 REQUIRED COMPONENTS Quick Charts)

# Sources and headers
set(SOURCES
    DataPoint.cpp
    DataProvider.cpp
    GraphItem.cpp
    main.cpp
)

set(HEADERS
    DataPoint.h
    DataProvider.h
    GraphItem.h
)

set(RESOURCES
    qml.qrc
)

# Add executable
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick
            Qt6::Charts
)

# Deployment rules (optional)
if(UNIX AND NOT ANDROID)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION /opt/${PROJECT_NAME}/bin
    )
endif()

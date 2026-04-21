cmake_minimum_required(VERSION 3.16)

project(Dashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/Dependencies)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Graphs QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDashboard
    Source/main.cpp
    Source/Backend.cpp

    Source/Utilities/Utilities.cpp
    Source/Utilities/RangesHelper.cpp
    Source/Server/Server.cpp
    Source/Server/DataProcessing/Parser.cpp
    Source/Server/DataProcessing/GraphDataProcessor.cpp

)

qt_add_qml_module(appDashboard
    URI Dashboard

    QML_FILES
        UI/Main.qml
        UI/DashboardCard.qml
        UI/ButtonsRow.qml
        UI/GraphCard.qml
        UI/StyledButton.qml

        UI/DashboardElements/RangeCard.qml
        UI/DashboardElements/CurrentDataCard.qml
        UI/DashboardElements/ConcentrationRangesCard.qml

        UI/GraphElements/ReadingSelectRow.qml
        UI/GraphElements/ReadingsGraph.qml
        UI/GraphElements/CalendarPopup.qml
        QML_FILES UI/GraphElements/StyledRadioButton.qml
        QML_FILES UI/GraphElements/PeriodSelectRow.qml
)

target_include_directories(appDashboard PRIVATE
    Source
    ../Protocol
    ${DEPENDENCIES_DIR}/boost_asio
    ${DEPENDENCIES_DIR}/fmt/include
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDashboard PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDashboard
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(${DEPENDENCIES_DIR}/fmt ${CMAKE_CURRENT_BINARY_DIR}/fmt)

target_link_libraries(appDashboard PRIVATE fmt::fmt Qt6::Quick Qt6::Core Qt6::Graphs Qt6::QuickControls2)

if(WIN32)
    target_link_libraries(appDashboard PRIVATE ws2_32 mswsock)
endif()

include(GNUInstallDirs)
install(TARGETS appDashboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

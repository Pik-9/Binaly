cmake_minimum_required(VERSION 2.6)
project(filalyzer)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Filalyzer_SRCS Filalyzer.cpp main.cpp)

qt4_automoc(${Filalyzer_SRCS})
add_executable(filalyzer ${Filalyzer_SRCS})
target_link_libraries(filalyzer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS filalyzer RUNTIME DESTINATION bin)

cmake_minimum_required(VERSION 2.8.11)
project(filalyzer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Filalyzer_SRCS
  src/Filalyzer.cpp
  src/main.cpp src/HexFile.cpp
  src/custom_comp.cpp
  src/Hexedit.cpp
  src/Threading.cpp
  src/SettingsDialog.cpp
)

find_package(Qt5Widgets)
add_executable(${PROJECT_NAME} ${Filalyzer_SRCS})
if(${Qt5Widgets_FOUND})
  qt5_use_modules(${PROJECT_NAME} Widgets)
else(${Qt5Widgets_FOUND})
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
endif(${Qt5Widgets_FOUND})

install(TARGETS filalyzer RUNTIME DESTINATION bin)
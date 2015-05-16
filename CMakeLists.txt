cmake_minimum_required(VERSION 2.8.11)
project(Binaly)

add_subdirectory(doc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Binaly_SRCS
  src/Binaly.cpp
  src/main.cpp src/HexFile.cpp
  src/custom_comp.cpp
  src/Hexedit.cpp
  src/Threading.cpp
  src/SettingsDialog.cpp
  src/About.cpp
)

set(Lang_FILES
  lang/Binaly_de.ts
)

find_package(Qt5Widgets)
if(${Qt5Widgets_FOUND})
  find_package(Qt5LinguistTools)
  qt5_add_translation(TRANS_BIN ${Lang_FILES})
  add_executable(${PROJECT_NAME} ${Binaly_SRCS} ${TRANS_BIN})
  qt5_use_modules(${PROJECT_NAME} Widgets)
else(${Qt5Widgets_FOUND})
  find_package(Qt4 REQUIRED)
  qt4_add_translation(TRANS_BIN ${Lang_FILES})
  add_executable(${PROJECT_NAME} ${Binaly_SRCS} ${TRANS_BIN})
  include(${QT_USE_FILE})
  target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
endif(${Qt5Widgets_FOUND})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${TRANS_BIN} DESTINATION share/Binaly)
install(FILES icon/Binaly.png DESTINATION share/Binaly)
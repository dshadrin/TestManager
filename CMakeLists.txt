cmake_minimum_required(VERSION 3.11)

set(PROJECT_NAME "TestManager")
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

###
# Boost & platform configuration
###
if(MSVC)
  message(STATUS "Toolset: MSVC")
  add_definitions(-DWINVER=0x0600)
  add_definitions(-D_WIN32_WINNT=0x0600)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t /wd4996 /wd4503 /wd4251 /wd4180")
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  set(Boost_USE_STATIC_LIBS ON)
elseif(MINGW)
  message(STATUS "Toolset: gcc mingw")
  add_definitions(-DWINVER=0x0502)
  add_definitions(-D_WIN32_WINNT=0x0502)
  add_definitions(-U__STRICT_ANSI__)
  set(Boost_USE_STATIC_LIBS ON)
elseif(UNIX)
  message(STATUS "Toolset: gcc unix")
  set(Boost_USE_STATIC_LIBS OFF)
endif()
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost LIBRARIES: ${Boost_LIBRARIES}")

###
# SQL configuration
###
find_package(PostgreSQL QUIET REQUIRED)
message(STATUS "PostgreSQL INCLUDE: ${PostgreSQL_INCLUDE_DIRS}")
message(STATUS "PostgreSQL LIBRARIES: ${PostgreSQL_LIBRARIES}")
include_directories(${PostgreSQL_INCLUDE_DIRS})

###
# Qt configuration
###
if(WIN32)
  if(MSVC)
    message(STATUS "MSVC Version: " ${MSVC_VERSION})
    if(MSVC12)
      set(USED_QT_PATH "d:/Qt/5.10.1/msvc2013_64")
    elseif(${MSVC_VERSION} STREQUAL "1914")
      set(USED_QT_PATH "d:/Qt/5.10.1/msvc2017_64")
    else()
      message(FATAL_ERROR "Unsupported MSVC version")
    endif()
  elseif(MINGW)
    set(USED_QT_PATH "d:/Qt/5.10.1/mingw53_32")
  endif()
  set(ENV{CMAKE_PREFIX_PATH} "${USED_QT_PATH}/lib/cmake")
  message(STATUS "CMAKE_PREFIX_PATH: " $ENV{CMAKE_PREFIX_PATH})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)

set(SOURCES
  main.cpp
  DatabaseDef.cpp
)

set(HEADERS
	StdInc.h
	DatabaseDef.h
)

set(RESOURCES
)

set(FORMS
)

qt5_add_resources(RESOURCES_R ${RESOURCES})
qt5_wrap_ui(FORM_H ${FORMS})

###
# App configuration
###
set(DestinationDir ${CMAKE_SOURCE_DIR}/dist)
message(STATUS "DestinationDir: ${DestinationDir}")
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORM_H} ${RESOURCES_R})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets ${Boost_LIBRARIES} ${PostgreSQL_LIBRARIES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${DestinationDir})

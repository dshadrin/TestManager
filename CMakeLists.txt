cmake_minimum_required(VERSION 3.11)

include(config.cmake)
project(${PROJECT_NAME} LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 14)

set(APP_BUILD_PATH "${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Build path: " ${APP_BUILD_PATH})

set(APP_SOURCE_PATH "${CMAKE_SOURCE_DIR}")
message(STATUS "Source path: " ${APP_SOURCE_PATH})

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
find_package(PostgreSQL REQUIRED)
message(STATUS "PostgreSQL INCLUDE: ${PostgreSQL_INCLUDE_DIRS}")
message(STATUS "PostgreSQL LIBRARIES: ${PostgreSQL_LIBRARIES}")
include_directories(${PostgreSQL_INCLUDE_DIRS})

if(MSVC)
  set(ECPQ_LIB_NAME "libecpg.lib")
  set(ECPG_NAME "ecpg.exe")
elseif(MINGW)
  set(ECPQ_LIB_NAME "libecpg.lib")
  set(ECPG_NAME "ecpg.exe")
elseif(UNIX)
  set(ECPQ_LIB_NAME "libecpg.a")
  set(ECPG_NAME "ecpg")
endif()

# get PostgreSQL ROOT directory
get_filename_component(PG_ROOT ${PostgreSQL_LIBRARIES} DIRECTORY)
get_filename_component(PG_ROOT ${PG_ROOT} DIRECTORY)
message(STATUS "PostgreSQL root: ${PG_ROOT}")

#find PostgreSQL ecpq library
find_file(ECPQ_LIB NAMES ${ECPQ_LIB_NAME} PATHS ${PG_ROOT}/lib)
if(ECPQ_LIB-NOTFOUND)
  message(FATAL_ERROR ${ECPQ_LIB})
endif()
list(APPEND PostgreSQL_LIBRARIES ${ECPQ_LIB})
message(STATUS "PostgreSQL LIBRARIES: ${PostgreSQL_LIBRARIES}")

#find PostgreSQL ecpg preprocessor
find_file(ECPG_EXECUTABLE NAMES ${ECPG_NAME} PATHS ${PG_ROOT}/bin)
if(ECPG_EXECUTABLE-NOTFOUND)
  message(FATAL_ERROR ${ECPG_EXECUTABLE})
endif()
message(STATUS "ECPG_EXECUTABLE: " ${ECPG_EXECUTABLE})

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
  src/main.cpp
  src/DatabaseDef.cpp
  src/${PROJECT_NAME}.cpp
  src/Connection.cpp
)

set(HEADERS
  inc/StdInc.h
  inc/DatabaseDef.h
  inc/${PROJECT_NAME}.h
  inc/Connection.h
  pgc/CheckDbStructure.h
)
include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/pgc)

set(RESOURCES
  res/${PROJECT_NAME}.qrc
)

set(FORMS
  ui/${PROJECT_NAME}.ui
  ui/Connection.ui
)

set(PGC_SRC
  pgc/CheckDbStructure.pgc
)

qt5_add_resources(RESOURCES_R ${RESOURCES})
qt5_wrap_ui(FORM_H ${FORMS})

function(PGC_PREPROCESS outfiles infiles)
    set(pgc_files ${infiles})
    foreach(it ${pgc_files})
        get_filename_component(outfile ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/pgc_${outfile}.c)
        add_custom_command(OUTPUT ${outfile}
          COMMAND ${ECPG_EXECUTABLE}
          ARGS -o ${outfile} ${infile}
          MAIN_DEPENDENCY ${infile} VERBATIM)
        list(APPEND ${outfiles} ${outfile})
    endforeach()
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

pgc_preprocess(PGC_C ${PGC_SRC})

###
# App configuration
###
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORM_H} ${RESOURCES_R} ${PGC_C})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets ${Boost_LIBRARIES} ${PostgreSQL_LIBRARIES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${DestinationDir})

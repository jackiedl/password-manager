cmake_minimum_required(VERSION 3.16)

#set project name and version
if (WIN32)
	project(PasswordManager VERSION 1.0 LANGUAGES CXX)
elseif(UNIX)
	project(PasswordManager)
endif()

#======================= INCLUSION OF QT =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

#======================= INCLUSION OF OpenSSL =======================#
set(OPENSSL_ROOT_DIR "$ENV{OPENSSLDIR}")
find_package(OpenSSL REQUIRED)

#======================= INCLUSION OF SQLite3 =======================#
set(SQLite3_INCLUDE_DIR "$ENV{SQLITE3}")
set(SQLite3_LIBRARY "$ENV{SQLITE3}/sqlite3d.lib")
find_package(SQLite3 REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
	"${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

#=================== SETUP EXECTUABLE ====================#
# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
elseif (UNIX)
	add_executable(${PROJECT_NAME} ${SOURCES})
endif()
# Add the target includes for PasswordManager
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries (${PROJECT_NAME} SQLite::SQLite3)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets)


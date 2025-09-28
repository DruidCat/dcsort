cmake_minimum_required(VERSION 3.16)  # Qt6 требует CMake >= 3.16
project(sort VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON) # <- Важно: это обрабатывает .ui файлы
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Установка C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение необходимых модулей Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# Установка исходников
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE FORMS CONFIGURE_DEPENDS "src/*.ui")
file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS "src/*.qrc")

# Создание исполняемого файла
qt_add_executable(sort
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

# Подключение необходимых компонентов
target_link_libraries(sort PRIVATE
    Qt6::Widgets
    Qt6::Sql
)

#[===[
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11")
endif()

cmake_minimum_required(VERSION 3.0.0)
cmake_policy(SET CMP0020 NEW)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_BINARY_DIR bin)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt5Widgets REQUIRED)

SET (PROJECT		"sort")

FILE(GLOB HEADERS	"src/*.h")
FILE(GLOB SOURCES	"src/*.cpp")
FILE(GLOB FORMS		"src/*.ui")
FILE(GLOB RESOURCES	"src/*.qrc")

QT5_WRAP_UI(UI_HEADERS ${FORMS})
QT5_ADD_RESOURCES(QRC_RESOURCES ${RESOURCES})

ADD_EXECUTABLE (${PROJECT} WIN32 ${HEADERS} ${SOURCES} ${UI_HEADERS} ${QRC_RESOURCES})
QT5_USE_MODULES(${PROJECT} Widgets Sql)
]===]
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..

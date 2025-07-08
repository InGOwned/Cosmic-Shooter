cmake_minimum_required(VERSION 3.15)
project(arkanoid_game LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Для статической линковки SFML
add_definitions(-DSFML_STATIC)

set(SFML_ROOT "${CMAKE_SOURCE_DIR}/ext/SFML-3.0.0")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

# Путь к зависимостям SFML (FreeType, FLAC, Vorbis)
set(SFML_DEPS_ROOT "${SFML_ROOT}/lib")

# Поиск SFML
find_package(SFML 3.0 COMPONENTS Audio Graphics Window System REQUIRED)

# Настройка структуры проекта
set(SOURCE_DIR src)
set(INCLUDE_DIR inc)

# Список исходных файлов
file(GLOB SOURCES 
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.h"
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} 
    ${SOURCES}
    "${SOURCE_DIR}/main.cpp"
)

# Подключение заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR}
    "${SFML_ROOT}/include"
)

# Указание путей поиска библиотек
target_link_directories(${PROJECT_NAME} PRIVATE
    "${SFML_ROOT}/lib"
    "${SFML_DEPS_ROOT}"
)

# --- ИСПРАВЛЕННАЯ СЕКЦИЯ ЛИНКОВКИ ---
# Используем генераторные выражения для автоматического выбора Debug/Release библиотек

# Основные библиотеки SFML
set(SFML_LIBS
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
)

foreach(LIB ${SFML_LIBS})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        debug "${SFML_ROOT}/lib/${LIB}-s-d.lib"
        optimized "${SFML_ROOT}/lib/${LIB}-s.lib"
    )
endforeach()

# Зависимости SFML (все находятся в SFML_ROOT/lib)
set(SFML_DEPS
    freetype
    flac
    vorbisenc
    vorbisfile
    vorbis
    ogg
)

foreach(DEP ${SFML_DEPS})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        debug "${SFML_ROOT}/lib/${DEP}d.lib"
        optimized "${SFML_ROOT}/lib/${DEP}.lib"
    )
endforeach()

# Системные зависимости
target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32
    winmm
    gdi32
    user32
    advapi32
    ws2_32
)

# Добавляем sfml-main (отдельно, так как у него другое именование)
target_link_libraries(${PROJECT_NAME} PRIVATE
    debug "${SFML_ROOT}/lib/sfml-main-s-d.lib"
    optimized "${SFML_ROOT}/lib/sfml-main.lib"
)

# Настройка для Visual Studio
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

cmake_minimum_required(VERSION 3.15)

# Указываем путь к вашим локальным библиотекам SFML, Ogg, Vorbis
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/ext/SFML")

# Добавляем путь Vcpkg. Он будет использоваться для других библиотек
list(APPEND CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/x64-windows")

set(PROJECT_NAME arkanoid_game)
project("${PROJECT_NAME}" LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройка для Windows
if(WIN32)
    add_definitions(-DSFML_STATIC)
endif()

# Настройка структуры проекта
set(SOURCE_DIR src)
set(INCLUDE_DIR inc)

# Список исходных файлов
file(GLOB SOURCES 
    "${SOURCE_DIR}/*.cpp"
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} 
    ${SOURCES}
    "${SOURCE_DIR}/main.cpp"
)

# Подключение заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/${INCLUDE_DIR}
    "${SFML_ROOT}/include" # Явно указываем путь к include SFML
)

# Поиск зависимостей (эти библиотеки, вероятно, из Vcpkg)
find_package(OpenAL CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(flac REQUIRED)

# ИЗМЕНЕНО: Удалены find_package для Ogg и Vorbis

# Подключение библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE 
    "${SFML_ROOT}/lib/sfml-graphics-s.lib" # Явно указываем путь к статической библиотеке SFML Graphics
    "${SFML_ROOT}/lib/sfml-window-s.lib"  # Явно указываем путь к статической библиотеке SFML Window
    "${SFML_ROOT}/lib/sfml-system-s.lib"  # Явно указываем путь к статической библиотеке SFML System
    "${SFML_ROOT}/lib/sfml-audio-s.lib"   # Явно указываем путь к статической библиотеке SFML Audio
    OpenAL::OpenAL 
    Freetype::Freetype 
    ${flac_LIBRARIES}
    "${SFML_ROOT}/lib/ogg.lib"           # Явно указываем путь к библиотеке Ogg
    "${SFML_ROOT}/lib/vorbis.lib"        # Явно указываем путь к библиотеке Vorbis
    "${SFML_ROOT}/lib/vorbisenc.lib"     # Явно указываем путь к библиотеке Vorbisenc
    "${SFML_ROOT}/lib/vorbisfile.lib"    # Явно указываем путь к библиотеке Vorbisfile
)

# Дополнительные библиотеки для Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        winmm
        gdi32
    )
endif()

# Копирование DLL файлов (для Windows)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SFML_ROOT}/bin"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

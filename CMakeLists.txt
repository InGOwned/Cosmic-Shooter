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
# Для SFML библиотек
set(SFML_LIBS graphics window system audio)

foreach(lib ${SFML_LIBS})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        debug "${SFML_ROOT}/lib/sfml-${lib}-s-d.lib"
        optimized "${SFML_ROOT}/lib/sfml-${lib}-s.lib"
    )
endforeach()

# Остальные зависимости
target_link_libraries(${PROJECT_NAME} PRIVATE 
    OpenAL::OpenAL 
    Freetype::Freetype 
    ${flac_LIBRARIES}
    debug "${SFML_ROOT}/lib/ogg.lib"           # Если есть debug-версии
    debug "${SFML_ROOT}/lib/vorbis.lib"
    debug "${SFML_ROOT}/lib/vorbisenc.lib"
    debug "${SFML_ROOT}/lib/vorbisfile.lib"
    optimized "${SFML_ROOT}/lib/ogg.lib"        # Release-версии
    optimized "${SFML_ROOT}/lib/vorbis.lib"
    optimized "${SFML_ROOT}/lib/vorbisenc.lib"
    optimized "${SFML_ROOT}/lib/vorbisfile.lib"
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
# if(WIN32)
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#             "${SFML_ROOT}/bin"
#             $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     )
# endif()

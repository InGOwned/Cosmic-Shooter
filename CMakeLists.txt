cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/x64-windows")
find_package(OpenAL CONFIG REQUIRED)

set(PROJECT_NAME arkanoid_game)  # Более подходящее имя для проекта
project("${PROJECT_NAME}" LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройка для Windows
if(WIN32)
    add_definitions(-DSFML_STATIC)
endif()

set(SFML_DIR "E:/Загрузки/SFML-2.6.1-windows-vc17-64-bit/SFML-2.6.1/lib/cmake/SFML")
set(OpenAL_DIR "$ENV{VCPKG_ROOT}/installed/x64-windows/share/openal")
set(OGG_LIB_PATH "E:/Repositories/vcpkg/installed/x64-windows/lib")
set(VORBIS_LIB_PATH "E:/Repositories/vcpkg/installed/x64-windows/lib")

# Поиск SFML
find_package(SFML 2.6.1 COMPONENTS graphics window system audio REQUIRED)
link_directories("${SFML_DIR}/../../lib")

# Настройка структуры проекта
set(SOURCE_DIR src)
set(INCLUDE_DIR include)

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
target_include_directories(arkanoid_game PRIVATE
    ${CMAKE_SOURCE_DIR}/inc  # Для заголовков
    ${SFML_INCLUDE_DIR}      # Для SFML
)

find_package(Freetype REQUIRED)
find_package(flac REQUIRED)
find_package(Ogg REQUIRED)
find_package(Vorbis REQUIRED COMPONENTS vorbis vorbisenc vorbisfile)


link_directories("${SFML_DIR}/../../lib")
# Подключение SFML библиотек
target_link_libraries(arkanoid_game PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    OpenAL::OpenAL 
    ${flac_LIBRARIES} 
    Vorbis::vorbis 
    Vorbis::vorbisenc 
    Vorbis::vorbisfile 
    Ogg::ogg
)

# Дополнительные библиотеки для Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        freetype
        winmm
        gdi32
        # ogg
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
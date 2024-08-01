cmake_minimum_required(VERSION 3.10)

# Nome del progetto
project(MochiOS)

# Imposta lo standard C++
# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER /home/codespace/opt/cross/bin/i686-elf-gcc)
set(CMAKE_CXX_COMPILER /home/codespace/opt/cross/bin/i686-elf-g++)
# add_link_options("-T src/kernel/linker.ld")

# Aggiungi i file sorgente
set(SOURCES
    src/kernel/kernel.cpp
    src/UART-driver/uart-driver.cpp
)

# Aggiungi i file header
set(HEADERS
)

# Crea l'eseguibile
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")
# add_library(kernel OBJECT ${SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Includi le directory
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Aggiungi eventuali librerie necessarie
# target_link_libraries(${PROJECT_NAME} some_library)

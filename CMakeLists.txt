cmake_minimum_required(VERSION 3.10)

# Nome del progetto
project(MochiOS)

# Imposta lo standard C++
set(CMAKE_CXX_STANDARD 17)

# Aggiungi i file sorgente
set(SOURCES
    src/main.cpp
)

# Aggiungi i file header
set(HEADERS
)

# Crea l'eseguibile
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Includi le directory
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Aggiungi eventuali librerie necessarie
# target_link_libraries(${PROJECT_NAME} some_library)

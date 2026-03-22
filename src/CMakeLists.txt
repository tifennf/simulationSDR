# 1. On crée l'exécutable
add_executable(simulationSDR main.cpp transmitter.cpp)

# 2. On lui indique où trouver les headers
# PUBLIC signifie que si simulationSDR était une librairie,
# ceux qui l'utilisent auraient aussi besoin de ce dossier include.
target_include_directories(simulationSDR PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# 3. (Optionnel) Ajout de warnings pour le compilateur
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(simulationSDR PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(simulationSDR PRIVATE /W4)
endif()

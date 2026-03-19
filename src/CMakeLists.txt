# 1. On crée l'exécutable
add_executable(mon_executable
  main.cpp
  transmitter.cpp
)

# 2. On lui indique où trouver les headers
# PUBLIC signifie que si mon_executable était une librairie,
# ceux qui l'utilisent auraient aussi besoin de ce dossier include.
target_include_directories(mon_executable PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# 3. (Optionnel) Ajout de warnings pour le compilateur
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(mon_executable PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(mon_executable PRIVATE /W4)
endif()

# 1. On crée l'exécutable
add_executable(simulator main.cpp transmitter.cpp receiver.cpp)

# 2. On lui indique où trouver les headers
# PUBLIC signifie que si simulation était une librairie,
# ceux qui l'utilisent auraient aussi besoin de ce dossier include.
target_include_directories(simulator PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(simulator PRIVATE CLI11::CLI11)

# 3. (Optionnel) Ajout de warnings pour le compilateur
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(simulator PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(simulator PRIVATE /W4)
endif()

# 4. Gestion de l'option ENABLE_STATS
if(ENABLE_STATS)
  target_compile_definitions(simulator PRIVATE ENABLE_STATS)
endif()

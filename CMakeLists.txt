cmake_minimum_required(VERSION 3.20)

# Définition du projet et du langage
project(MonProjet
    VERSION 1.0.0
    LANGUAGES CXX
)

# Forcer le standard C++ (ex: C++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Désactive les extensions spécifiques aux compilateurs (ex: gnu++20)

# Inclusion des sous-dossiers
add_subdirectory(src)

# Décommenter plus tard pour les tests
# enable_testing()
# add_subdirectory(tests)

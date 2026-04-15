https://github.com/tifennf/simulationSDR

## Initialisation du projet
```
cmake -S . -B build
```

### Options
```
-DENABLE_STATS=ON
```

## Compiler
```
cmake --build build
```

## Lancer les tests
Après avoir compilé, se placer dans le dossier build/ puis exécuter la commande
```
ctest --output-on-failure
```


## Initialisation du projet
```
cmake -S . -B build
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

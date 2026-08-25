# Simulateur d'une chaîne de traitement SDR (Software-Defined-Radio)
Le simulateur a été optimisé pour tourner efficacement sur une NVIDIA Jetson Orin Nano.

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

## Scripts
- *run_sim.sh:* Lance les 5 simulations du Lab 2
- *lab4_run_sim.sh:* Lance les 5 simulations avec l'option --src-all-zeros puis avec l'option --mod-all-ones
- *find_f_s.sh:* Lance les 5 simulations avec toutes les combinaisons de f et s possibles
- *run_refs.sh:* Lance les simulations détaillées à la fin du lab4

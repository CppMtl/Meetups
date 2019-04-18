# Dépendances

- C++17
- CMake 3.12
- Catch 2 (incorporé)

# Instructions avec CMake
Afin de bâtir et d'exécuter la cible de tests automatisés présentée durant la présentation, il faut exécuter les commandes suivantes :

1. `mkdir Build`
1. `cd Build`
1. `cmake -DCMAKE_BUILD_TYPE=Debug ..`
1. `cmake --build .`
1. `./AsyncButton/AsyncButton`

# Structure du code

Il y a deux cibles dans le projet : AsyncButton et AsyncSystem. AsyncButton est la cible de tests automatisés. Son fichier source SyncTests.cpp montre l'approche synchrone. Les fichiers sources AsyncTests1.cpp, AsyncTests2.cpp et AsyncTests3.cpp correspondent respectivement à la comparaison de journaux d'événements, la consommation d'événements et la simulation de l'aggrégation des événements. La deuxième cible, AsyncSystem, consiste en des outils génériques tel que les templates `AsyncSystem` et `AsyncSystemTestRig` qui ont été étudiés lors de la présentation.

- AsyncSystem
  - src
    - AsyncButton.cpp
    - AsyncTests1.cpp
    - AsyncTests2.cpp
    - AsyncTests3.cpp
    - ButtonControlledMissileSystem.cpp
    - main.cpp
    - SyncTests.cpp

- AsyncSystemTestRig
  - include
    - AsyncSystem.hpp
    - AsyncSystemTestRig.hpp
    - VariableCondition.hpp

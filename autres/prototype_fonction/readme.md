# Les prototypes de fonctions avec Arduino <!-- omit in toc -->

# Introduction
Les prototypes de fonctions sont des fonctions qui sont déclarées avant leur utilisations et définies après. Ils sont utilisés pour permettre à une fonction d'être utilisée avant sa définition. Cela permet de définir les fonctions dans l'ordre qui nous convient le mieux.

Voici un exemple de prototype :

```cpp
// Prototype de fonction
void maFonction(int param1, int param2);

void fonctionX() {
    maFonction (1, 2);
}

void maFonction(int param1, int param2) {
    // ...
}

```


Avec Arduino, ceux-ci sont optionnels si la fonction n'a pas de paramètres optionnels. Ainsi le code précédent pourrait ressembler à ceci :

```cpp
void fonctionX() {
    maFonction (1, 2);
}

void maFonction(int param1, int param2) {
    // ...
}

```

Cependant, si des paramètres optionnels sont utilisés, il est nécessaire de déclarer le prototype de la fonction avant de l'utiliser. Par exemple :

```cpp
// Prototype de fonction avec paramètre optionnel
void maFonction(int param1, int param2 = 0);

void fonctionX() {
    maFonction (1);
}

void maFonction(int param1, int param2) {
    // ...
}

```

# Résumé
- Les prototypes de fonctions sont des fonctions qui sont déclarées avant leur utilisations et définies après.
- Les prototypes de fonctions sont optionnels si la fonction n'a pas de paramètres optionnels.
- Les prototypes de fonctions sont obligatoires si la fonction a des paramètres optionnels.


# ConvexHull -- NUCCI Killian SEBAN Nicolas - TD4/TP6

# Compilation
Pour ce projet, nous avons fait le choix d'utiliser un Makefile afin de faciliter la compilation. Vous pouvez donc utiliser cette commande en vous situant à la racine du projet :
```sh
make
```
Le fichier exécutable se trouvera ensuite dans le dossier `build`, auquel vous devrez accéder avant de lancer le programme (La police de caractère se situe à cet endroit, et est nécessaire au bon fonctionnement).

A noter, il est possible de passer en paramètre à `make`, `DEBUG_CVH_CLEANING=yes` afin d'animer pas à pas, l'algorithme de nettoyage.\
Vous pouvez ignorer l'animation pendant l'ajout manuel des points en restant appuyé sur majuscule gauche. Pour avancer dans l'animation, vous pourrez appuyer sur espace.\
Nous l'avions réalisé afin de comprendre les étapes de l'algorithme, mais nous n'avions pas souhaité l'ajouter définitivement comme une option au programme car l'actualisation de l'affichage est insérée directement dans le code de l'algorithme de nettoyage, et cela aurait sinon nécessité un paramètre supplémentaire, qui n'aurait pas sa place dans une fonction devant être purement algorithmique.\
Vous pouvez donc l'activer de la manière suivante :
```sh
make clean
make DEBUG_CVH_CLEANING=yes
```

# Utilisation

Voici une façon d'exécuter le programme avec les paramètres :
```sh
cd build
./demo --window 500x500 -n 500 --concentration 2.5 -p -a
```

- `-w`, `--window`        taille de la fenêtre au format XXXxXXX, par défaut `800x500`
- `-n`, `--points`        nombre de points à générer, par défaut `1000`
- `-c`, `--concentration` puissance de la concentration centrée, par defaut `3`
- `-r`, `--radius`        rayon dans lequel la génration aléatoire opère, par défaut le rayon minimal de la fenêtre 
- `-a`, `--animation`     spécifie si l'animation est activée
- `-p`, `--progressive`   spécifie si le tri est activé
- `-h`, `--help`          message d'aide

Certaines options sont modifiables directement dans le menu comme le tri, l'animation mais également le mode de génération : cercle ou carré, et encore le placement des points à la main par l'utilisateur.
Après avoir accédé à un mode d'utilisation, vous pouvez appuyer sur Echap pour revenir au menu principal.

## Génération de points

Pour la génération de points en cercle, la concentration est paramétrable avec un nombre flottant :
- Égal à 0, les points se génèrent sur le cercle
- Entre 0 et 1, les points s'écartent du centre, et se limitent au rayon
- Égal à 1, la répartition est uniforme
- Plus grand que 1, les points se centrent à l'origine
- Inférieur à 0, les points se créent à l'extérieur du cercle, mais aussi probablement à l'extérieur de la fenêtre.

Pour la génération carrée, les points sont dispersés uniformément si égal à 1, sinon de manière croissante à partir de l'origine.

Dans tous les cas, il est possible de trier les points en fonction de l'origine, ce qui peut amener l'effet de croissance des polygônes.

# Documentation

Les fichiers sources ont été soigneusement documentés lorsque nécessaire, avec l'outil `doxygen`. Ainsi, vous pouvez la générer en utilisant la commande :
```sh
doxygen
```
Nous avons utilisé un thème pour doxygen, mais nous ne l'avons pas intégré à l'archive, il est donc possible d'obtenir un avertissement pendant la génération de la documentation, expliquant que doxygen reprend les fichiers de styles par défaut.

# Améliorations et bugs

- La concentration de la génération carrée n'est pas paramétrable, ainsi si le paramètre concentration vaut 1, la génération sera uniforme sinon croissante indépendament de la valeur.

- Une génération de points gausienne pourrait être intéressante.

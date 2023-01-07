# ConvexHull -- NUCCI Killian SEBAN Nicolas - TD4/TP6

# Compilation
Pour ce projet, nous avons fait le choix d'utiliser un Makefile afin de faciliter la compilation. Vous pouvez donc utiliser cette commande en vous situant à la racine du projet :
```sh
make
```
Le fichier exécutable se trouvera ensuite dans le dossier `build`.

A noter, il est possible de passer en paramètre à `make`, `-DDEBUG_CVH_CLEANING=yes` afin d'animer pas à pas, l'algorithme de nettoyage de la partie 1.\
Vous pouvez ignorer l'animation pendant l'ajout manuel des points en restant appuyé sur majuscule gauche. Pour avancer dans l'animation, vous pourrez appuyer sur espace.
Nous l'avions créé afin de comprendre les étapes de l'algorithme, mais nous n'avions pas souhaité l'ajouter définitivement comme une option au programme :
```sh
make clean
make -DDEBUG_CVH_CLEANING=yes
```


# Utilisation

Voici une façon d'exécuter le programme avec les paramètres:
```sh
cd build
./demo --window 500x500 -n 100 --concentration 3 --radius 250 -p
```

- `-w`, `--window`        taille de la fenêtre au format XXXxXXX, par défaut `500x500`
- `-n`, `--points`        nombre de points à générer, par défaut `1000`
- `-c`, `--concentration` puissance de la concentration centrée `3`
- `-r`, `--radius`        rayon dans lequel la génration aléatoire opère, par défaut le rayon de la fenêtre 
- `-p`, `--progressive`   spécifie si le tri est activé
- `-h`, `--help`          message d'aide

Certaines options sont modifiables directement dans le menu comme le tri et l'animation mais également si l'on souhaite générer un cercle ou un carré, ou encore le placement des points a la main par l'utilisateur.


# Documentation
Les fichiers sources ont été soigneusement documentés lorsque nécessaire, avec l'outil `doxygen`. Ainsi, vous pouvez la générer en utilisant la commande :
```sh
doxygen
```
Nous avons utilisé un thème pour doxygen, mais nous ne l'avons pas intégré à l'archive, il est donc possible d'obtenir un avertissement pendant la génération de la documentation, expliquant que doxygen reprend les fichiers de styles par défaut.

# Amélioration et bugs

- La concentration de la génération carrée n'est pas paramétrable, ainsi si le paramètre concentration vaut 1, la génération sera uniforme sinon croissante indépendamente de la valeur.


# ConvexHull -- NUCCI Killian SEBAN Nicolas - TD4/TP6

# Compilation
Pour ce projet, nous avons fait le choix d'utiliser un Makefile afin de faciliter la compilation. Vous pouvez donc utiliser cette commande en vous situant à la racine du projet :
```sh
make
```
Le fichier exécutable se trouvera ensuite dans le dossier `build`.

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
./demo --window 500x500 -n 100 --concentration 3 --radius 250 -p
```

- `-w`, `--window`        taille de la fenêtre au format XXXxXXX, par défaut `800x500`
- `-n`, `--points`        nombre de points à générer, par défaut `1000`
- `-c`, `--concentration` puissance de la concentration centrée, par defaut `3`
- `-r`, `--radius`        rayon dans lequel la génration aléatoire opère, par défaut le rayon minimal de la fenêtre 
- `-a`, ``--animation``   spécifie si l'animation est activée
- `-p`, `--progressive`   spécifie si le tri est activé
- `-h`, `--help`          message d'aide

Certaines options sont modifiables directement dans le menu comme le tri et l'animation mais également si l'on souhaite générer un cercle ou un carré, ou encore le placement des points à la main par l'utilisateur.

# Documentation

Les fichiers sources ont été soigneusement documentés lorsque nécessaire, avec l'outil `doxygen`. Ainsi, vous pouvez la générer en utilisant la commande :
```sh
doxygen
```
Nous avons utilisé un thème pour doxygen, mais nous ne l'avons pas intégré à l'archive, il est donc possible d'obtenir un avertissement pendant la génération de la documentation, expliquant que doxygen reprend les fichiers de styles par défaut.

# Amélioration et bugs

- La concentration de la génération carrée n'est pas paramétrable, ainsi si le paramètre concentration vaut 1, la génération sera uniforme sinon croissante indépendament de la valeur.

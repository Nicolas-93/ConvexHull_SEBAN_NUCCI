#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "args.h"

int ARG_min(int a, int b);

void print_help(void) {
    printf(
        "Utilisation : demo\n"
        "options :\n"
        "  -w, --window        taille de la fenetre au format XXxXX, par defaut 800x600\n"
        "  -n, --points        nombre de points a generer, par defaut 1000\n"
        "  -c, --concentration puissance de la concentration centree, par defaut 3\n"
        "  -r, --radius        rayon dans lequel la genration aleatoire opere, par defaut le rayon minimal de la fenetre\n"
        "  -a, --animation     specifie si l'animation est active"
        "  -p, --progressive   specifie si le tri est actif"
        "  -h, --help          ce message d'aide"
    );
}

Parameters parse_args(int argc, char *argv[]) {
    static Parameters params = {
        .window.width = 800, .window.height = 500,
        .gen = {
            .shape = CERCLE,
            .nb_points = 1000,
            .rayon = 0,
            .concentration = 3,
            .progressif = false,
            .animation = false
        },
        .inception = false
    };
    static struct option long_options[] = {
        {"window",        required_argument, NULL, 'w'},
        {"radius",        required_argument, NULL, 'r'},
        {"points",        required_argument, NULL, 'n'},
        {"concentration", required_argument, NULL, 'c'},
        {"progressive",   no_argument,       NULL, 'p'},
        {"animation",     no_argument,       NULL, 'a'},
        {"help",          required_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    char* end_ind;
    char c;
    while ((c = getopt_long(argc, argv, "w:r:n:c:t:hp", long_options,
                       &option_index)) != -1) {
        switch (c) {
        case 'w':
            if (sscanf(optarg, "%dx%d", &params.window.width, &params.window.height) != 2 ||
                params.window.width < 100 || params.window.height < 100
            ) {
                fprintf(stderr, "Veuillez entrer une taille de fenetre superieure à 100x100\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'r':
            if ((params.gen.rayon = atoi(optarg)) <= 0) {
                fprintf(stderr, "Format de rayon incorrect.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'n':
            if ((params.gen.nb_points = atoi(optarg)) <= 0) {
                fprintf(stderr, "Veuilez demander au moins 1 point à generer.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'c':
            params.gen.concentration = strtod(optarg, &end_ind);
            if (end_ind == optarg) {
                fprintf(stderr, "Format de l'argument concentration incorrect.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'p':
            params.gen.progressif = true;
            break;
        case 'a':
            params.gen.animation = true;
            break;
        case 'h':
        case '?':
        default:
            print_help();
            exit(EXIT_FAILURE);
        }
    }
    if (params.gen.rayon == 0) {
        params.gen.rayon = ARG_min(params.window.width, params.window.height) / 2 - 20;
    }
    return params;
}

int ARG_min(int a, int b) {
    return a < b ? a : b;
}

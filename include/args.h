#ifndef H_ARGS
#define H_ARGS
#include <stdbool.h>
#include "UI.h"

typedef struct Parameters {
    struct {
        int nb_points;
        int rayon;
        double concentration;
        int progressif;
    } gen;
    Size window;
    int thickness;
} Parameters;

void print_help(void);
Parameters parse_args(int argc, char *argv[]);

#endif

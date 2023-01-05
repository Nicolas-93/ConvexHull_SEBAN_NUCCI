#ifndef H_ARGS
#define H_ARGS
#include <stdbool.h>
#include "UI.h"

enum Shape {CARRE, CERCLE};
typedef struct Parameters {
    struct {
        enum Shape shape;
        int enabled;
        int nb_points;
        int rayon;
        double concentration;
        int progressif;
        bool animation;
    } gen;
    Size window;
    int thickness;
} Parameters;

void print_help(void);
Parameters parse_args(int argc, char *argv[]);

#endif

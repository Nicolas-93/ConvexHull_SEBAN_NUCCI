#ifndef H_ARGS
#define H_ARGS

enum Shape {CARRE, CERCLE};
typedef struct {
    struct {
        enum Shape shape;
        int enabled;
        int nb_points;
        int rayon;
        double concentration;
        int progressif;
        int animation;
    } gen;
    struct {
        int width;
        int height;
    } window;
    int inception;
    int thickness;
} Parameters;

void print_help(void);
Parameters parse_args(int argc, char *argv[]);

#endif

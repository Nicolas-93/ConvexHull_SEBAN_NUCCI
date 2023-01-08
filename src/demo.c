#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "args.h"
#include "scenes.h"

int main(int argc, char* argv[]) {
    Parameters params = parse_args(argc, argv);
    MLV_create_window("Convex Hull", "", params.window.width, params.window.height);
    SCN_menu(&params);
    MLV_free_window();
    exit(EXIT_SUCCESS);
}

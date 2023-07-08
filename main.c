#include <stdio.h>
#include "gui.h"

int main(int argc, char* argv[]) {
    Gui* gui = gui_new();
    if (!gui) {
        fprintf(stderr, "Could not initialize GUI.\n");
        return 1;
    }

    gui_run(gui);

    gui_free(gui);

    return 0;
}

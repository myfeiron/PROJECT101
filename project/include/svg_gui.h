//bonus part
#ifndef SVG_GUI_H
#define SVG_GUI_H

#include <SDL.h>
#include "svg_types.h"

// GUI constants
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600
#define TOOLBAR_WIDTH 200
#define TOOLBAR_HEIGHT 600

// GUI state
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* canvas_texture;
    SvgDocument* document;
    int selected_shape;
    int mouse_x, mouse_y;
    int dragging;
    int pan_x, pan_y;
    float zoom;
    int show_toolbar;
} GUIState;

// Function declarations
int gui_init(GUIState* state);
void gui_cleanup(GUIState* state);
void gui_render(GUIState* state);
void gui_handle_events(GUIState* state, int* running);
void gui_draw_shape(SDL_Renderer* renderer, SvgShape* shape, int offset_x, int offset_y, float zoom);
void gui_draw_toolbar(GUIState* state);
void gui_add_shape_at_mouse(GUIState* state, SvgShapeType type);
void run_gui(SvgDocument* doc);

#endif // SVG_GUI_H

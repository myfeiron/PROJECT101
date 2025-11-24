#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <SDL.h>
#include "../include/svg_gui.h"
#include "../include/svg_types.h"
#include "../include/svg_parser.h"
#include "../include/svg_render.h"
#include "../include/svg_writer.h"

int gui_init(GUIState* state) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 0;
    }

    state->window = SDL_CreateWindow("SVG Editor - ENGR1010J",
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!state->window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 0;
    }

    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
    if (!state->renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return 0;
    }

    state->canvas_texture = SDL_CreateTexture(state->renderer, SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_TARGET, CANVAS_WIDTH, CANVAS_HEIGHT);
    if (!state->canvas_texture) {
        printf("Canvas texture creation failed: %s\n", SDL_GetError());
        return 0;
    }

    // Initialize state
    state->document = NULL;
    state->selected_shape = -1;
    state->mouse_x = 0;
    state->mouse_y = 0;
    state->dragging = 0;
    state->pan_x = 0;
    state->pan_y = 0;
    state->zoom = 1.0f;
    state->show_toolbar = 1;

    return 1;
}

void gui_cleanup(GUIState* state) {
    if (state->canvas_texture) SDL_DestroyTexture(state->canvas_texture);
    if (state->renderer) SDL_DestroyRenderer(state->renderer);
    if (state->window) SDL_DestroyWindow(state->window);
    if (state->document) free_svg_document(state->document);
    SDL_Quit();
}

void gui_render(GUIState* state) {
    // Clear screen
    SDL_SetRenderDrawColor(state->renderer, 240, 240, 240, 255);
    SDL_RenderClear(state->renderer);

    // Render canvas area
    SDL_Rect canvas_rect = {0, 0, CANVAS_WIDTH, CANVAS_HEIGHT};
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(state->renderer, &canvas_rect);

    // Render SVG shapes
    if (state->document) {
        SvgShape* current = state->document->shapes;
        int index = 0;
        while (current) {
            gui_draw_shape(state->renderer, current, state->pan_x, state->pan_y, state->zoom);

            // Highlight selected shape
            if (index == state->selected_shape) {
                SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
                switch (current->type) {
                    case SVG_SHAPE_CIRCLE:
                        // Draw selection rectangle around circle
                        SDL_Rect sel_rect = {
                            (int)((current->data.circle.cx - current->data.circle.r) * state->zoom + state->pan_x) - 2,
                            (int)((current->data.circle.cy - current->data.circle.r) * state->zoom + state->pan_y) - 2,
                            (int)(current->data.circle.r * 2 * state->zoom) + 4,
                            (int)(current->data.circle.r * 2 * state->zoom) + 4
                        };
                        SDL_RenderDrawRect(state->renderer, &sel_rect);
                        break;
                    case SVG_SHAPE_RECT:
                        SDL_Rect sel_rect2 = {
                            (int)(current->data.rect.x * state->zoom + state->pan_x) - 2,
                            (int)(current->data.rect.y * state->zoom + state->pan_y) - 2,
                            (int)(current->data.rect.width * state->zoom) + 4,
                            (int)(current->data.rect.height * state->zoom) + 4
                        };
                        SDL_RenderDrawRect(state->renderer, &sel_rect2);
                        break;
                    case SVG_SHAPE_LINE:
                        // Draw thicker line for selection
                        SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
                        SDL_RenderDrawLine(state->renderer,
                                        (int)(current->data.line.x1 * state->zoom + state->pan_x),
                                        (int)(current->data.line.y1 * state->zoom + state->pan_y),
                                        (int)(current->data.line.x2 * state->zoom + state->pan_x),
                                        (int)(current->data.line.y2 * state->zoom + state->pan_y));
                        break;
                }
            }

            current = current->next;
            index++;
        }
    }

    // Render toolbar
    if (state->show_toolbar) {
        gui_draw_toolbar(state);
    }

    SDL_RenderPresent(state->renderer);
}

void gui_draw_shape(SDL_Renderer* renderer, SvgShape* shape, int offset_x, int offset_y, float zoom) {
    switch (shape->type) {
        case SVG_SHAPE_CIRCLE: {
            SvgCircle* circle = &shape->data.circle;
            int cx = (int)(circle->cx * zoom + offset_x);
            int cy = (int)(circle->cy * zoom + offset_y);
            int r = (int)(circle->r * zoom);

            // Parse fill color
            int r_col = 255, g_col = 255, b_col = 255;
            if (circle->fill && strlen(circle->fill) > 1) {
                sscanf(circle->fill + 1, "%2x%2x%2x", &r_col, &g_col, &b_col);
            }

            SDL_SetRenderDrawColor(renderer, r_col, g_col, b_col, 255);

            // Draw filled circle using midpoint algorithm
            int x = 0, y = r;
            int d = 3 - 2 * r;
            while (x <= y) {
                for (int i = cx - x; i <= cx + x; i++) {
                    SDL_RenderDrawPoint(renderer, i, cy + y);
                    SDL_RenderDrawPoint(renderer, i, cy - y);
                }
                for (int i = cx - y; i <= cx + y; i++) {
                    SDL_RenderDrawPoint(renderer, i, cy + x);
                    SDL_RenderDrawPoint(renderer, i, cy - x);
                }
                if (d < 0) d += 4 * x + 6;
                else d += 4 * (x - y--) + 10;
                x++;
            }
            break;
        }
        case SVG_SHAPE_RECT: {
            SvgRect* rect = &shape->data.rect;
            SDL_Rect sdl_rect = {
                (int)(rect->x * zoom + offset_x),
                (int)(rect->y * zoom + offset_y),
                (int)(rect->width * zoom),
                (int)(rect->height * zoom)
            };

            int r_col = 255, g_col = 255, b_col = 255;
            if (rect->fill && strlen(rect->fill) > 1) {
                sscanf(rect->fill + 1, "%2x%2x%2x", &r_col, &g_col, &b_col);
            }

            SDL_SetRenderDrawColor(renderer, r_col, g_col, b_col, 255);
            SDL_RenderFillRect(renderer, &sdl_rect);
            break;
        }
        case SVG_SHAPE_LINE: {
            SvgLine* line = &shape->data.line;
            int r_col = 0, g_col = 0, b_col = 0;
            if (line->stroke && strlen(line->stroke) > 1) {
                sscanf(line->stroke + 1, "%2x%2x%2x", &r_col, &g_col, &b_col);
            }

            SDL_SetRenderDrawColor(renderer, r_col, g_col, b_col, 255);
            SDL_RenderDrawLine(renderer,
                            (int)(line->x1 * zoom + offset_x),
                            (int)(line->y1 * zoom + offset_y),
                            (int)(line->x2 * zoom + offset_x),
                            (int)(line->y2 * zoom + offset_y));
            break;
        }
    }
}

void gui_draw_toolbar(GUIState* state) {
    // Toolbar background
    SDL_Rect toolbar_rect = {CANVAS_WIDTH, 0, TOOLBAR_WIDTH, TOOLBAR_HEIGHT};
    SDL_SetRenderDrawColor(state->renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(state->renderer, &toolbar_rect);

    // Toolbar border
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(state->renderer, &toolbar_rect);

    // Simple text labels (in a real implementation, you'd use SDL_ttf)
    // For now, just draw colored rectangles for buttons
    SDL_Rect button1 = {CANVAS_WIDTH + 10, 10, 180, 40};
    SDL_SetRenderDrawColor(state->renderer, 100, 200, 100, 255);
    SDL_RenderFillRect(state->renderer, &button1);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(state->renderer, &button1);

    SDL_Rect button2 = {CANVAS_WIDTH + 10, 60, 180, 40};
    SDL_SetRenderDrawColor(state->renderer, 200, 100, 100, 255);
    SDL_RenderFillRect(state->renderer, &button2);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(state->renderer, &button2);

    SDL_Rect button3 = {CANVAS_WIDTH + 10, 110, 180, 40};
    SDL_SetRenderDrawColor(state->renderer, 100, 100, 200, 255);
    SDL_RenderFillRect(state->renderer, &button3);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(state->renderer, &button3);
}

void gui_handle_events(GUIState* state, int* running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    state->mouse_x = event.button.x;
                    state->mouse_y = event.button.y;

                    // Check if clicking in toolbar
                    if (state->show_toolbar && event.button.x >= CANVAS_WIDTH) {
                        int button_y = (event.button.y - 10) / 50;
                        if (button_y >= 0 && button_y <= 2) {
                            SvgShapeType types[] = {SVG_SHAPE_CIRCLE, SVG_SHAPE_RECT, SVG_SHAPE_LINE};
                            gui_add_shape_at_mouse(state, types[button_y]);
                        }
                    } else {
                        // Check for shape selection
                        state->selected_shape = -1;
                        if (state->document) {
                            SvgShape* current = state->document->shapes;
                            int index = 0;
                            while (current) {
                                int hit = 0;
                                float mx = (event.button.x - state->pan_x) / state->zoom;
                                float my = (event.button.y - state->pan_y) / state->zoom;

                                switch (current->type) {
                                    case SVG_SHAPE_CIRCLE:
                                        if ((mx - current->data.circle.cx) * (mx - current->data.circle.cx) +
                                            (my - current->data.circle.cy) * (my - current->data.circle.cy) <=
                                            current->data.circle.r * current->data.circle.r) {
                                            hit = 1;
                                        }
                                        break;
                                    case SVG_SHAPE_RECT:
                                        if (mx >= current->data.rect.x && mx <= current->data.rect.x + current->data.rect.width &&
                                            my >= current->data.rect.y && my <= current->data.rect.y + current->data.rect.height) {
                                            hit = 1;
                                        }
                                        break;
                                    case SVG_SHAPE_LINE:
                                        // Simple line hit detection (could be improved)
                                        double dist = fabs((current->data.line.y2 - current->data.line.y1) * mx -
                                                         (current->data.line.x2 - current->data.line.x1) * my +
                                                         current->data.line.x2 * current->data.line.y1 -
                                                         current->data.line.y2 * current->data.line.x1) /
                                                     sqrt((current->data.line.y2 - current->data.line.y1) *
                                                           (current->data.line.y2 - current->data.line.y1) +
                                                           (current->data.line.x2 - current->data.line.x1) *
                                                           (current->data.line.x2 - current->data.line.x1));
                                        if (dist < 5.0) hit = 1;
                                        break;
                                }

                                if (hit) {
                                    state->selected_shape = index;
                                    state->dragging = 1;
                                    break;
                                }

                                current = current->next;
                                index++;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    state->dragging = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                state->mouse_x = event.motion.x;
                state->mouse_y = event.motion.y;

                if (state->dragging && state->selected_shape >= 0 && state->document) {
                    SvgShape* current = state->document->shapes;
                    int index = 0;
                    while (current && index < state->selected_shape) {
                        current = current->next;
                        index++;
                    }
                    if (current) {
                        float dx = (event.motion.xrel) / state->zoom;
                        float dy = (event.motion.yrel) / state->zoom;

                        switch (current->type) {
                            case SVG_SHAPE_CIRCLE:
                                current->data.circle.cx += dx;
                                current->data.circle.cy += dy;
                                break;
                            case SVG_SHAPE_RECT:
                                current->data.rect.x += dx;
                                current->data.rect.y += dy;
                                break;
                            case SVG_SHAPE_LINE:
                                current->data.line.x1 += dx;
                                current->data.line.y1 += dy;
                                current->data.line.x2 += dx;
                                current->data.line.y2 += dy;
                                break;
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_DELETE:
                        if (state->selected_shape >= 0 && state->document) {
                            // Remove selected shape (simplified implementation)
                            SvgShape* current = state->document->shapes;
                            SvgShape* prev = NULL;
                            int index = 0;
                            while (current && index < state->selected_shape) {
                                prev = current;
                                current = current->next;
                                index++;
                            }
                            if (current) {
                                if (prev) prev->next = current->next;
                                else state->document->shapes = current->next;
                                
                                // Free allocated strings
                                switch (current->type) {
                                    case SVG_SHAPE_CIRCLE:
                                        if (current->data.circle.fill) free(current->data.circle.fill);
                                        break;
                                    case SVG_SHAPE_RECT:
                                        if (current->data.rect.fill) free(current->data.rect.fill);
                                        break;
                                    case SVG_SHAPE_LINE:
                                        if (current->data.line.stroke) free(current->data.line.stroke);
                                        break;
                                }
                                
                                free(current);
                                state->selected_shape = -1;
                                printf("Shape deleted\n");
                            }
                        }
                        break;
                    case SDLK_s:
                        if (state->document) {
                            char filename[256];
                            time_t t = time(NULL);
                            struct tm *tm_info = localtime(&t);
                            snprintf(filename, sizeof(filename), "output_%04d%02d%02d_%02d%02d%02d.svg",
                                   tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
                                   tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
                            if (svg_save_to_file(filename, state->document) == 0) {
                                printf("Saved to %s\n", filename);
                            } else {
                                printf("Failed to save file\n");
                            }
                        }
                        break;
                    case SDLK_t:
                        state->show_toolbar = !state->show_toolbar;
                        break;
                    case SDLK_ESCAPE:
                        *running = 0;
                        break;
                }
                break;
        }
    }
}

void gui_add_shape_at_mouse(GUIState* state, SvgShapeType type) {
    if (!state->document) {
        state->document = create_svg_document(800, 600);
    }

    SvgShape* new_shape = (SvgShape*)malloc(sizeof(SvgShape));
    if (!new_shape) return;

    new_shape->type = type;
    new_shape->next = NULL;

    float mx = (state->mouse_x - state->pan_x) / state->zoom;
    float my = (state->mouse_y - state->pan_y) / state->zoom;

    switch (type) {
        case SVG_SHAPE_CIRCLE:
            new_shape->data.circle.cx = mx;
            new_shape->data.circle.cy = my;
            new_shape->data.circle.r = 30.0f;
            new_shape->data.circle.fill = strdup("#FF0000");
            break;
        case SVG_SHAPE_RECT:
            new_shape->data.rect.x = mx - 25;
            new_shape->data.rect.y = my - 25;
            new_shape->data.rect.width = 50.0f;
            new_shape->data.rect.height = 50.0f;
            new_shape->data.rect.fill = strdup("#00FF00");
            break;
        case SVG_SHAPE_LINE:
            new_shape->data.line.x1 = mx - 25;
            new_shape->data.line.y1 = my - 25;
            new_shape->data.line.x2 = mx + 25;
            new_shape->data.line.y2 = my + 25;
            new_shape->data.line.stroke = strdup("#0000FF");
            break;
    }

    // Add to end of list
    if (!state->document->shapes) {
        state->document->shapes = new_shape;
    } else {
        SvgShape* current = state->document->shapes;
        while (current->next) current = current->next;
        current->next = new_shape;
    }
}

void run_gui(SvgDocument* doc) {
    GUIState state;
    int running = 1;

    if (!gui_init(&state)) {
        return;
    }

    // Use provided document or create empty one
    if (doc) {
        state.document = doc;
    } else {
        state.document = create_svg_document(800, 600);
    }

    while (running) {
        gui_handle_events(&state, &running);
        gui_render(&state);
        SDL_Delay(16); // ~60 FPS
    }

    gui_cleanup(&state);
}

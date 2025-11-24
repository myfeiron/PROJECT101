#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
#include "../include/jpg_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

// Helper function to draw a pixel
static void draw_pixel(uint8_t *pixels, int width, int height, int x, int y, uint32_t color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    
    int index = (y * width + x) * 3;
    pixels[index + 0] = (color >> 16) & 0xFF; // R
    pixels[index + 1] = (color >> 8) & 0xFF;  // G
    pixels[index + 2] = color & 0xFF;         // B
}

// Draw a circle using midpoint algorithm
static void draw_circle(uint8_t *pixels, int width, int height, SvgCircle *circle) {
    int cx = (int)circle->cx;
    int cy = (int)circle->cy;
    int r = (int)circle->r;

    // Parse color
    uint32_t color = 0xFFFFFF;
    if (circle->fill && strlen(circle->fill) > 1) {
        sscanf(circle->fill + 1, "%6x", &color);
    }

    // Fill circle
    for (int y = cy - r; y <= cy + r; y++) {
        for (int x = cx - r; x <= cx + r; x++) {
            int dx = x - cx;
            int dy = y - cy;
            if (dx * dx + dy * dy <= r * r) {
                draw_pixel(pixels, width, height, x, y, color);
            }
        }
    }
}

// Draw a rectangle
static void draw_rect(uint8_t *pixels, int width, int height, SvgRect *rect) {
    int x1 = (int)rect->x;
    int y1 = (int)rect->y;
    int x2 = x1 + (int)rect->width;
    int y2 = y1 + (int)rect->height;

    // Parse color
    uint32_t color = 0xFFFFFF;
    if (rect->fill && strlen(rect->fill) > 1) {
        sscanf(rect->fill + 1, "%6x", &color);
    }

    for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
            draw_pixel(pixels, width, height, x, y, color);
        }
    }
}

// Draw a line using Bresenham's algorithm
static void draw_line(uint8_t *pixels, int width, int height, SvgLine *line) {
    int x1 = (int)line->x1;
    int y1 = (int)line->y1;
    int x2 = (int)line->x2;
    int y2 = (int)line->y2;

    // Parse color
    uint32_t color = 0x000000;
    if (line->stroke && strlen(line->stroke) > 1) {
        sscanf(line->stroke + 1, "%6x", &color);
    }

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        draw_pixel(pixels, width, height, x1, y1, color);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

int export_to_jpg(const char *filename, const SvgDocument *doc, int quality) {
    if (!doc || !filename) return -1;
    
    int width = (int)doc->width;
    int height = (int)doc->height;
    
    // Validate quality
    if (quality < 1) quality = 1;
    if (quality > 100) quality = 100;
    
    // Allocate pixel buffer (RGB format)
    uint8_t *pixels = (uint8_t *)calloc(width * height * 3, sizeof(uint8_t));
    if (!pixels) return -1;
    
    // Fill with white background
    memset(pixels, 255, width * height * 3);
    
    // Render all shapes
    SvgShape *current = doc->shapes;
    while (current) {
        switch (current->type) {
            case SVG_SHAPE_CIRCLE:
                draw_circle(pixels, width, height, &current->data.circle);
                break;
            case SVG_SHAPE_RECT:
                draw_rect(pixels, width, height, &current->data.rect);
                break;
            case SVG_SHAPE_LINE:
                draw_line(pixels, width, height, &current->data.line);
                break;
        }
        current = current->next;
    }
    
    // Write JPG file using stb_image_write
    int result = stbi_write_jpg(filename, width, height, 3, pixels, quality);
    
    free(pixels);
    
    return result ? 0 : -1;
}

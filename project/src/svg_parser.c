#include "../include/svg_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



// Helper function to extract attribute value
static int get_attribute(const char *line, const char *attr, char *value, int max_len) {
    char search[64];
    snprintf(search, sizeof(search), "%s=\"", attr);
    
    const char *start = strstr(line, search);
    if (!start) return 0;
    
    start += strlen(search);
    const char *end = strchr(start, '"');
    if (!end) return 0;
    
    int len = end - start;
    if (len >= max_len) len = max_len - 1;
    
    strncpy(value, start, len);
    value[len] = '\0';
    return 1;
}

int svg_load_from_file(const char *filename, SvgDocument **doc_out) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1;
    }
    
    SvgDocument *doc = (SvgDocument *)malloc(sizeof(SvgDocument));
    if (!doc) {
        fclose(file);
        return -1;
    }
    
    doc->width = 800.0;
    doc->height = 600.0;
    doc->shapes = NULL;
    
    SvgShape *last_shape = NULL;
    int shape_id = 0;
    char line[1024];
    
    while (fgets(line, sizeof(line), file)) {
        char value[256];
        
        // Parse SVG dimensions
        if (strstr(line, "<svg")) {
            if (get_attribute(line, "width", value, sizeof(value))) {
                doc->width = atof(value);
            }
            if (get_attribute(line, "height", value, sizeof(value))) {
                doc->height = atof(value);
            }
        }
        
        // Parse circle
        if (strstr(line, "<circle")) {
            SvgShape *shape = (SvgShape *)malloc(sizeof(SvgShape));
            shape->type = SVG_SHAPE_CIRCLE;
            shape->id = ++shape_id;
            shape->next = NULL;
            
            if (get_attribute(line, "cx", value, sizeof(value)))
                shape->data.circle.cx = atof(value);
            if (get_attribute(line, "cy", value, sizeof(value)))
                shape->data.circle.cy = atof(value);
            if (get_attribute(line, "r", value, sizeof(value)))
                shape->data.circle.r = atof(value);
            if (get_attribute(line, "fill", value, sizeof(value)))
                shape->data.circle.fill = strdup(value);
            
            if (!doc->shapes) {
                doc->shapes = shape;
            } else {
                last_shape->next = shape;
            }
            last_shape = shape;
        }
        
        // Parse rectangle
        if (strstr(line, "<rect")) {
            SvgShape *shape = (SvgShape *)malloc(sizeof(SvgShape));
            shape->type = SVG_SHAPE_RECT;
            shape->id = ++shape_id;
            shape->next = NULL;
            
            if (get_attribute(line, "x", value, sizeof(value)))
                shape->data.rect.x = atof(value);
            if (get_attribute(line, "y", value, sizeof(value)))
                shape->data.rect.y = atof(value);
            if (get_attribute(line, "width", value, sizeof(value)))
                shape->data.rect.width = atof(value);
            if (get_attribute(line, "height", value, sizeof(value)))
                shape->data.rect.height = atof(value);
            if (get_attribute(line, "fill", value, sizeof(value)))
                shape->data.rect.fill = strdup(value);
            
            if (!doc->shapes) {
                doc->shapes = shape;
            } else {
                last_shape->next = shape;
            }
            last_shape = shape;
        }
        
        // Parse line
        if (strstr(line, "<line")) {
            SvgShape *shape = (SvgShape *)malloc(sizeof(SvgShape));
            shape->type = SVG_SHAPE_LINE;
            shape->id = ++shape_id;
            shape->next = NULL;
            
            if (get_attribute(line, "x1", value, sizeof(value)))
                shape->data.line.x1 = atof(value);
            if (get_attribute(line, "y1", value, sizeof(value)))
                shape->data.line.y1 = atof(value);
            if (get_attribute(line, "x2", value, sizeof(value)))
                shape->data.line.x2 = atof(value);
            if (get_attribute(line, "y2", value, sizeof(value)))
                shape->data.line.y2 = atof(value);
            if (get_attribute(line, "stroke", value, sizeof(value)))
                shape->data.line.stroke = strdup(value);
            
            if (!doc->shapes) {
                doc->shapes = shape;
            } else {
                last_shape->next = shape;
            }
            last_shape = shape;
        }
    }
    
    fclose(file);
    *doc_out = doc;
    return 0;
}

SvgDocument* create_svg_document(float width, float height) {
    SvgDocument *doc = (SvgDocument *)malloc(sizeof(SvgDocument));
    if (!doc) return NULL;

    doc->width = width;
    doc->height = height;
    doc->shapes = NULL;

    return doc;
}

void svg_free_document(SvgDocument *doc) {
    if (!doc) return;

    SvgShape *current = doc->shapes;
    while (current) {
        SvgShape *next = current->next;
        
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
        current = next;
    }

    free(doc);
}

// Alias for GUI compatibility
void free_svg_document(SvgDocument *doc) {
    svg_free_document(doc);
}

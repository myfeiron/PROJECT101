#include "../include/svg_writer.h"
#include <stdio.h>
#include <stdlib.h>

int svg_save_to_file(const char *filename, const SvgDocument *doc) {
    if (!doc || !filename) return -1;
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot create file %s\n", filename);
        return -1;
    }
    
    // Write SVG header
    fprintf(file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(file, "<svg width=\"%.0f\" height=\"%.0f\" xmlns=\"http://www.w3.org/2000/svg\">\n", 
            doc->width, doc->height);
    
    // Write all shapes
    SvgShape *current = doc->shapes;
    while (current) {
        switch (current->type) {
            case SVG_SHAPE_CIRCLE:
                fprintf(file, "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" fill=\"%s\"/>\n",
                       current->data.circle.cx,
                       current->data.circle.cy,
                       current->data.circle.r,
                       current->data.circle.fill ? current->data.circle.fill : "#000000");
                break;
            
            case SVG_SHAPE_RECT:
                fprintf(file, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" fill=\"%s\"/>\n",
                       current->data.rect.x,
                       current->data.rect.y,
                       current->data.rect.width,
                       current->data.rect.height,
                       current->data.rect.fill ? current->data.rect.fill : "#000000");
                break;
            
            case SVG_SHAPE_LINE:
                fprintf(file, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\"/>\n",
                       current->data.line.x1,
                       current->data.line.y1,
                       current->data.line.x2,
                       current->data.line.y2,
                       current->data.line.stroke ? current->data.line.stroke : "#000000");
                break;
        }
        current = current->next;
    }
    
    // Write SVG footer
    fprintf(file, "</svg>\n");
    
    fclose(file);
    return 0;
}

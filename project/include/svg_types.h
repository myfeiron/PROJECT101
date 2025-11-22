#ifndef SVG_TYPES_H
#define SVG_TYPES_H

//define a struct SvgShapeType to list the types of different shapes
typedef enum {
    SVG_SHAPE_CIRCLE,//circle
    SVG_SHAPE_RECT,//rectangle
    SVG_SHAPE_LINE//line
} SvgShapeType;

//show the characters of circles
typedef struct {
    double cx, cy, r;//coordinates and radius
    char* fill; // colors e.g., "#RRGGBB" 
} SvgCircle;

typedef struct {
    double x, y, width, height;
    char* fill; // e.g., "#RRGGBB"
} SvgRect;

typedef struct {
    double x1, y1, x2, y2;
    char* stroke; //line doesn't have fill, only stroke e.g., "#RRGGBB"
} SvgLine;

typedef struct SvgShape {
    SvgShapeType type;//note the current shape type
    union {
        SvgCircle circle;
        //if type==svg_shape_circle, the data is collected in data.circle
        SvgRect rect;
        SvgLine line;
    } data;
    int id;//number every shape
    struct SvgShape *next;//self-reference structure, the pointer of the next one
} SvgShape;

//display the 
typedef struct {
    double width, height;// of the whole document
    SvgShape *shapes;//point to the head of shape list
} SvgDocument;

#endif

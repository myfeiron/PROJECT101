#ifndef SVG_PARSER_H
#define SVG_PARSER_H

#include "svg_types.h"

//read the svg file ; load the shapes and docement
//return:0 -> success
int svg_load_from_file(const char *filename, SvgDocument **doc_out);

//dynamically create an empty svg file, return a pointer that points to the new file
SvgDocument* create_svg_document(float width, float height);

//free all the svgshapes in every data table
void svg_free_document(SvgDocument *doc);
void free_svg_document(SvgDocument *doc); // Alias for compatibility

#endif

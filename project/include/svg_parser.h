#ifndef SVG_PARSER_H
#define SVG_PARSER_H

#include "svg_types.h"

int svg_load_from_file(const char *filename, SvgDocument **doc_out);
SvgDocument* create_svg_document(float width, float height);
void svg_free_document(SvgDocument *doc);
void free_svg_document(SvgDocument *doc); // Alias for compatibility

#endif

#ifndef RENDER_CONSOLE_H
#define RENDER_CONSOLE_H

#include "svg_types.h"

//print out the information of the svg document(summarize)
void svg_print_summary(const SvgDocument *doc);

//print out the information of every shapes in detail(accord with the project requirement)
void svg_print_shapes(const SvgDocument *doc);

#endif

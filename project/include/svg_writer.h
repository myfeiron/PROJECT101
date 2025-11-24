#ifndef SVG_WRITER_H
#define SVG_WRITER_H

#include "svg_types.h"

int svg_save_to_file(const char *filename, const SvgDocument *doc);

#endif

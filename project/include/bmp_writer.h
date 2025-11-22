#ifndef BMP_WRITER_H
#define BMP_WRITER_H

#include "svg_types.h"

int export_to_bmp(const char *filename, const SvgDocument *doc);

//*filename: the filename of the BMP file to be writtrn into
//Svgdocument *doc: read the input svg file

#endif

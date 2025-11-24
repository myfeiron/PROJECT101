#ifndef JPG_WRITER_H
#define JPG_WRITER_H

#include "svg_types.h"

// Export SVG document to JPG format
// quality: 1-100, higher is better quality (recommended: 90)
int export_to_jpg(const char *filename, const SvgDocument *doc, int quality);

#endif // JPG_WRITER_H

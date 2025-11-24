# Code Demonstration - For Presentation

## 1️⃣ Core Data Structure (svg_types.h)

```c
// Shape type enumeration
typedef enum {
    SVG_SHAPE_CIRCLE,  // Circle
    SVG_SHAPE_RECT,    // Rectangle
    SVG_SHAPE_LINE     // Line
} SvgShapeType;

// Shape node (linked list)
typedef struct SvgShape {
    SvgShapeType type;      // Shape type
    union {                 // Union saves memory!
        SvgCircle circle;   // Circle data
        SvgRect rect;       // Rectangle data
        SvgLine line;       // Line data
    } data;
    struct SvgShape *next;  // Pointer to next shape
} SvgShape;
```

**Explanation:** Union allows different shapes to share memory, linked list enables dynamic operations

---

## 2️⃣ Circle Drawing Algorithm (bmp_writer.c)

```c
// Draw circle using midpoint circle algorithm
static void draw_circle(uint8_t *pixels, int width, int height, 
                       SvgCircle *circle) {
    int cx = (int)circle->cx;  // Center x
    int cy = (int)circle->cy;  // Center y
    int r = (int)circle->r;    // Radius
    
    // Fill the circle
    for (int y = cy - r; y <= cy + r; y++) {
        for (int x = cx - r; x <= cx + r; x++) {
            int dx = x - cx;
            int dy = y - cy;
            // Check if point is inside circle
            if (dx * dx + dy * dy <= r * r) {
                draw_pixel(pixels, width, height, x, y, color);
            }
        }
    }
}
```

**Explanation:** Iterate through bounding square, use distance formula to check if point is inside circle

---

## 3️⃣ Mondrian's "Composition" Code (mondrian.svg)

```xml
<svg width="1000" height="750">
  <!-- White background -->
  <rect x="0" y="0" width="1000" height="750" fill="#F0F0F0"/>
  
  <!-- Large red block (top left) -->
  <rect x="100" y="100" width="250" height="280" fill="#DD3333"/>
  
  <!-- Blue block (top right) -->
  <rect x="520" y="20" width="180" height="140" fill="#003399"/>
  
  <!-- Yellow block -->
  <rect x="370" y="20" width="130" height="60" fill="#FFDD00"/>
  
  <!-- Black grid lines (horizontal) -->
  <rect x="0" y="18" width="1000" height="4" fill="#000000"/>
  <rect x="0" y="98" width="1000" height="4" fill="#000000"/>
  
  <!-- Black grid lines (vertical) -->
  <rect x="98" y="0" width="4" height="750" fill="#000000"/>
  <rect x="368" y="0" width="4" height="750" fill="#000000"/>
</svg>
```

**Explanation:** 
- Primary colors (red, blue, yellow) + black and white
- 4-pixel wide black lines as grid
- Create art using only rectangles!

---

## 📊 Performance Comparison

| Format | File Size | Compression |
|--------|-----------|-------------|
| BMP | 1406 KB | 0% |
| JPG(90) | 19 KB | **98.6%** ⭐ |

---

## 🎯 Key Points for Demo

1. **Data Structure**: Emphasize Union's memory optimization
2. **Algorithm**: Briefly explain circle drawing logic
3. **Artwork**: Show simplicity of SVG code
4. **Performance**: JPG compression is impressive

---

**Demo Time: ~2 minutes**

# SVG Processing System - 5-Minute Presentation

## 📋 Presentation Outline (5 Minutes)

### Minute 1: Project Introduction (0:00-1:00)
**Speaker: Member 1 (Project Leader)**

Good morning/afternoon everyone! We are Team XX, and today we're excited to present our SVG Processing System.

**Project Overview:**
- A complete SVG processing system developed in C
- Supports parsing, editing, and multi-format export
- Features an interactive GUI editor

**Core Features:**
1. ✅ SVG file parsing (circles, rectangles, lines)
2. ✅ BMP export (uncompressed)
3. ✅ JPG export (quality control, 98% smaller files)
4. ✅ Interactive GUI editor
5. ✅ SVG file saving

---

### Minute 2: Code Demonstration & Explanation (1:00-2:00)
**Speakers: Member 2 (Parser) + Member 3 (Exporter)**

**Member 2: Core Data Structure**

Show `svg_types.h`:
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

**Key Points:**
- Union allows different shapes to share memory space
- Linked list structure enables dynamic add/remove
- Each shape only uses necessary memory

---

**Member 3: Graphics Algorithm Implementation**

Show circle drawing from `bmp_writer.c`:
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
            // Check if point is inside circle: distance <= radius
            if (dx * dx + dy * dy <= r * r) {
                draw_pixel(pixels, width, height, x, y, color);
            }
        }
    }
}
```

**Key Points:**
- Iterate through bounding square
- Use distance formula to check if point is inside circle
- Simple and efficient filling algorithm

**JPG Export Comparison:**
- BMP: 1406 KB (uncompressed)
- JPG: 19 KB (98.6% smaller!)

---

### Minute 3: Live Demonstration (2:00-3:00)
**Speakers: Member 4 (GUI) + Member 5 (Interaction)**

**Live Demo:**

1. **Parse SVG File**
```cmd
build\svg_processor.exe -p assets\demo.svg
```
Show: Console output with shape information

2. **Export to JPG**
```cmd
build\svg_processor.exe -ej assets\demo.svg output.jpg 90
```
Show: Generated JPG file

3. **Launch GUI Editor**
```cmd
build\svg_processor.exe -g assets\demo.svg
```

**GUI Operations Demo:**
- Click to select shapes
- Drag to move
- Add new shapes from toolbar
- Press S to save
- Press ESC to exit

---

### Minute 4: Creative Showcase + Code (3:00-4:00)
**Speaker: Member 6 (Documentation)**

**1. Mondrian's "Composition"**

Show mondrian.jpg, then show the code:

```xml
<!-- Mondrian style: Pure color rectangles + black grid lines -->
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

**Key Points:**
- Create art using only rectangles
- Primary colors (red, blue, yellow) + black and white
- 4-pixel wide black lines as grid
- Perfect demonstration of our rectangle functionality

---

**2. "The Starry Night" Simplified**

Show starry_night_v2.jpg

**Technical Highlights:**
- Multi-layer circles simulate star halos
- Circle combinations create swirling clouds
- Black cypress tree stacked with multiple circles
- Creative use of circles

---

### Minute 5: Summary & Q&A (4:00-5:00)
**Speaker: Member 1 (Project Leader)**

**Project Achievements:**
- Code: 1110 lines of C code
- Modules: 7 functional modules
- Formats: SVG, BMP, JPG
- Shapes: Circles, rectangles, lines

**Team Division:**
- 6-person team with clear responsibilities
- Each member responsible for independent module
- Collaborative development, completed on time

**Technology Stack:**
- C99 standard
- SDL2 graphics library
- stb_image_write image library

**Q&A Time**
Questions are welcome!

---

## 🎯 Presentation Checklist

### Prepare in Advance:
- [ ] Ensure project is compiled (run build.bat)
- [ ] Prepare demo files (demo.svg, starry_night_v2.jpg, mondrian.jpg)
- [ ] Test all commands to ensure they work
- [ ] Prepare GUI demo (open in advance)

### Demo Equipment:
- [ ] Projector/large screen
- [ ] Laptop
- [ ] Backup USB drive (with all files)

### Division of Labor:
- Member 1: Opening introduction + Summary
- Member 2: Data structure explanation
- Member 3: Algorithm implementation explanation
- Member 4: GUI demo (operate computer)
- Member 5: GUI functionality explanation
- Member 6: Artwork showcase

---

## 💡 Speaking Tips

### Opening (Member 1):
"Good morning everyone! Today we're presenting an SVG processing system developed entirely in C. You might ask, why C? Because we wanted to challenge ourselves and implement every detail of graphics processing from the ground up."

### Technical Explanation (Members 2-3):
"We use a linked list to store shapes, allowing dynamic addition and deletion. Look at this Union design - it lets us store different shape types using minimal memory."

### Demo Section (Members 4-5):
"Now let's see it in action. First, we'll parse an SVG file... (operate) ...As you can see, it identifies all the shapes. Now let's launch the GUI editor... (operate) ...You can directly drag shapes with the mouse."

### Creative Showcase (Member 6):
"We didn't just implement basic features - we also created artwork with it. This is a simplified version of Van Gogh's 'The Starry Night', where we used 200 lines to fill a triangular roof. This is Mondrian's 'Composition', perfectly showcasing our rectangle and line capabilities."

### Closing (Member 1):
"Through this project, we not only learned C programming but also understood the underlying principles of graphics processing. Thank you!"

---

## 🎬 Demo Script

### Command Line Demo Script:
```cmd
@echo off
echo ========================================
echo SVG Processing System - Live Demo
echo ========================================
echo.

echo [1] Parse SVG File
build\svg_processor.exe -p assets\demo.svg
pause

echo.
echo [2] Export to JPG (quality 90)
build\svg_processor.exe -ej assets\demo.svg demo_live.jpg 90
pause

echo.
echo [3] Launch GUI Editor
build\svg_processor.exe -g assets\demo.svg
```

Save as `demo_presentation.bat`

---

## 📊 Backup Information

### If Someone Asks Technical Details:

**Q: Why choose C language?**
A: C lets us understand low-level implementation. There's no "magic" from high-level languages - every line of code is written by us.

**Q: Why use SDL2?**
A: SDL2 is a cross-platform graphics library, lightweight yet powerful, perfect for our GUI needs.

**Q: How is JPG export implemented?**
A: We use the stb_image_write single-header library, which provides a simple JPG encoding interface.

**Q: Can you support more shapes?**
A: Yes! Our architecture is modular. Adding new shape types only requires extending the Union and corresponding rendering functions.

**Q: How's the performance?**
A: For medium-sized SVGs (100-200 shapes), parsing and rendering complete within 1 second.

---

## 🎯 Success Points

1. **Time Control**: Strictly control time for each section
2. **Smooth Demo**: Practice multiple times in advance
3. **Backup Plan**: If GUI fails, show screenshots
4. **Team Coordination**: Natural and smooth transitions
5. **Confident Expression**: Showcase our achievements

---

**Good luck with your presentation! 🎉**

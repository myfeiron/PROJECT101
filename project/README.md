# SVG Processing System - ENGR1010J Project

## 项目概述

这是一个用C语言实现的完整SVG（可缩放矢量图形）处理系统，支持解析、显示、编辑和导出SVG文件。

## 快速开始

### 1. 编译项目
```cmd
cd project
build.bat
```

### 2. 运行演示
```cmd
run_demo.bat
```

### 3. 或直接使用
```cmd
# 解析SVG
build\svg_processor.exe -p assets\demo.svg

# 导出BMP
build\svg_processor.exe -eb assets\demo.svg output.bmp

# 启动GUI编辑器
build\svg_processor.exe -g assets\demo.svg
```

## Features
- Parse SVG files with circle, rectangle, and line elements
- Display parsed SVG content in console
- Export SVG to BMP format
- Modular code structure for easy maintenance

## Build Instructions

### Windows
使用提供的构建脚本：
```cmd
cd project
build.bat
```

或手动编译：
```cmd
gcc -Wall -Wextra -std=c99 -O2 -Iinclude -Igui_libs\SDL2-2.30.6\include src\main.c src\svg_parser.c src\svg_render.c src\bmp_writer.c src\svg_gui.c -o build\svg_processor.exe -Lgui_libs\SDL2-2.30.6\lib\x64 -lSDL2 -lm
```

### 依赖项
- GCC编译器
- SDL2库（已包含在 gui_libs/ 目录中）
- Windows操作系统

## Usage

### Parse and Display SVG
```cmd
build/svg_processor.exe --parse assets/test.svg
build/svg_processor.exe -p assets/test.svg
```

### Export to BMP
```cmd
build/svg_processor.exe --export_bmp assets/test.svg assets/output.bmp
build/svg_processor.exe -eb assets/test.svg assets/output.bmp
```

### Launch Interactive GUI Editor
```cmd
build\svg_processor.exe --gui [assets\test.svg]
build\svg_processor.exe -g [assets\test.svg]
```

**GUI功能：**
- 鼠标点击选择图形
- 拖拽移动图形
- 工具栏添加新图形（圆形、矩形、线条）
- DELETE键删除选中的图形
- T键切换工具栏显示
- ESC键退出编辑器
- 实时预览和编辑

## Project Structure
```
project/
├── src/
│   ├── main.c           # Main program entry point
│   ├── svg_parser.c     # SVG file parsing
│   ├── svg_render.c     # Console output rendering
│   └── bmp_writer.c     # BMP export functionality
├── include/
│   ├── svg_types.h      # Data structure definitions
│   ├── svg_parser.h
│   ├── svg_render.h
│   └── bmp_writer.h
├── assets/
│   ├── test.svg         # Sample SVG file
│   └── output.bmp       # Generated BMP output
├── build/
│   └── svg_processor.exe # Compiled executable
├── docs/
│   └── [documentation files]
├── Makefile             # Build configuration
└── README.md            # This file
```

## Supported SVG Elements
- `<circle>` - with cx, cy, r, fill attributes
- `<rect>` - with x, y, width, height, fill attributes
- `<line>` - with x1, y1, x2, y2, stroke attributes

## Implementation Notes
- Uses linked list for shape storage
- BMP export uses 24-bit RGB format
- Bresenham's algorithm for line drawing
- Simple scanline rendering for shapes

## 测试

运行测试脚本：
```cmd
test.bat
```

这将测试所有主要功能：解析、BMP导出和GUI编辑器。

## 技术亮点

1. **模块化设计**：代码分为独立的模块（解析、渲染、导出、GUI）
2. **内存管理**：正确的内存分配和释放，避免内存泄漏
3. **图形算法**：
   - Bresenham算法用于线条绘制
   - 中点圆算法用于圆形绘制
   - 扫描线填充算法
4. **交互式GUI**：使用SDL2实现的完整图形编辑器
5. **跨平台兼容**：标准C99代码，易于移植

## 已完成功能
- [x] SVG文件解析（圆形、矩形、线条）
- [x] 控制台输出显示
- [x] BMP格式导出
- [x] 交互式GUI编辑器
- [x] 图形选择和拖拽
- [x] 动态添加/删除图形
- [x] 内存管理优化

## 未来改进
- [ ] 添加JPG/PNG导出支持
- [ ] 支持更多SVG元素（多边形、路径、文本）
- [ ] 颜色选择器
- [ ] 撤销/重做功能
- [ ] 保存为SVG文件
- [ ] 图形变换（旋转、缩放）

## 项目成员
[在此添加团队成员姓名]

## License
Educational project for ENGR1010J

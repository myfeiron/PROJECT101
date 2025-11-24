# SVG Processing System - ENGR1010J Project

## 项目概述
用C语言实现的SVG处理系统，支持解析、编辑和多格式导出。

## 快速开始

### 编译
```cmd
build.bat
```

### 使用
```cmd
# 解析SVG
build\svg_processor.exe -p assets\demo.svg

# 导出BMP
build\svg_processor.exe -eb assets\demo.svg output.bmp

# 导出JPG（新增！质量1-100）
build\svg_processor.exe -ej assets\demo.svg output.jpg 90

# GUI编辑器
build\svg_processor.exe -g assets\demo.svg
```

## 核心功能
- ✅ SVG文件解析（圆形、矩形、线条）
- ✅ 控制台显示
- ✅ BMP导出（无压缩）
- ✅ JPG导出（支持质量调节，文件小98%）
- ✅ 交互式GUI编辑器
- ✅ SVG保存

## GUI操作
- 🖱️ 点击选择，拖拽移动
- 🎨 工具栏添加图形（圆/矩/线）
- ⌨️ DELETE删除，S保存，T切换工具栏，ESC退出

## 技术特点
- Bresenham线条算法、中点圆算法
- SDL2图形库、stb_image_write图像库
- 模块化设计、完整内存管理

## 项目结构
```
project/
├── src/          # 源代码（7个模块）
├── include/      # 头文件
├── assets/       # 测试SVG
├── build/        # 可执行文件
└── gui_libs/     # SDL2库
```

## 文档
- README.md - 本文档
- TEAM_DIVISION.md - 团队分工
- TODO.md - 任务清单

---
**ENGR1010J Course Project**

# SVG处理系统 - 用户指南

## 简介

这是一个用C语言开发的SVG（可缩放矢量图形）处理系统，支持解析、显示、编辑和导出SVG文件。

## 功能特性

### 1. SVG文件解析
- 支持读取标准SVG文件
- 解析圆形（circle）、矩形（rect）、线条（line）元素
- 提取颜色、位置、尺寸等属性

### 2. 控制台显示
- 在命令行中显示SVG文档信息
- 列出所有图形元素及其属性

### 3. BMP图像导出
- 将SVG转换为BMP位图格式
- 24位真彩色输出
- 使用Bresenham算法绘制线条
- 使用中点圆算法绘制圆形

### 4. 交互式GUI编辑器
- 可视化编辑SVG图形
- 鼠标拖拽移动图形
- 动态添加新图形
- 删除选中的图形
- 保存编辑结果

## 使用方法

### 解析SVG文件

```cmd
build\svg_processor.exe --parse assets\test.svg
```

输出示例：
```
SVG Document: width=800.00, height=600.00
Total shapes: 3
[1] CIRCLE: cx=100.00, cy=200.00, r=50.00, fill=#FF0000
[2] RECT: x=10.00, y=20.00, width=200.00, height=100.00, fill=#00FF00
[3] LINE: from (0.00,0.00) to (150.00,150.00), stroke=#0000FF
```

### 导出为BMP图像

```cmd
build\svg_processor.exe --export_bmp assets\test.svg output.bmp
```

这将创建一个output.bmp文件，包含渲染后的SVG图形。

### 启动GUI编辑器

```cmd
build\svg_processor.exe --gui assets\test.svg
```

或创建新文档：
```cmd
build\svg_processor.exe --gui
```

## GUI编辑器操作指南

### 基本操作

1. **选择图形**
   - 用鼠标左键点击任意图形
   - 选中的图形会显示红色边框

2. **移动图形**
   - 选中图形后，按住鼠标左键拖拽
   - 释放鼠标完成移动

3. **添加图形**
   - 点击右侧工具栏的按钮
   - 绿色按钮：添加圆形（红色）
   - 红色按钮：添加矩形（绿色）
   - 蓝色按钮：添加线条（蓝色）

### 键盘快捷键

| 按键 | 功能 |
|------|------|
| DELETE | 删除选中的图形 |
| S | 保存当前文档为SVG文件 |
| T | 显示/隐藏工具栏 |
| ESC | 退出编辑器 |

### 保存文件

按S键保存时，文件会自动命名为：
```
output_YYYYMMDD_HHMMSS.svg
```

例如：`output_20241124_153045.svg`

## 技术实现

### 数据结构

项目使用链表存储SVG图形：

```c
typedef struct SvgShape {
    SvgShapeType type;
    union {
        SvgCircle circle;
        SvgRect rect;
        SvgLine line;
    } data;
    int id;
    struct SvgShape *next;
} SvgShape;
```

### 图形算法

1. **Bresenham线条算法**
   - 高效的整数运算
   - 无需浮点计算
   - 用于绘制直线

2. **中点圆算法**
   - 对称性优化
   - 只计算1/8圆弧
   - 用于绘制圆形

3. **扫描线填充**
   - 逐行填充像素
   - 用于矩形和圆形填充

### 内存管理

- 动态分配图形节点
- 正确释放字符串内存
- 避免内存泄漏
- 链表遍历和清理

## 故障排除

### 问题：找不到SDL2.dll

**解决方案**：
确保SDL2.dll在以下位置之一：
- `build\SDL2.dll`
- 系统PATH路径中

### 问题：无法打开SVG文件

**解决方案**：
- 检查文件路径是否正确
- 确保文件格式为有效的SVG
- 使用相对路径或绝对路径

### 问题：GUI窗口无法显示

**解决方案**：
- 确保SDL2库正确安装
- 检查显卡驱动是否更新
- 尝试以管理员权限运行

## 项目结构

```
project/
├── src/              # 源代码
│   ├── main.c        # 主程序入口
│   ├── svg_parser.c  # SVG解析
│   ├── svg_render.c  # 控制台渲染
│   ├── svg_writer.c  # SVG保存
│   ├── bmp_writer.c  # BMP导出
│   └── svg_gui.c     # GUI编辑器
├── include/          # 头文件
├── assets/           # 测试文件
├── build/            # 编译输出
├── gui_libs/         # SDL2库
├── build.bat         # 构建脚本
├── test.bat          # 测试脚本
└── README.md         # 项目说明
```

## 扩展开发

### 添加新的图形类型

1. 在`svg_types.h`中添加新的枚举值
2. 在union中添加新的结构体
3. 在解析器中添加解析逻辑
4. 在渲染器中添加绘制逻辑

### 添加新的导出格式

1. 创建新的writer模块（如jpg_writer.c）
2. 实现导出函数
3. 在main.c中添加命令行选项

## 许可证

本项目为ENGR1010J课程教育项目。

## 联系方式

如有问题或建议，请联系项目团队成员。

# 代码详解 - 项目运行逻辑完整讲解

## 🎯 项目整体架构

```
用户输入命令
    ↓
main.c (主程序入口)
    ↓
根据命令选择功能：
    ├─ 解析模式 → svg_parser.c → svg_render.c
    ├─ BMP导出 → svg_parser.c → bmp_writer.c
    ├─ JPG导出 → svg_parser.c → jpg_writer.c
    └─ GUI模式 → svg_parser.c → svg_gui.c
```

---

## 📂 项目文件结构

```
project/
├── include/              # 头文件（接口定义）
│   ├── svg_types.h      # 数据结构定义
│   ├── svg_parser.h     # 解析器接口
│   ├── svg_render.h     # 渲染器接口
│   ├── bmp_writer.h     # BMP导出接口
│   ├── jpg_writer.h     # JPG导出接口
│   └── svg_gui.h        # GUI接口
│
├── src/                 # 源代码（功能实现）
│   ├── main.c           # 主程序（命令行处理）
│   ├── svg_parser.c     # SVG解析器
│   ├── svg_render.c     # 控制台渲染
│   ├── bmp_writer.c     # BMP导出
│   ├── jpg_writer.c     # JPG导出
│   ├── svg_gui.c        # GUI编辑器
│   └── svg_writer.c     # SVG保存
│
└── assets/              # 测试文件
    ├── demo.svg
    ├── starry_night.svg
    └── mondrian.svg
```

---

## 🚀 运行流程详解

### 场景1：解析SVG文件

**命令：** `build\svg_processor.exe -p assets\demo.svg`

#### 步骤1：main.c 接收命令
```c
int main(int argc, char *argv[]) {
    // argc = 3
    // argv[0] = "build\svg_processor.exe"
    // argv[1] = "-p"
    // argv[2] = "assets\demo.svg"
    
    if (strcmp(argv[1], "--parse") == 0 || strcmp(argv[1], "-p") == 0) {
        // 进入解析模式
        SvgDocument *doc = NULL;
        svg_load_from_file(argv[2], &doc);  // 调用解析器
        svg_print_summary(doc);              // 打印摘要
        svg_print_shapes(doc);               // 打印图形
        svg_free_document(doc);              // 释放内存
    }
}
```

#### 步骤2：svg_parser.c 解析文件
```c
int svg_load_from_file(const char *filename, SvgDocument **doc_out) {
    // 1. 打开文件
    FILE *file = fopen(filename, "r");
    
    // 2. 创建文档结构
    SvgDocument *doc = malloc(sizeof(SvgDocument));
    doc->width = 800.0;
    doc->height = 600.0;
    doc->shapes = NULL;  // 链表头指针
    
    // 3. 逐行读取文件
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // 4. 检测图形类型
        if (strstr(line, "<circle")) {
            // 解析圆形
            SvgShape *shape = malloc(sizeof(SvgShape));
            shape->type = SVG_SHAPE_CIRCLE;
            
            // 提取属性：cx="100" cy="200" r="50"
            get_attribute(line, "cx", value, sizeof(value));
            shape->data.circle.cx = atof(value);
            
            // 添加到链表
            shape->next = NULL;
            if (!doc->shapes) {
                doc->shapes = shape;  // 第一个节点
            } else {
                last_shape->next = shape;  // 追加到末尾
            }
        }
        // 类似处理 <rect> 和 <line>
    }
    
    *doc_out = doc;  // 返回文档指针
    return 0;
}
```

#### 步骤3：svg_render.c 显示结果
```c
void svg_print_shapes(const SvgDocument *doc) {
    SvgShape *current = doc->shapes;  // 从链表头开始
    
    while (current) {  // 遍历链表
        switch (current->type) {
            case SVG_SHAPE_CIRCLE:
                printf("[%d] CIRCLE: cx=%.2f, cy=%.2f, r=%.2f\n",
                       current->id,
                       current->data.circle.cx,
                       current->data.circle.cy,
                       current->data.circle.r);
                break;
            // 类似处理矩形和线条
        }
        current = current->next;  // 移动到下一个节点
    }
}
```

**输出：**
```
SVG Document: width=800.00, height=600.00
Total shapes: 215
[1] RECT: x=0.00, y=0.00, width=800.00, height=450.00, fill=#87CEEB
[2] CIRCLE: cx=650.00, cy=100.00, r=60.00, fill=#FFD700
...
```

---

### 场景2：导出为JPG

**命令：** `build\svg_processor.exe -ej assets\demo.svg output.jpg 90`

#### 步骤1：main.c 识别导出命令
```c
if (strcmp(argv[1], "--export_jpg") == 0 || strcmp(argv[1], "-ej") == 0) {
    int quality = 90;  // 默认质量
    if (argc >= 5) {
        quality = atoi(argv[4]);  // 读取质量参数
    }
    
    // 1. 先解析SVG
    SvgDocument *doc = NULL;
    svg_load_from_file(argv[2], &doc);
    
    // 2. 导出为JPG
    export_to_jpg(argv[3], doc, quality);
    
    // 3. 释放内存
    svg_free_document(doc);
}
```

#### 步骤2：jpg_writer.c 渲染和导出
```c
int export_to_jpg(const char *filename, const SvgDocument *doc, int quality) {
    int width = (int)doc->width;   // 800
    int height = (int)doc->height; // 600
    
    // 1. 分配像素缓冲区（RGB格式）
    uint8_t *pixels = calloc(width * height * 3, sizeof(uint8_t));
    // pixels[0] = R, pixels[1] = G, pixels[2] = B, ...
    
    // 2. 填充白色背景
    memset(pixels, 255, width * height * 3);
    
    // 3. 遍历所有图形，绘制到像素缓冲区
    SvgShape *current = doc->shapes;
    while (current) {
        switch (current->type) {
            case SVG_SHAPE_CIRCLE:
                draw_circle(pixels, width, height, &current->data.circle);
                break;
            case SVG_SHAPE_RECT:
                draw_rect(pixels, width, height, &current->data.rect);
                break;
            case SVG_SHAPE_LINE:
                draw_line(pixels, width, height, &current->data.line);
                break;
        }
        current = current->next;
    }
    
    // 4. 使用stb_image_write库写入JPG文件
    int result = stbi_write_jpg(filename, width, height, 3, pixels, quality);
    
    free(pixels);
    return result ? 0 : -1;
}
```

#### 步骤3：绘制圆形的详细过程
```c
static void draw_circle(uint8_t *pixels, int width, int height, 
                       SvgCircle *circle) {
    int cx = (int)circle->cx;  // 圆心x = 650
    int cy = (int)circle->cy;  // 圆心y = 100
    int r = (int)circle->r;    // 半径 = 60
    
    // 解析颜色 "#FFD700" → RGB(255, 215, 0)
    uint32_t color = 0xFFD700;
    sscanf(circle->fill + 1, "%6x", &color);
    
    // 遍历圆的外接正方形
    for (int y = cy - r; y <= cy + r; y++) {      // 40 到 160
        for (int x = cx - r; x <= cx + r; x++) {  // 590 到 710
            int dx = x - cx;  // 距离圆心的x距离
            int dy = y - cy;  // 距离圆心的y距离
            
            // 判断点是否在圆内：dx² + dy² <= r²
            if (dx * dx + dy * dy <= r * r) {
                // 计算像素位置
                int index = (y * width + x) * 3;
                pixels[index + 0] = (color >> 16) & 0xFF;  // R
                pixels[index + 1] = (color >> 8) & 0xFF;   // G
                pixels[index + 2] = color & 0xFF;          // B
            }
        }
    }
}
```

**结果：** 生成 output.jpg 文件（19 KB）

---

### 场景3：GUI编辑器

**命令：** `build\svg_processor.exe -g assets\demo.svg`

#### 步骤1：main.c 启动GUI
```c
if (strcmp(argv[1], "--gui") == 0 || strcmp(argv[1], "-g") == 0) {
    // 1. 加载SVG文件（如果提供）
    SvgDocument *doc = NULL;
    if (argc >= 3) {
        svg_load_from_file(argv[2], &doc);
    }
    
    // 2. 启动GUI
    run_gui(doc);
}
```

#### 步骤2：svg_gui.c 初始化GUI
```c
void run_gui(SvgDocument* doc) {
    GUIState state;
    
    // 1. 初始化SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    // 2. 创建窗口
    state.window = SDL_CreateWindow("SVG Editor",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   1000, 800,
                                   SDL_WINDOW_SHOWN);
    
    // 3. 创建渲染器
    state.renderer = SDL_CreateRenderer(state.window, -1, 
                                       SDL_RENDERER_ACCELERATED);
    
    // 4. 设置初始状态
    state.document = doc;
    state.selected_shape = -1;
    state.dragging = 0;
    
    // 5. 主循环
    int running = 1;
    while (running) {
        gui_handle_events(&state, &running);  // 处理事件
        gui_render(&state);                   // 渲染画面
        SDL_Delay(16);                        // 60 FPS
    }
    
    // 6. 清理
    gui_cleanup(&state);
}
```

#### 步骤3：事件处理循环
```c
void gui_handle_events(GUIState* state, int* running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {  // 获取事件
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                // 鼠标按下
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mx = event.button.x;
                    int my = event.button.y;
                    
                    // 检查是否点击了图形
                    SvgShape* current = state->document->shapes;
                    int index = 0;
                    while (current) {
                        // 碰撞检测
                        if (current->type == SVG_SHAPE_CIRCLE) {
                            int dx = mx - current->data.circle.cx;
                            int dy = my - current->data.circle.cy;
                            if (dx*dx + dy*dy <= r*r) {
                                state->selected_shape = index;
                                state->dragging = 1;
                                break;
                            }
                        }
                        current = current->next;
                        index++;
                    }
                }
                break;
                
            case SDL_MOUSEMOTION:
                // 鼠标移动
                if (state->dragging) {
                    // 移动选中的图形
                    float dx = event.motion.xrel;
                    float dy = event.motion.yrel;
                    
                    // 找到选中的图形
                    SvgShape* shape = get_shape_at_index(state, 
                                                         state->selected_shape);
                    if (shape->type == SVG_SHAPE_CIRCLE) {
                        shape->data.circle.cx += dx;
                        shape->data.circle.cy += dy;
                    }
                }
                break;
                
            case SDL_KEYDOWN:
                // 键盘按下
                switch (event.key.keysym.sym) {
                    case SDLK_DELETE:
                        // 删除选中的图形
                        remove_shape(state, state->selected_shape);
                        break;
                    case SDLK_s:
                        // 保存文件
                        svg_save_to_file("output.svg", state->document);
                        break;
                    case SDLK_ESCAPE:
                        // 退出
                        *running = 0;
                        break;
                }
                break;
        }
    }
}
```

#### 步骤4：渲染画面
```c
void gui_render(GUIState* state) {
    // 1. 清空屏幕（灰色背景）
    SDL_SetRenderDrawColor(state->renderer, 240, 240, 240, 255);
    SDL_RenderClear(state->renderer);
    
    // 2. 绘制画布区域（白色）
    SDL_Rect canvas = {0, 0, 800, 600};
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(state->renderer, &canvas);
    
    // 3. 绘制所有图形
    SvgShape* current = state->document->shapes;
    int index = 0;
    while (current) {
        gui_draw_shape(state->renderer, current, 0, 0, 1.0);
        
        // 4. 如果是选中的图形，绘制红色边框
        if (index == state->selected_shape) {
            SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
            // 绘制选择框...
        }
        
        current = current->next;
        index++;
    }
    
    // 5. 绘制工具栏
    gui_draw_toolbar(state);
    
    // 6. 显示到屏幕
    SDL_RenderPresent(state->renderer);
}
```

---

## 🔑 核心数据结构详解

### 1. SvgShape（图形节点）
```c
typedef struct SvgShape {
    SvgShapeType type;  // 图形类型（圆/矩/线）
    
    union {             // Union：三种图形共享内存
        SvgCircle circle;   // 只有当type=CIRCLE时使用
        SvgRect rect;       // 只有当type=RECT时使用
        SvgLine line;       // 只有当type=LINE时使用
    } data;
    
    struct SvgShape *next;  // 指向下一个图形（链表）
} SvgShape;
```

**内存布局：**
```
SvgShape (假设32位系统):
├─ type: 4 bytes
├─ data: 24 bytes (Union，取最大的)
│   ├─ circle: cx(8) + cy(8) + r(8) = 24 bytes
│   ├─ rect: x(8) + y(8) + w(8) + h(8) = 32 bytes ← 最大
│   └─ line: x1(8) + y1(8) + x2(8) + y2(8) = 32 bytes
└─ next: 8 bytes (指针)

总大小: 4 + 32 + 8 = 44 bytes
```

**如果不用Union：**
```c
struct SvgShape {
    SvgShapeType type;
    SvgCircle circle;  // 32 bytes
    SvgRect rect;      // 32 bytes
    SvgLine line;      // 32 bytes
    struct SvgShape *next;
};
// 总大小: 4 + 32 + 32 + 32 + 8 = 108 bytes
// Union节省了: 108 - 44 = 64 bytes (59%)
```

### 2. 链表操作

**添加图形：**
```c
// 创建新图形
SvgShape *new_shape = malloc(sizeof(SvgShape));
new_shape->type = SVG_SHAPE_CIRCLE;
new_shape->data.circle.cx = 100;
new_shape->data.circle.cy = 200;
new_shape->data.circle.r = 50;
new_shape->next = NULL;

// 添加到链表末尾
if (!doc->shapes) {
    doc->shapes = new_shape;  // 第一个节点
} else {
    SvgShape *current = doc->shapes;
    while (current->next) {
        current = current->next;  // 找到最后一个节点
    }
    current->next = new_shape;  // 追加
}
```

**删除图形：**
```c
// 删除索引为index的图形
SvgShape *current = doc->shapes;
SvgShape *prev = NULL;
int i = 0;

while (current && i < index) {
    prev = current;
    current = current->next;
    i++;
}

if (current) {
    if (prev) {
        prev->next = current->next;  // 跳过当前节点
    } else {
        doc->shapes = current->next;  // 删除头节点
    }
    free(current);  // 释放内存
}
```

---

## 🎨 图形算法详解

### 1. 圆形绘制（中点圆算法简化版）
```c
// 原理：遍历外接正方形，判断每个点是否在圆内
for (int y = cy - r; y <= cy + r; y++) {
    for (int x = cx - r; x <= cx + r; x++) {
        // 计算点到圆心的距离
        int dx = x - cx;
        int dy = y - cy;
        int distance_squared = dx * dx + dy * dy;
        
        // 如果距离 <= 半径，则在圆内
        if (distance_squared <= r * r) {
            draw_pixel(x, y, color);
        }
    }
}
```

**示例：** 圆心(100, 100)，半径50
```
遍历范围：x: 50-150, y: 50-150
点(100, 100): dx=0, dy=0, d²=0 <= 2500 ✓ 在圆内
点(150, 100): dx=50, dy=0, d²=2500 <= 2500 ✓ 在圆边上
点(151, 100): dx=51, dy=0, d²=2601 > 2500 ✗ 在圆外
```

### 2. 线条绘制（Bresenham算法）
```c
// 原理：用整数运算逐步逼近直线
int dx = abs(x2 - x1);
int dy = abs(y2 - y1);
int sx = x1 < x2 ? 1 : -1;  // x方向步进
int sy = y1 < y2 ? 1 : -1;  // y方向步进
int err = dx - dy;

while (1) {
    draw_pixel(x1, y1, color);
    
    if (x1 == x2 && y1 == y2) break;
    
    int e2 = 2 * err;
    if (e2 > -dy) {
        err -= dy;
        x1 += sx;  // x步进
    }
    if (e2 < dx) {
        err += dx;
        y1 += sy;  // y步进
    }
}
```

---

## 💾 内存管理

### 分配内存
```c
// 1. 创建文档
SvgDocument *doc = malloc(sizeof(SvgDocument));

// 2. 创建图形
SvgShape *shape = malloc(sizeof(SvgShape));

// 3. 复制字符串
shape->data.circle.fill = strdup("#FF0000");  // 分配新内存并复制
```

### 释放内存
```c
void svg_free_document(SvgDocument *doc) {
    SvgShape *current = doc->shapes;
    
    while (current) {
        SvgShape *next = current->next;
        
        // 释放字符串
        if (current->type == SVG_SHAPE_CIRCLE) {
            if (current->data.circle.fill) {
                free(current->data.circle.fill);
            }
        }
        // 类似处理其他类型...
        
        free(current);  // 释放节点
        current = next;
    }
    
    free(doc);  // 释放文档
}
```

**重要：** 每个malloc都要有对应的free，否则会内存泄漏！

---

## 🔄 完整运行流程图

```
用户输入: svg_processor.exe -ej demo.svg output.jpg 90
    ↓
main.c: 解析命令行参数
    ↓
识别为JPG导出模式
    ↓
调用 svg_load_from_file("demo.svg", &doc)
    ↓
svg_parser.c: 打开文件
    ↓
逐行读取，识别 <circle>, <rect>, <line>
    ↓
提取属性: cx, cy, r, fill, etc.
    ↓
创建 SvgShape 节点，添加到链表
    ↓
返回 SvgDocument 指针
    ↓
调用 export_to_jpg("output.jpg", doc, 90)
    ↓
jpg_writer.c: 分配像素缓冲区 (800x600x3)
    ↓
填充白色背景
    ↓
遍历链表，绘制每个图形:
    ├─ 圆形: 遍历外接正方形，判断距离
    ├─ 矩形: 填充矩形区域
    └─ 线条: Bresenham算法
    ↓
调用 stbi_write_jpg() 写入文件
    ↓
释放像素缓冲区
    ↓
调用 svg_free_document(doc)
    ↓
释放所有图形节点和字符串
    ↓
程序结束
```

---

## 🎯 关键要点总结

1. **模块化设计**：每个功能独立成模块，通过头文件定义接口
2. **数据结构**：Union节省内存，链表支持动态操作
3. **解析流程**：逐行读取 → 识别标签 → 提取属性 → 构建链表
4. **渲染流程**：遍历链表 → 根据类型绘制 → 写入像素缓冲区
5. **内存管理**：malloc/free配对，避免泄漏
6. **GUI循环**：事件处理 → 更新状态 → 渲染画面 → 重复

---

**这就是整个项目的运行逻辑！** 🎉

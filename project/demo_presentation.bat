@echo off
echo ========================================
echo SVG处理系统 - 现场演示
echo ========================================
echo.

echo [1/3] 解析SVG文件
echo ----------------------------------------
build\svg_processor.exe -p assets\demo.svg
echo.
pause

echo.
echo [2/3] 导出JPG（质量90）
echo ----------------------------------------
build\svg_processor.exe -ej assets\demo.svg demo_live.jpg 90
if exist demo_live.jpg (
    echo 成功生成：demo_live.jpg
) else (
    echo 生成失败
)
echo.
pause

echo.
echo [3/3] 启动GUI编辑器
echo ----------------------------------------
echo GUI操作演示：
echo   - 点击选择图形
echo   - 拖拽移动
echo   - 工具栏添加图形
echo   - DELETE删除
echo   - S保存
echo   - ESC退出
echo.
echo 按任意键启动GUI...
pause >nul

build\svg_processor.exe -g assets\demo.svg

echo.
echo ========================================
echo 演示完成！
echo ========================================
pause

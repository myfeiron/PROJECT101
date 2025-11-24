@echo off
echo ========================================
echo SVG Processor - Quick Demo
echo ========================================
echo.

echo This demo will:
echo 1. Parse the demo SVG file
echo 2. Export it to BMP
echo 3. Launch the GUI editor
echo.
pause

echo.
echo [1/3] Parsing demo.svg...
echo ----------------------------------------
build\svg_processor.exe --parse assets\demo.svg
echo.
pause

echo.
echo [2/3] Exporting to BMP...
echo ----------------------------------------
build\svg_processor.exe --export_bmp assets\demo.svg demo_output.bmp
if exist demo_output.bmp (
    echo Success! File created: demo_output.bmp
) else (
    echo Failed to create BMP file
)
echo.
pause

echo.
echo [3/3] Launching GUI Editor...
echo ----------------------------------------
echo Controls:
echo   - Click to select and drag shapes
echo   - Toolbar buttons to add shapes
echo   - DELETE: Remove shape
echo   - S: Save to SVG
echo   - T: Toggle toolbar
echo   - ESC: Exit
echo.
echo Press any key to launch...
pause >nul

build\svg_processor.exe --gui assets\demo.svg

echo.
echo Demo completed!
pause

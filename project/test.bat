@echo off
echo ========================================
echo SVG Processor Test Suite
echo ========================================
echo.

echo Test 1: Parse SVG file
build\svg_processor.exe --parse assets\test.svg
echo.

echo Test 2: Export to BMP
build\svg_processor.exe --export_bmp assets\test.svg test_output.bmp
if exist test_output.bmp (
    echo BMP export successful!
) else (
    echo BMP export failed!
)
echo.

echo Test 3: GUI mode (will open window - press ESC to close)
echo Press any key to launch GUI...
pause >nul
build\svg_processor.exe --gui assets\test.svg

echo.
echo All tests completed!

REM ========DIRS=====================

set DATA_DIR=%cd%
set TOOL_DIR=%DATA_DIR%\..\tools\
set SPRITE_DIR=%DATA_DIR%\sprites\
set DEST_RES_DIR=%DATA_DIR%\..\build\res\
set DEF_DIR=%DATA_DIR%\..\test_core\flt_test\auto_generated\


REM ========Tools====================

if "%PYTHON%" == "" (
	set PYTHON="E:\sdks\Python25\python"
)

if "%COMPUTERNAME%"=="Second dev computer name" (
	set PYTHON="C:\Python25\python"
)

set XKIT=%TOOL_DIR%\XKit.exe
set UIDEFS=%TOOL_DIR%\UIDefs.py
set UITOOL=%TOOL_DIR%\UITool.exe



REM ========DIRS=====================

set DATA_DIR=%cd%
set TOOL_DIR=%DATA_DIR%\..\tools\
set DATA_SPRITE_DIR=%DATA_DIR%\sprites\
set DATA_UI_DIR=%DATA_DIR%\ui\
set DATA_LEVEL_DIR=%DATA_DIR%\levels\
set DEST_RES_DIR=%DATA_DIR%\..\build\res\
set DEF_DIR=%DATA_DIR%\..\src\auto_generated\


REM ========Tools====================

if "%PYTHON%" == "" (
	set PYTHON="E:\sdks\Python25\python"
)

if "%COMPUTERNAME%"=="Second dev computer name" (
	set PYTHON="C:\Python25\python"
)

set XKIT=%TOOL_DIR%\XKit.exe
set UITOOL=%TOOL_DIR%\UITool.exe
set UIDEFS=%TOOL_DIR%\UIDefs.py



call config.bat

@echo =================================
@echo ==     make UI                 ==
@echo =================================

pushd %DATA_UI_DIR%

@echo clean UI data dir
del *.h

@echo make UI def fines

rem for %%i in ( *.xml ) do (
rem  python %UIDEFS% -f "%%i" "ui_defs.h"
rem )

python %UIDEFS% -d "." "ui_defs.h"

copy *.xml %DEST_RES_DIR%
move *.h %DEF_DIR%

popd

pause
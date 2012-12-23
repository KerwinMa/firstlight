call config.bat

@echo =================================
@echo ==     make UI                 ==
@echo =================================

pushd %DEST_RES_DIR%

python %UIDEFS% -f "testLayer.xml" "ui_defs.h"

rem %UITOOL% "testLayer.xml" "ui_defs.h" %DEST_RES_DIR%

move *.h %DEF_DIR%

popd

pause
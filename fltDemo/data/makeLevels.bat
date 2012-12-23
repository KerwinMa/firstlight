call config.bat

@echo =================================
@echo ==     make Level              ==
@echo =================================

pushd %DATA_LEVEL_DIR%

copy *.xml %DEST_RES_DIR%

popd

pause
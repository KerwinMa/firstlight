rem @echo off

call config.bat

@echo =================================
@echo ==     Convert Sprites         ==
@echo =================================

pushd %SPRITE_DIR%

@echo clean
del *.xsp
del *.h

@echo convert xanim to xsp
for %%i in ( *.xanim ) do (
 %XKIT% %%i c
)

move *.xsp %DEST_RES_DIR%
move *.h %DEF_DIR%

popd

pause
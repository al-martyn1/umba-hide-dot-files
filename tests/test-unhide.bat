@if exist "%~dp0\_setup_tests.bat" @call "%~dp0\_setup_tests.bat"
@echo OUTROOTPATH=%OUTROOTPATH%
@echo SLN_EXE=%SLN_EXE%
set FOLDER=%~dp0..
%SLN_EXE% --unhide "%FOLDER%" --open "%FOLDER%"
attrib /D "%FOLDER%\*.*" >test-unhide.log

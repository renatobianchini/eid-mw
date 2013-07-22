@echo [INFO] Define default value for BEID_DIR_XERCES_310 if not defined yet
@echo [INFO] Input BEID_DIR_XERCES_310=%BEID_DIR_XERCES_310%
@set FILE_TO_FIND="bin\xerces-c_3_1.dll" "lib\xerces-c_3.lib"
@echo [INFO] Looking for files: %FILE_TO_FIND%

@set FILE_NOT_FOUND=
@for %%i in (%FILE_TO_FIND%) do @if not exist "%BEID_DIR_XERCES_310%\%%~i" set FILE_NOT_FOUND=%%~i
@if "%FILE_NOT_FOUND%"=="" goto find_xerces_310
@echo        Not found in "%BEID_DIR_XERCES_310%"

@set BEID_DIR_XERCES_310=%~dp0..\ThirdParty\Xerces\Xerces-3.1.0
@set FILE_NOT_FOUND=
@for %%i in (%FILE_TO_FIND%) do @if not exist "%BEID_DIR_XERCES_310%\%%~i" set FILE_NOT_FOUND=%%~i
@if "%FILE_NOT_FOUND%"=="" goto find_xerces_310
@echo        Not found in "%BEID_DIR_XERCES_310%"

@echo [ERROR] Xerces 3.1.0 could not be found
@echo         If the path is different from "~dp0..\ThirdParty\Xerces\Xerces-3.1.0"
@echo         please define BEID_DIR_XERCES_310 environment variable.
@exit /B 1

:find_xerces_310
@echo        Found in "%BEID_DIR_XERCES_310%"
@exit /B 0
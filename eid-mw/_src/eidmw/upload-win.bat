echo %0 %1 %2

if %1%=="" goto PARAM_ERR_1
if %2%=="" goto PARAM_ERR_2

set MSI_TYPE=%1
set BUILD_NR=%2

if %MSI_TYPE%==is     goto %MSI_TYPE%
if %MSI_TYPE%==is-sdk goto %MSI_TYPE%

if %MSI_TYPE%==is35     goto %MSI_TYPE%
if %MSI_TYPE%==is35-pro goto %MSI_TYPE%
if %MSI_TYPE%==is35-sdk goto %MSI_TYPE%

if %MSI_TYPE%==msi35     goto %MSI_TYPE%
if %MSI_TYPE%==msi35-64  goto %MSI_TYPE%
if %MSI_TYPE%==msi35-pro goto %MSI_TYPE%
if %MSI_TYPE%==msi35-sdk goto %MSI_TYPE%
if %MSI_TYPE%==msi35-pro-sdk goto %MSI_TYPE%

if %MSI_TYPE%==msi35BWC goto %MSI_TYPE%
if %MSI_TYPE%==msi35BWC-64 goto %MSI_TYPE%

if %MSI_TYPE%==msi40     goto %MSI_TYPE%
if %MSI_TYPE%==msi40-64  goto %MSI_TYPE%

if %MSI_TYPE%==qi     goto %MSI_TYPE%

if %MSI_TYPE%==dt35     goto %MSI_TYPE%

if %MSI_TYPE%==cleanup  goto %MSI_TYPE%

goto PARAM_ERR_2

:is
set   SRC=%~dp0misc\setup_win\_output\runtime_single_exe\setup-beid-runtime-3.0.exe
set LOCAL=%~dp0misc\setup_win\_output\runtime_single_exe\setup-beid-runtime-3.0-%BUILD_NR%.exe
goto UPLOAD

:is-sdk
set   SRC=%~dp0misc\setup_win\_output\sdk_single_exe\setup-beid-sdk-3.0.exe
set LOCAL=%~dp0misc\setup_win\_output\sdk_single_exe\setup-beid-sdk-3.0-%BUILD_NR%.exe
goto UPLOAD

:is35
set   SRC=%~dp0misc\setup_win\_output\runtime_single_exe\setup-beid-runtime-3.5.exe
set LOCAL=%~dp0misc\setup_win\_output\runtime_single_exe\setup-beid-runtime-3.5-%BUILD_NR%.exe
goto UPLOAD

:is35-pro
set   SRC=%~dp0misc\setup_win\_output\runtime_pro_single_exe\setup-beid-runtime-pro-3.5.exe
set LOCAL=%~dp0misc\setup_win\_output\runtime_pro_single_exe\setup-beid-runtime-pro-3.5-%BUILD_NR%.exe
goto UPLOAD

:is35-sdk
set   SRC=%~dp0misc\setup_win\_output\sdk_single_exe\setup-beid-sdk-3.5.exe
set LOCAL=%~dp0misc\setup_win\_output\sdk_single_exe\setup-beid-sdk-3.5-%BUILD_NR%.exe
goto UPLOAD

:msi35
set   SRC=%~dp0misc\Wix_MW35\MW35Wix\bin\Release\BeidMW35.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_runtime\BeidMW35-%BUILD_NR%.msi
goto UPLOAD

:msi35-64
set   SRC=%~dp0misc\Wix_MW35\MW35Wix\bin\x64\Release\BeidMW35-64.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_runtime\BeidMW35-64-%BUILD_NR%.msi
goto UPLOAD

:msi35-pro
set   SRC=%~dp0misc\Wix_MW35\MW35Wix\bin\Release\BeidMW35-Pro.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_runtime_pro\BeidMW35-Pro-%BUILD_NR%.msi
goto UPLOAD

:msi40
set   SRC=%~dp0misc\Wix_MW35\MW35Wix\bin\Release\BeidMW40.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_runtime\BeidMW40-%BUILD_NR%.msi
goto UPLOAD

:msi40-64
set   SRC=%~dp0misc\Wix_MW35\MW35Wix\bin\x64\Release\BeidMW40-64.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_runtime\BeidMW40-64-%BUILD_NR%.msi
goto UPLOAD

:msi35BWC
set   SRC=%~dp0misc\Wix_MW35\MW35Wix\bin\Release\BeidMW35BWC.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_runtime\BeidMW35BWC-%BUILD_NR%.msi
goto UPLOAD

:msi35BWC-64
set   SRC=%~dp0misc\Wix_MW35\MW35Wix\bin\x64\Release\BeidMW35BWC-64.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_runtime\BeidMW35BWC-64-%BUILD_NR%.msi
goto UPLOAD

:msi35-sdk
set   SRC=%~dp0misc\Wix_MW35\MW35Wix-Sdk\bin\Release\BeidMW35-Sdk.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_sdk\BeidMW35-Sdk-%BUILD_NR%.msi
goto UPLOAD

:msi35-pro-sdk
set   SRC=%~dp0misc\Wix_MW35\MW35Wix-Sdk\bin\Release\BeidMW35-Pro-Sdk.msi
set LOCAL=%~dp0misc\setup_win\_output\msi_sdk_pro\BeidMW35-Pro-Sdk-%BUILD_NR%.msi
goto UPLOAD

:qi
set   SRC=%~dp0..\eID-Quickinstaller\release\eID-QuickInstaller.exe
set LOCAL=%~dp0misc\setup_win\_output\qi_runtime\eID-QuickInstaller-build-%BUILD_NR%.exe
goto UPLOAD

:dt35
set   SRC=%~dp0..\eDT\release\eDTGui.exe
set LOCAL=%~dp0misc\setup_win\_output\eDT\eDTGui-build-%BUILD_NR%.exe
goto UPLOAD

:cleanup
set   SRC=%~dp0misc\Wix_MW35\beidcleanup\Release\beidcleanup.exe
set LOCAL=%~dp0misc\setup_win\_output\cleanup\beidcleanup-%BUILD_NR%.exe
goto UPLOAD

:UPLOAD
if not exist "%SRC%" goto FILE_NOT_EXIST
copy /Y "%SRC%" "%LOCAL%"

if not exist "%~dp0..\..\_src" goto END
if not exist "%~dp0..\..\windows" goto END
copy /Y "%LOCAL%" "%~dp0..\..\windows\"
goto END


:PARAM_ERR_2
echo [ERROR] Error parameter 1 (MSI_TYPE)
exit 1
goto END

:PARAM_ERR_1
echo [ERROR] Error parameter 1 (VERSION)
exit 1
goto END

:FILE_NOT_EXIST
echo [ERROR] File does not exist: %SRC%
exit 1
goto END

:END
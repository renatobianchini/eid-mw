:: Required: 7zip installed

set thecurrentdir=%cd%

cd %~dp0 

call .\SetPath7zip.bat

set x=belgiumeid
svn export ./src ./build

cd %~dp0\build
"%BEID_DIR_7ZIP%\7z" a -tzip "%x%.xpi" * -r -mx=9
cd %~dp0 

move build\%x%.xpi %x%.xpi

rmdir /s /q build

cd %thecurrentdir%

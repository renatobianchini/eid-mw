::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: eID Middleware Project.
:: Copyright (C) 2008-2010 FedICT.
::
:: This is free software; you can redistribute it and/or modify it
:: under the terms of the GNU Lesser General Public License version
:: 3.0 as published by the Free Software Foundation.
::
:: This software is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
:: Lesser General Public License for more details.
::
:: You should have received a copy of the GNU Lesser General Public
:: License along with this software; if not, see
:: http://www.gnu.org/licenses/.
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: use this script to download and run the windows build environment script 
:: build_env.ps1  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
mkdir c:\tmp
:: download build_env.ps1 script
powershell.exe -NonInteractive -ExecutionPolicy unrestricted -command "Import-Module BitsTransfer; Start-BitsTransfer -Source http://eid-mw.googlecode.com/svn-history/trunk/dev_env/windows/scripts/test_env.ps1 -Destination c:\tmp\test_env.ps1"
:: run build_env.ps1
powershell.exe -NonInteractive -ExecutionPolicy unrestricted -File c:\tmp\test_env.ps1
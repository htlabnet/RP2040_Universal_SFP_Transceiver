
@echo off
cd /d %~dp0

echo ========================================
echo    .pio.h Generator for Arduino IDE
echo ========================================

if not exist "%LOCALAPPDATA%\Arduino15\packages\rp2040\tools\pqt-pioasm\" (
  echo 'Raspberry Pi Pico Arduino core' does not exist^!
  pause
  exit
)

for /f %%i in ('dir /b /o-d %LOCALAPPDATA%\Arduino15\packages\rp2040\tools\pqt-pioasm\') do (
  if exist "%~dp0src\sfp\sfp_transmitter.pio.h" (
    echo File Deleted : ".\src\sfp\sfp_transmitter.pio.h"
    del /q "%~dp0src\sfp\sfp_transmitter.pio.h"
  )
  if exist "%~dp0src\sfp\sfp_receiver.pio.h" (
    echo File Deleted : ".\src\sfp\sfp_receiver.pio.h"
    del /q "%~dp0src\sfp\sfp_receiver.pio.h"
  )
  echo PIOASM Version : %%i
  echo PIOASM Path : "%LOCALAPPDATA%\Arduino15\packages\rp2040\tools\pqt-pioasm\%%i\pioasm.exe"
  "%LOCALAPPDATA%\Arduino15\packages\rp2040\tools\pqt-pioasm\%%i\pioasm.exe" "%~dp0src\sfp\sfp_transmitter.pio" "%~dp0src\sfp\sfp_transmitter.pio.h"
  "%LOCALAPPDATA%\Arduino15\packages\rp2040\tools\pqt-pioasm\%%i\pioasm.exe" "%~dp0src\sfp\sfp_receiver.pio" "%~dp0src\sfp\sfp_receiver.pio.h"
  goto break
)
:break

if exist "%~dp0src\sfp\sfp_transmitter.pio.h" (
  echo File Generated : ".\src\sfp\sfp_transmitter.pio.h"
)
if exist "%~dp0src\sfp\sfp_receiver.pio.h" (
  echo File Generated : ".\src\sfp\sfp_receiver.pio.h"
)

echo Done.

pause
exit

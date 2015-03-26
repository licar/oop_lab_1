REM Простой запуск программы
start rle.exe pack input.txt output.txt
IF NOT ERRORLEVEL 0 GOTO err
ECHO Program testing succeeded
ping -n 6 localhost>Nul
EXIT
:err
ECHO Program testing failed
ping -n 6 localhost>Nul
EXIT
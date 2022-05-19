set TEST=ModifiedSource
xCopy ..\Source .\%TEST%\ /E
for /f "tokens=*" %%f in ('dir /b .\%TEST%\*.h')   do unifdef.exe  -m -f definitions.def ./%TEST%/%%f
for /f "tokens=*" %%f in ('dir /b .\%TEST%\*.cpp') do unifdef.exe  -m -f definitions.def  ./%TEST%/%%f
pause
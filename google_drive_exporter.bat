@echo off
setlocal enabledelayedexpansion

:: Loop through numbers from 0 to 49
for /L %%i in (0, 1, 49) do (
    echo Running for Team %%i
    call run_all_helper_scripts.bat %%i

    :: Create a folder named #\data for each team
    set "teamFolder=upload\%%i\data"
    mkdir "!teamFolder!"

    :: Copy files from \data to #\data
    copy Main\data\* "!teamFolder!"
)

echo Script completed.
pause

:: Remove keys.txt and hash.txt from all subfolders
powershell -Command "Get-ChildItem -Recurse -Include keys.txt, hash.txt | ForEach-Object { Remove-Item $_.FullName -Force }"

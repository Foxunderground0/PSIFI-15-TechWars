@echo off
setlocal enabledelayedexpansion

set "teamId=0"

if not "%1"=="" set "teamId=%1"

:: Clear Main\data\* and Main\Main.ino\*
del /Q "Main\data\*" 2>nul
del /Q "Main\Main.ino\*" 2>nul

:: Recreate the directories
mkdir "Main\data"

:: Define paths to Python scripts with double quotes
set "assemblerScript=.\Web\assembler.py"
set "gameCompressorScript=.\Main\Helper Scripts\game_compressor.py"
set "loginCompressorScript=.\Main\Helper Scripts\login_compressor.py"
set "keyFileGeneratorScript=.\Main\Helper Scripts\key_file_generator.py"
set "copyChallengeScriptScript=.\Main\Helper Scripts\copy_challenge_script.py"
set "fillMainTemplateScript=.\Main\Helper Scripts\fill_main_template.py"
set "generateListOfDataFilenamesScript=.\Main\Helper Scripts\generate_list_of_data_filenames.py"
set "generateMD5Script=.\Main\Helper Scripts\md5_generator.py"

:: Run Python scripts with the provided teamId argument
py -3 "!assemblerScript!"
py -3 "!gameCompressorScript!"
py -3 "!loginCompressorScript!"
py -3 "!copyChallengeScriptScript!" !teamId!
py -3 "!generateMD5Script!" !teamId!
py -3 "!keyFileGeneratorScript!" !teamId!
py -3 "!fillMainTemplateScript!" !teamId!
py -3 "!generateListOfDataFilenamesScript!"

endlocal

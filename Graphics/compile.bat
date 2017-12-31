@ECHO OFF
SET DEBUG_FOLDER=%~dp0..\Source\Debug\Resources
SET RELEASE_FOLDER=%~dp0..\Source\Release\Resources
MKDIR "%DEBUG_FOLDER%"
MKDIR "%RELEASE_FOLDER%"

FOR /f %%i IN ('npm root --quiet -g') DO SET NODE_PATH=%%i
ECHO %NODE_PATH%
node "%~dp0\compile.js"
magick convert "%~dp0\textures.png" "%DEBUG_FOLDER%\textures.tga"
COPY "%DEBUG_FOLDER%\textures.tga" "%RELEASE_FOLDER%\textures.tga"
MOVE "%~dp0\font.bin" "%DEBUG_FOLDER%\font.bin"
COPY "%DEBUG_FOLDER%\font.bin" "%RELEASE_FOLDER%\font.bin"
DEL "%~dp0\textures.png"

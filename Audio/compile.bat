@ECHO OFF
SETLOCAL EnableDelayedExpansion
SET DEBUG_FOLDER=%~dp0..\Source\Debug\Resources
SET RELEASE_FOLDER=%~dp0..\Source\Release\Resources
MKDIR "%DEBUG_FOLDER%"
MKDIR "%RELEASE_FOLDER%"

SET /A COUNTER=0
SET SOUNDS=click defeat hover mark reveal victory unmark

ffmpeg -y -i effects_Seq04.wav -af areverse effects_Seq07.wav
(FOR %%i IN (%SOUNDS%) DO (
	SET /A COUNTER+=1
	SET FILE=effects_Seq0!COUNTER!.wav
	ffmpeg -y -i "%~dp0\!FILE!"  -acodec libvorbis "%~dp0\%%i.ogg"
	MOVE "%~dp0\%%i.ogg" "%DEBUG_FOLDER%\%%i.ogg"
	COPY "%DEBUG_FOLDER%\%%i.ogg" "%RELEASE_FOLDER%\%%i.ogg"
	DEL "%~dp0\!FILE!"
))

echo Set enviroment variable ORB_MP_PATH to current path

setx ORB_MP_PATH "%cd%"

if /I "%ERRORLEVEL%" EQU "0" (
	set ORB_MP_PATH
	echo.
) else (
	echo Error: ORB_MP_PATH not set.
)
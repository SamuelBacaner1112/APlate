call ../../../Build/BuildBaseV100.bat ../APlate APlate.vcxproj Clean Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/APlateData/APlateData.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/PlateGangje/PlateGangje.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/APlateDBDraw/APlateDBDraw.vcxproj  // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/APlateCalc/APlateCalc.vcxproj  // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/GeneralInput/GeneralInput.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/APlateDraw/APlateDraw.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/APlateOutCalc/APlateOutCalc.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/APlateUI/APlateUI.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../APlate ../Src/APlateOutBM/APlateOutBM.vcxproj // Release
call ../../../Build/BuildBaseV100.bat ../APlate APlate.vcxproj // Release

REM code sign ////////////////////////////////////////////////
rem target directory 보관
SET TARGETDIR=%cd%/../CommonCompile/Output_v100

rem 코드사인 폴더로 이동
cd ../../../ToolInhouse/CodeSignTool/CertificateFile_pfx

rem 코드사인 실행
call codesign_all %TARGETDIR%
REM code sign end //////////////////////////////////////////////////

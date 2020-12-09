set Dir_Output=..\CommonCompile\Output_v100\

echo === Lib Copy ===
copy ..\..\..\Lib\Sx32w.lib %Dir_Output% > %Dir_Output%out
copy ..\..\..\Lib\ShImageMng\CommonCompile\Output_v100\ShImageMng.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Lib\XTPToolkitPro\CommonCompile\Output_v100\ToolkitPro1624vc100.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Lib\XTPExt\CommonCompile\Output_v100\XTPExtvc100.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Lib\iconv\dll_win32\libiconv.dll %Dir_Output% > %Dir_Output%out

echo === Dll Copy ===
copy ..\..\..\Dll\libintl3.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Dll\libiconv2.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Dll\GLU32.DLL %Dir_Output% > %Dir_Output%out
copy ..\..\..\Dll\opengl32.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Dll\ASolverDLL\*.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Dll\Sx32w.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Dll\Sx32w_6_5.dll %Dir_Output% > %Dir_Output%out

echo === AEdit Copy ===
copy ..\..\..\Tool\HGEdit\CommonCompile\Output_v100\AEdit.exe %Dir_Output% > %Dir_Output%out

echo === ALine Copy ===
copy ..\..\..\Engine\BaseEng\CommonCompile\Output_v100\ALine.exe %Dir_Output% > %Dir_Output%out

echo === BaseEng dll Copy ===
copy ..\..\..\Engine\BaseEng\CommonCompile\Output_v100\*.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Engine\BaseEng\i18n\Language.xml %Dir_Output% > %Dir_Output%out

echo === BaseEng etc Copy ===
copy ..\..\..\Engine\BaseEng\Src\HgBaseDesign\AExpJoint.dll %Dir_Output% > %Dir_Output%out
copy ..\..\..\Engine\BaseEng\Src\BearingInfo\BearingDB.hdb %Dir_Output% > %Dir_Output%out
copy ..\..\..\Engine\BaseEng\Src\HgBaseDesign\HGSTEELDB.mdb %Dir_Output% > %Dir_Output%out
copy ..\..\..\Engine\BaseEng\CommonCompile\Output_v100\ABorder.exe %Dir_Output% > %Dir_Output%out
IF NOT EXIST "%Dir_Output%BlockDomyun" MD "%Dir_Output%BlockDomyun"
copy ..\..\..\Engine\BaseEng\Data\Dxf\HCAD_Symbol.dxf %Dir_Output%BlockDomyun\HCAD_Symbol.dxf > %Dir_Output%out

echo === *.axl Copy & *dll Paste ===
copy ..\Data\*.axl %Dir_Output%*.dll > %Dir_Output%out
copy ..\Data\BlockDomyun\*.* %Dir_Output%BlockDomyun > %Dir_Output%out

echo == *.pdf Copy ===
copy *.pdf %Dir_Output% > %Dir_Output%out


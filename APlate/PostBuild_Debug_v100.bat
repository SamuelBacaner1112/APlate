set Dir_Output=..\CommonCompile\OutputD_v100\

echo === Lib Copy ===
copy ..\..\..\Lib\Sx32w.lib %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Lib\ShImageMng\CommonCompile\OutputD_v100\ShImageMngD.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Lib\XTPToolkitPro\CommonCompile\OutputD_v100\ToolkitPro1624vc100D.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Lib\XTPExt\CommonCompile\OutputD_v100\XTPExtvc100D.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Lib\iconv\dll_win32\libiconvD.dll %Dir_Output% > %Dir_Output%outD

echo === Dll Copy ===
copy ..\..\..\Dll\libintl3.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Dll\libiconv2.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Dll\GLU32.DLL %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Dll\opengl32.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Dll\ASolverDLL\*.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Dll\Sx32w_6.5.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Dll\Sx32w.dll %Dir_Output% > %Dir_Output%outD

echo === AEdit Copy ===
copy ..\..\..\Tool\HGEdit\CommonCompile\OutputD_v100\AEdit.exe %Dir_Output% > %Dir_Output%outD

echo === ALine Copy ===
copy ..\..\..\Engine\BaseEng\CommonCompile\OutputD_v100\ALine.exe %Dir_Output% > %Dir_Output%outD

echo === BaseEng dll Copy ===
copy ..\..\..\Engine\BaseEng\CommonCompile\OutputD_v100\*.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Engine\BaseEng\i18n\Language.xml %Dir_Output% > %Dir_Output%outD

echo === BaseEng etc Copy ===
copy ..\..\..\Engine\BaseEng\Src\HgBaseDesign\AExpJoint.dll %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Engine\BaseEng\Src\BearingInfo\BearingDB.hdb %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Engine\BaseEng\Src\HgBaseDesign\HGSTEELDB.mdb %Dir_Output% > %Dir_Output%outD
copy ..\..\..\Engine\BaseEng\CommonCompile\OutputD_v100\ABorder.exe %Dir_Output% > %Dir_Output%outD
IF NOT EXIST "%Dir_Output%BlockDomyun" MD "%Dir_Output%BlockDomyun"
copy ..\..\..\Engine\BaseEng\Data\Dxf\HCAD_Symbol.dxf %Dir_Output%BlockDomyun\HCAD_Symbol.dxf > %Dir_Output%outD

echo === *.axl Copy & *dll Paste ===
copy ..\Data\*.axl %Dir_Output%*.dll > %Dir_Output%outD
copy ..\Data\BlockDomyun\*.* %Dir_Output%BlockDomyun > %Dir_Output%outD

echo == *.pdf Copy ===
copy *.pdf %Dir_Output% > %Dir_Output%outD


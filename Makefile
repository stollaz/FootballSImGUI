# Attempting to make a Makefile for Walnut

exe = WalnutApp

version =
# version = -std=c+11
# version = -std=c+20

# includes = ..\vendor\imgui ..\vendor\glfw\include ..\Walnut\src C:\VulkanSDK\1.3.211.0\Include ..\vendor\glm %(AdditionalIncludeDirectories)
include = -Ivendor/imgui -Ivendor/glfw/include -IWalnut/src -IC:/VulkanSDK/1.3.211.0/Include -Ivendor/glm

optim = -Ofast

default: all

all:
	@echo Building...
	g++ $(version) $(optim) $(include) WalnutApp/src/$(exe).cpp -o $(exe).exe
	@echo Executing...
	@$(exe).exe
build:
	@echo Building...
	g++ $(version) $(optim) $(include) WalnutApp/src/$(exe).cpp -o $(exe).exe
execute:
	@echo Executing...
	@$(exe).exe
clean:
	@del $(exe).exe

#/JMC /ifcOutput "..\bin-int\Debug-windows-x86_64\WalnutApp\" /GS /W3 /Zc:wchar_t 
# /I"..\vendor\imgui" 
# /I"..\vendor\glfw\include" 
# /I"..\Walnut\src" 
# /I"C:\VulkanSDK\1.3.211.0\Include" 
# /I"..\vendor\glm" 
# /ZI /Gm- /Od /Fd"..\bin-int\Debug-windows-x86_64\WalnutApp\vc143.pdb" /Zc:inline /fp:precise /external:W3 /D "WL_PLATFORM_WINDOWS" /D "WL_DEBUG" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /MDd /std:c++17 /FC /Fa"..\bin-int\Debug-windows-x86_64\WalnutApp\" /EHsc /nologo /Fo"..\bin-int\Debug-windows-x86_64\WalnutApp\" /Fp"..\bin-int\Debug-windows-x86_64\WalnutApp\WalnutApp.pch" /diagnostics:column 
# makefile
# Created by IBM WorkFrame/2 MakeMake at 3:23:25 on 14 Dec 1998
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .c .obj .rc .res 

.all: \
    .\ascii.exe

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

{e:\data\c\ascii}.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /DFULL /Tdp /O /Gm /C %s

{e:\data\c\ascii}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /DFULL /Tdp /O /Gm /C %s

.\ascii.exe: \
    .\ascii.obj \
    .\tool.obj \
    .\ascii.res \
    {$(LIB)}os2386.lib \
    {$(LIB)}cppom30.lib \
    {$(LIB)}ascii.def
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /Feascii.exe 
     os2386.lib 
     cppom30.lib 
     ascii.def
     .\ascii.obj
     .\tool.obj
<<
    rc.exe .\ascii.res ascii.exe

.\ascii.res: \
    e:\data\c\ascii\ascii.rc \
    {e:\data\c\ascii;$(INCLUDE)}ascii.ico \
    e:\data\c\ascii\ascii.h

.\ascii.obj: \
    e:\data\c\ascii\ascii.c \
    {e:\data\c\ascii;$(INCLUDE);}ascii.h \
    {e:\data\c\ascii;$(INCLUDE);}tool.h

.\tool.obj: \
    e:\data\c\ascii\tool.c \
    {e:\data\c\ascii;$(INCLUDE);}ascii.h \
    {e:\data\c\ascii;$(INCLUDE);}tool.h

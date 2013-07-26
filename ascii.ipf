:userdoc.

.*--------------------------

.nameit symbol=ascii text='ASCII Table'
.nameit symbol=version text='2.0'

.*--------------------------

:h1 res=1000.&ascii.
:i1.general help
:i1.about &ascii.
:p.
:hp2.&ascii. &version.:ehp2.
:p.
&ascii. is a program to give you easy access to the complete set of characters in a codepage.
It consists of a main window, which contains a value set with all the 256 characters, a MLE into which the
characters can be typed, and a menubar that gives you access to special functions. In the following list you
can select the different parts to learn more about them.
:sl.
:li.:link reftype=hd res=1100.Character table value set:elink.
:li.:link reftype=hd res=1200.Edit window MLE:elink.
:li.:link reftype=hd res=1300.File menu:elink.
:li.Edit menu
:li.Help menu
:esl.
:p.
The program is available in a free demo version, and a full commercial version. The free demo version differs
from the commercial version only in the fact that the save, save as and open commands are disabled. To find
out more about the differences between the two versions, or to view the license or the copyright notice you
can select an item from the following list.
:sl.
:li.:link reftype=hd res=2000.How to register:elink.
:li.:link reftype=hd res=2100.License and copyright notice:elink.
:li.:link reftype=hd res=2200.Differences between the demo version and the full version:elink.
:esl.

.*--------------------------

:h2 res=1100.The ASCII table
:i1.ASCII table
:p.
:hp2.The ASCII table:ehp2.
:p.
The table is the central part of the application. In the table all 256 characters of the current codepage are
displayed. However some codepages don't have all characters defined. In that case the characters that aren't
defined are left blank. Whether a codepage is completely shown depends on the font. Some fonts don't have all
glyphs defined. In general the fonts provide by the system (for example Times New Roman) should have all glyphs,
for other fonts you should check with the designer of the font.
:p.
If you select a character in the table it will be inserted in the :link reftype=hd res=1200.multiline entryfield
:elink., if you want to insert a character that is already selected you can double click on the character with
mouse button 1. The characters are shown in the codepage and font you've selected.
:p.
If you change codepage the table is updated, you will notice this, unless you have a very fast computer, this
is because of the way value sets are implemented in OS/2.

.*--------------------------

:h2 res=1200.The multiline entryfield
:i1.multiline entryfield
:p.
:hp2.The multiline entryfield:ehp2.
:p.
The multiline entryfield is the part where you can type text. Also if you select a character in the :link reftype=hd
res=1100.ASCII table:elink. it will be inserted into the multiline entryfield at the current cursor position.
You can load a file to edit from disk using the :link reftype=hd res=1310.File->Open:elink. menuitem. An edited
file can be saved with the :link reftype=hd res=1320.File->Save:elink. or the :link reftype=hd res=1330.File->Save:elink.
as menuitem.

.*--------------------------

:h2 res=1300.The File menu
:i1.file menu
:p.
:hp2.The File menu:ehp2.
:p.
The file menu gives you access to the common file commands, to learn more about these commands select one from
the following list.
:sl.
:li.:link reftype=hd res=1310.Open:elink.
:li.:link reftype=hd res=1320.Save:elink.
:li.:link reftype=hd res=1330.Save as:elink.
:li.:link reftype=hd res=1340.Close:elink.
:esl.

.*--------------------------

:h3 res=1310.Open
:i1.open file
:p.
:hp2.Open:ehp2.
:p.
When you select this menuitem a common file dialog will pop up, in which you can select a file to open in the
:link reftype=hd res=1200.multiline entryfield:elink.. The contents of the multiline entryfield will be replaced
by the contents of the file without warning, if you have changed something you should :link reftype=hd res=1320.save:elink.
you file first.

.*--------------------------

:h3 res=1320.Save
:i1.save file
:p.
:hp2.Save:ehp2.
:p.
This function allows you to save a named file, if your file hasn't got a name yet, this function is grayed out
and you should use the :link reftype=hd res=1330.save as:elink. function instead.

.*--------------------------

:h3 res=1330.Save as
:i1.save as
:i1.save a file under a different name
:p.
:hp2.Save as:ehp2.
:p.
This function allows you to save your file under a different name, or to save your file if it hasn't got a name
yet. When you select it, a standard file dialog will pop up, in which you can type the filename under which to
save the file. If you select a file that already exists, you will be asked whether the file should be replaced.

.*--------------------------

:h3 res=1340.Close
:i1.close
:i1.exit
:i1.quit
:p.
:hp2.Close:ehp2.
:p.
When you select close the program will end. If you have changed a file you should save it before, because close
will end the program without warning.

.*--------------------------

:h1 res=2000.How to register
:i1.register
:i1.buying &ascii.
:p.
:hp2.How to register:ehp2.
:p.
Although the first version of ASCII Table was freeware, this version is shareware. This is because it became
apparent that unless I would ask money for my program I could not continue to support this program and develop
new programs. This has different reasons, but they all come down to one simple rule, if you want something you
need to pay for it, and this program can't be an exception on that rule.
Registration is handled by BMT Micro, and upon registration you will get the full, unlimited version of the
program, which you can copy over your demo version. I kindly ask you not to give away your full version, doing
so would be a violation of the concept of shareware. It could harm OS/2 shareware programmers, and in the end
OS/2.

.*--------------------------

:h2 res=2100.License and copyright notice
:i1.license
:i1.copyright notice
:p.
:hp2.License and copyright notice:ehp2.
:p.
This program is subject to the following license. Please read the license carefully, it contains important
information.
:ol.
:li.The copyright on this program remains the property of the author, David J.
van Enckevort.
:li.This program is distributed with limited warranty.



:li.You are granted a license to use this program, you do not own this program
and any right that is not explicitly granted to you in these conditions is
reserved to the author.
:li.You may not reverse compile this program or alter it in any way.
:li.You may distribute the demo program freely, as long as you don't ask money for
the demo program.  A small fee for mailing and a diskette is allowed.  Also you may
distribute this demo program only in its original zip file. The full version you receive upon registration
is not intended for distribution.
:li.You may not distribute this program as part of a set of programs, or in any
other way combined with other programs without prior permission of the author.
:li.By using this program you accept these conditions.
:eol.

.*--------------------------

:h2 res=2200.Differences between the full version and the demo
:i1.difference
:i1.full version
:i1.demo
:p.
:hp2.Differences between the full version and the demo:ehp2.
:p.
There exist two versions of this program, the free demo version, which can be distributed, and the full
version, which cannot be distributed. These two version differ in some ways. This section will explain the
differences between the two versions.
:p.
On first view the two versions look the same, however the demo version does not support some functions. These
functions are: open, save and save as, and can be found in the file menu. When you select any of these
functions in the demo version you will get a message that it is not supported. In this way you can easily
detect which version you use. (If you downloaded this program it should be the demo version, if not contact
the author). The following is a brief description of these functions&colon.
:sl.
:li.The open function allows you to open a file for editing in the multiline entryfield. You can use it to insert
special characters in a pre-prepared document.
:li.The save function allows you to save the file you opened in the multiline entryfield. If you haven't opened a
file yet, or you didn't save the contents of the multiline entryfield with the save as function, this function
will be grayed out, and is not selectable.
:li.The save as function allows you to save the contents of the multiline entryfield to a file. It will pop up
a dialog in which you can type the name of the file, or select the file. If you select a file that already exists
you will be prompted whether to replace the file.
:esl.

:euserdoc.

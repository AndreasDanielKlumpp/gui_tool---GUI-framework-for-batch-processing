v000xx

## gui_tool - GUI framework for batch processing

Gui framework for batch processing e.g. for batch coversion of multiple files inside of folders and their sub folders
This example: Batch processing of image files for 4 levels of subfolders: (1) jpg to baseline jpg (non progressive) via jpegtran (2) png to jpg via imagemagick (convert)


### Content

A) REQUIRED

B) INSTALL

C) LICENSE


------------------

### A) REQUIRED

- Linux (tested with Debian 9)

- Qt5, qt5-default

build-essential

qmake

GNU Autotools (automake, autoconf, configure, make)

mesa-common-dev

libgl1-mesa-dev, libglu1-mesa-dev

for this example:

jpegtran
imagemagick (convert)

---------------

(gtk2: install qt5-style-plugins -> echo "export QT_QPA_PLATFORMTHEME=gtk2" >> ~/.profile) 


### B) INSTALL

- Copy Source into (replace [username]): /home/[username]/gui_tool

- mark all .sh and .desktop files as executables

- ./INSTALL.sh

- Edit Desktop file (gui_tool.desktop):

replace [username] in:

Exec=/home/[username]/gui_tool/gui_tool %F 

Icon=/home/[username]/gui_tool/Icon1.png

(To start gui_tool via command (e.g. for debug output in terminal): /home/[username]/gui_tool/gui_tool)


### C) LICENSE

gui_tool - GUI framework for batch processing
       
Copyright (c) 2021 A.D.Klumpp

This program is free software; you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published by 
the Free Software Foundation; either version 2 of the License, or 
(at your option) any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
GNU General Public License for more details.





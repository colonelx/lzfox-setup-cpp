UNIX_TARGET = unix/bin/LZFox
WIN_TARGET = win/bin/LZFox.exe

OBJ = serial_linux.cc serial_windows.cc clock.h clock.cc lzfoxwindow.cc main.cc
win:
	x86_64-w64-mingw32-g++ $(OBJ) -o $(WIN_TARGET) `x86_64-w64-mingw32-pkg-config gtkmm-3.0 --cflags --libs`

unix:
	g++ $(OBJ) -o $(UNIX_TARGET) `pkg-config gtkmm-3.0 --cflags --libs`

.PHONY: win unix
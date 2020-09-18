UNIX_TARGET = unix/bin/LZFox
WIN_TARGET = win/bin/LZFox.exe
OBJ = *.cc lzfox.res
OBJ_UNIX = exception.cc stationinfowindow.cc station_id.cc lzfox.cc lzfox_linux.cc serial_linux.cc clock.h clock.cc lzfoxwindow.cc main.cc
OBJ_WIN = exception.cc stationinfowindow.cc station_id.cc lzfox.cc lzfox_windows.cc serial_windows.cc clock.h clock.cc lzfoxwindow.cc main.cc
win:
	x86_64-w64-mingw32-g++ $(OBJ_WIN) -o $(WIN_TARGET) `x86_64-w64-mingw32-pkg-config gtkmm-3.0 --cflags --libs` 
	# -mwindows

unix:
	g++ $(OBJ_UNIX) -o $(UNIX_TARGET) `pkg-config gtkmm-3.0 --cflags --libs`  

.PHONY: win unix

g++ -std=c++11 -I/usr/local/include -I/usr/local/include/FL/images -mwindows -DWIN32 -DUSE_OPENGL32 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 main_fltk.cpp -L/usr/local/lib -mwindows -lfltk -lole32 -luuid -lcomctl32 -Wdeprecated-declarations -o main_fltk.exe

# The following line is aimed to compile cxx files coded for fltk via g++ getting arguments from fltk-config
g++ -std=c++11 `fltk-config --cxxflags` main_fltk.cpp `fltk-config --ldflags` -Wdeprecated-declarations -o main_fltk.exe
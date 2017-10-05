all:
	g++ -std=c++11 -Wall -W -Werror -o ModeChord_viewer \
main.cpp MainWindow.cpp `pkg-config gtkmm-3.0 --cflags --libs`

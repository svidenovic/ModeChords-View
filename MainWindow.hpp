
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <iostream>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
using namespace std;
using namespace Gtk;

class MainWindow : public Window{
	public:
		MainWindow();
		virtual ~MainWindow();
	protected:
		Box* search_box;
		Label* search_lbl;
		Entry* keyNmode_input;
};

#endif

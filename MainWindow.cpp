
#include "MainWindow.hpp"

MainWindow::MainWindow(){
	const unsigned int wwidth = 600;
	const unsigned int wheight = 500;
	
	this->set_default_size(wwidth,wheight);
	this->set_border_width(10);
	this->set_position( WIN_POS_CENTER );
	
	this->search_box = new Box();
	this->add( *this->search_box );
	
	this->search_lbl = new Label(" Show chords for (key mode): ");
	this->search_box->pack_start( *this->search_lbl );
	
	this->keyNmode_input = new Entry();
	this->search_box->pack_start( *this->keyNmode_input );
	
	this->search_lbl->show();
	this->keyNmode_input->show();
	this->search_box->show();
}
MainWindow::~MainWindow(){}


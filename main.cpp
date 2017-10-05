
#include <iostream>
#include <iterator>
#include <vector>
#include <regex>
#include <algorithm>
#include <string>
#include <gtkmm.h>
#include <dirent.h>
using namespace std;
using namespace Gtk;

#define img_bg_color "#009D95"

string abs_path = "/home/vidzor/Progs/ModeChords/";
string base_dir = "Mode_Chords_db/";
#define chromScale_len 12
#define modes_len 7
string chromatic_scale[] = {"A","A#","B","C","C#","D","D#","E","F","F#","G","G#"};
string modes[] = {"ionian","dorian","phrygian","lydian","mixolydian","aeolian","locrian"};
string selected_mode = "";

string prev_KeyNmode_input = "";

Window* MainWindow = nullptr;
Entry* KeyNmode_input = nullptr;
Button* GO_button = nullptr;
Label* Status_label = nullptr;
Label* Active_chord_label = nullptr;
Image* ModeChords_image = nullptr;
Button* Chord1_button = nullptr;
Button* Chord2_button = nullptr;
Button* Chord3_button = nullptr;
Button* Chord4_button = nullptr;
Button* Chord5_button = nullptr;
Button* Chord6_button = nullptr;
Button* Chord7_button = nullptr;

void load_up_image( Button* btn ){
	string btntxt = btn->get_label();
	if(btntxt.size() > 1 && selected_mode.size() > 0){
		Active_chord_label->set_text(" Active Chord: ("+btntxt+")     ");
		regex re("[:] |[/]");
		stringstream s_btxt (regex_replace (btntxt,re," "));
		string token;
		vector<string> token_list;
		while(getline(s_btxt, token, ' '))
		   token_list.push_back(token);
		
		string img_path = abs_path +base_dir +selected_mode +"/"
			+"chord-" +token_list.at(0) +"_"
			+token_list.at(1) +"-"+ token_list.at(2) +".png";
		
		ModeChords_image->set( img_path );
	}
}

void load_up_widgets(){
	auto builder = Builder::create_from_file( abs_path +"MainUI.glade" );
	builder->get_widget("MainWindow", MainWindow);
	builder->get_widget("KeyNmode_input", KeyNmode_input);
	builder->get_widget("GO_button", GO_button);
	builder->get_widget("Status_label", Status_label);
	builder->get_widget("Active_chord_label", Active_chord_label);
	builder->get_widget("ModeChords_image", ModeChords_image);
	builder->get_widget("Chord1_button", Chord1_button);
	builder->get_widget("Chord2_button", Chord2_button);
	builder->get_widget("Chord3_button", Chord3_button);
	builder->get_widget("Chord4_button", Chord4_button);
	builder->get_widget("Chord5_button", Chord5_button);
	builder->get_widget("Chord6_button", Chord6_button);
	builder->get_widget("Chord7_button", Chord7_button);
	
	Chord1_button->signal_clicked().connect(
		sigc::bind <Button*> (
			sigc::ptr_fun( & load_up_image ),
			Chord1_button
		)
	);
	Chord2_button->signal_clicked().connect(
		sigc::bind <Button*> (
			sigc::ptr_fun( & load_up_image ),
			Chord2_button
		)
	);
	Chord3_button->signal_clicked().connect(
		sigc::bind <Button*> (
			sigc::ptr_fun( & load_up_image ),
			Chord3_button
		)
	);
	Chord4_button->signal_clicked().connect(
		sigc::bind <Button*> (
			sigc::ptr_fun( & load_up_image ),
			Chord4_button
		)
	);
	Chord5_button->signal_clicked().connect(
		sigc::bind <Button*> (
			sigc::ptr_fun( & load_up_image ),
			Chord5_button
		)
	);
	Chord6_button->signal_clicked().connect(
		sigc::bind <Button*> (
			sigc::ptr_fun( & load_up_image ),
			Chord6_button
		)
	);
	Chord7_button->signal_clicked().connect(
		sigc::bind <Button*> (
			sigc::ptr_fun( & load_up_image ),
			Chord7_button
		)
	);
}

string gen_re_sub_pattern_( string mode_name ){
	// make it so it accepts partial inputs, (ly = lydian)
	string re_pattern = "";
	for (size_t ln=2; ln<mode_name.size()+1; ln++){
		string mode_name_part = "";
		for (size_t i=0; i<ln; i++)
			mode_name_part += mode_name[i];
		re_pattern += mode_name_part;
		if(ln < mode_name.size())
			re_pattern += "|";
	}
	return re_pattern;
}
string gen_re_pattern(){
	// key (note) part:
	string re_pattern = "^\\s*(";
	string cs_tmp = "";
	size_t i = 0;
	while (i < chromScale_len-1){
		re_pattern += chromatic_scale[i] +"|";
		cs_tmp = chromatic_scale[i];
		transform( 
			cs_tmp.begin(), cs_tmp.end(), cs_tmp.begin(), ::tolower
		);
		re_pattern += cs_tmp +"|";
		i++;
	}
	re_pattern += chromatic_scale[i] +"|";
	cs_tmp = chromatic_scale[i];
	transform( 
		cs_tmp.begin(), cs_tmp.end(), cs_tmp.begin(), ::tolower
	);
	re_pattern += cs_tmp +")\\s+(";
	
	// mode part:
	for (i=0; i < modes_len-1; i++)
		re_pattern += gen_re_sub_pattern_(modes[i]) +"|";
	re_pattern += gen_re_sub_pattern_(modes[i]) +")\\s*$";
	
	return re_pattern;
}

string* extract_key_mode(){
	string key_mode = (string) KeyNmode_input->get_text();
	string key = "", mode = "";
	
	regex re_pattern( gen_re_pattern() );
	
	auto m_begin = sregex_iterator(
		key_mode.begin(), key_mode.end(), re_pattern
	);
    auto m_end = sregex_iterator();
    
    for (sregex_iterator i = m_begin; i != m_end; ++i) {
        smatch match = *i;
        string key = match.str(1);
        string mode = match.str(2);
		if( key.size() && mode.size() ){
			string* key_mode_pair = new string[2];
			key_mode_pair[0] = key;
			key_mode_pair[1] = mode;
			return key_mode_pair;
		}
    }
    return nullptr;
}

void update_widgets( string key, string mode ){
	ModeChords_image->clear();
	Active_chord_label->set_text(" Chords: ");
	
	string path = abs_path + base_dir +key+"_"+mode+"/";
	string* img_name;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL){
			img_name = new string(ent->d_name);
			regex re_pattern( "^chord-([1-7])_([A-G][#]?(m|dim)?)-([A-G][#]?).png$" );
			smatch match;
			if (regex_search( *img_name, match, re_pattern ) && match.size()>1){
				string btn_txt = match.str(1)+": "
					+match.str(2) +"/"+ match.str(4);
				switch( stoi(match.str(1)) ){ // chord_num
					case 1: Chord1_button->set_label(btn_txt); break;
					case 2: Chord2_button->set_label(btn_txt); break;
					case 3: Chord3_button->set_label(btn_txt); break;
					case 4: Chord4_button->set_label(btn_txt); break;
					case 5: Chord5_button->set_label(btn_txt); break;
					case 6: Chord6_button->set_label(btn_txt); break;
					case 7: Chord7_button->set_label(btn_txt); break;
					default: break;
				}
			}
		}
		closedir (dir);
	}
}

void activate_key_mode(){
	if(KeyNmode_input->get_text() != prev_KeyNmode_input){
		prev_KeyNmode_input = KeyNmode_input->get_text();
	}else return;
	
	string* key_mode_pair = extract_key_mode();
	if( key_mode_pair == nullptr ){
		selected_mode = "";
		Chord1_button->set_label("1");
		Chord2_button->set_label("2");
		Chord3_button->set_label("3");
		Chord4_button->set_label("4");
		Chord5_button->set_label("5");
		Chord6_button->set_label("6");
		Chord7_button->set_label("7");
		Active_chord_label->set_text(" Chords: ");
		Status_label->set_text(" Invalid input ");
		ModeChords_image->clear();
	}else{
		string key = key_mode_pair[0];
		string mode = key_mode_pair[1];
		delete[] key_mode_pair;
		
		transform( 
			key.begin(), key.end(), key.begin(), ::toupper
		);
		regex re_pattern( "("+mode+"\\w*)" );
		for (int i=0; i<modes_len; i++){
			smatch match;
			if (regex_search( modes[i], match, re_pattern ) && match.size()){
				mode = match.str(1); break;
			}
		}
		selected_mode = key+"_"+mode;
		Status_label->set_text(" Selected mode: "+key+" "+mode);
		KeyNmode_input->set_text( key+" "+mode );
		update_widgets( key, mode );
	}
}

bool KeyNmode_input_key_press (GdkEventKey* event){
	if(event->keyval == GDK_KEY_Return){
		activate_key_mode();
		return true;
	}
	return false;
}

bool MainWindow_alt_N_keyPress (GdkEventKey* e){
	if((e->keyval == GDK_KEY_1) && (e->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK){
		load_up_image( Chord1_button );
		return true;
	}
	else if((e->keyval == GDK_KEY_2) && (e->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK){
		load_up_image( Chord2_button );
		return true;
	}
	else if((e->keyval == GDK_KEY_3) && (e->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK){
		load_up_image( Chord3_button );
		return true;
	}
	else if((e->keyval == GDK_KEY_4) && (e->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK){
		load_up_image( Chord4_button );
		return true;
	}
	else if((e->keyval == GDK_KEY_5) && (e->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK){
		load_up_image( Chord5_button );
		return true;
	}
	else if((e->keyval == GDK_KEY_6) && (e->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK){
		load_up_image( Chord6_button );
		return true;
	}
	else if((e->keyval == GDK_KEY_7) && (e->state &(GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK)) == GDK_MOD1_MASK){
		load_up_image( Chord7_button );
		return true;
	}
	return false;
}

int main(int argc, char** argv){
	auto app = Application::create(argc, argv, "org.gtkmm.examples.base");
	
	load_up_widgets();
	ModeChords_image->override_background_color(Gdk::RGBA(img_bg_color));
	
	MainWindow->signal_key_press_event().connect( 
		sigc::ptr_fun( & MainWindow_alt_N_keyPress )
	);
	
	GO_button->signal_clicked().connect(
		sigc::ptr_fun( & activate_key_mode )
	);
	
	KeyNmode_input->signal_key_press_event().connect( 
		sigc::ptr_fun( & KeyNmode_input_key_press )
	);
	KeyNmode_input->add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
	
	return app->run( *MainWindow );
}

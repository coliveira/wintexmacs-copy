
/******************************************************************************
* MODULE     : aqua_dialogues.mm
* DESCRIPTION: Aqua dialogues widgets classes
* COPYRIGHT  : (C) 2007  Massimiliano Gubinelli
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "mac_cocoa.h" 

#include "aqua_dialogues.h"

#include "widget.hpp" 
#include "message.hpp"
#include "aqua_utilities.h"
#include "aqua_other_widgets.h"
#include "aqua_basic_widgets.h"

#include "url.hpp"
#include "analyze.hpp"

#define NOT_IMPLEMENTED { if (DEBUG_EVENTS) cout << "STILL NOT IMPLEMENTED\n";  }

#pragma mark aqua_chooser_widget_rep

class aqua_chooser_widget_rep: public aqua_widget_rep {
protected:	
	command cmd;
	string type;
	string mgn;
	string win_title;
  string directory;
  coord2 position;
  coord2 size;
  string file;
	
public:
  aqua_chooser_widget_rep (command, string, string);
  ~aqua_chooser_widget_rep ();
	
	virtual void send (slot s, blackbox val);
  virtual blackbox query (slot s, int type_id);
  virtual widget read (slot s, blackbox index);
  virtual void write (slot s, blackbox index, widget w);
  virtual void notify (slot s, blackbox new_val);
	//  virtual void connect (slot s, widget w2, slot s2);
	//  virtual void deconnect (slot s, widget w2, slot s2);
	virtual widget plain_window_widget (string s);

	void perform_dialog();
};

aqua_chooser_widget_rep::aqua_chooser_widget_rep (command _cmd, string _type, string _mgn) 
: aqua_widget_rep(), cmd(_cmd), type(_type), 
  mgn(_mgn), position (coord2 (0, 0)), 
  size (coord2 (100, 100)), file ("")
{
}

aqua_chooser_widget_rep::~aqua_chooser_widget_rep()  {  }



void
aqua_chooser_widget_rep::send (slot s, blackbox val) {
  switch (s) {
		case SLOT_VISIBILITY:
		{	
			check_type<bool> (val, "SLOT_VISIBILITY");
			bool flag = open_box<bool> (val);
      (void) flag;
      NOT_IMPLEMENTED
		}	
			break;
		case SLOT_SIZE:
		{
			if (type_box (val) != type_helper<coord2>::id)
				fatal_error ("type mismatch", "SLOT_SIZE");
			size = open_box<coord2> (val);
		}
			break;
		case SLOT_POSITION:
		{
			if (type_box (val) != type_helper<coord2>::id)
				fatal_error ("type mismatch", "SLOT_POSITION");
			position = open_box<coord2> (val);
		}
			break;
		case SLOT_KEYBOARD_FOCUS:
		{
			if (type_box (val) != type_helper<bool>::id)
				fatal_error ("type mismatch", "SLOT_KEYBOARD_FOCUS");
			perform_dialog();
		}
			break;
			
		case SLOT_STRING_INPUT:
	//		send_string (THIS, "input", val);
        NOT_IMPLEMENTED 
			break;
		case SLOT_INPUT_TYPE:
        if (type_box (val) != type_helper<string>::id)
          fatal_error ("type mismatch", "SLOT_DIRECTORY");
        type = open_box<string> (val);        
		//	send_string (THIS, "type", val);
			break;
#if 0
		case SLOT_INPUT_PROPOSAL:
			//send_string (THIS, "default", val);
			break;
#endif
		case SLOT_FILE:
			//send_string (THIS, "file", val);
        NOT_IMPLEMENTED
			break;
		case SLOT_DIRECTORY:
        if (type_box (val) != type_helper<string>::id)
          fatal_error ("type mismatch", "SLOT_DIRECTORY");
        directory = open_box<string> (val);
        directory = as_string (url_pwd () * url_system (directory));
        break;
        
		default:
			aqua_widget_rep::send(s,val);
  }
}


blackbox
aqua_chooser_widget_rep::query (slot s, int type_id) {
  switch (s) {
		case SLOT_POSITION:  
		{
			typedef pair<SI,SI> coord2;
			if (type_id != type_helper<coord2>::id)
				fatal_error ("type mismatch (SLOT_POSITION)", "aqua_chooser_widget_rep::query");
			return close_box<coord2> (position);
		}
		case SLOT_SIZE:
		{
			typedef pair<SI,SI> coord2;
			if (type_id != type_helper<coord2>::id)
				fatal_error ("type mismatch (SLOT_SIZE)", "aqua_chooser_widget_rep::query");
			return close_box<coord2> (size);
		}
			
		case SLOT_STRING_INPUT:
		{
			if (type_id != type_helper<string>::id)
				fatal_error ("type mismatch (SLOT_STRING_INPUT)", "aqua_chooser_widget_rep::query");
			return close_box<string> (file);
		}

		default:
			return aqua_widget_rep::query(s,type_id);
  }
}


void
aqua_chooser_widget_rep::notify (slot s, blackbox new_val) {
  switch (s) {
			default: ;
  }
  widget_rep::notify (s, new_val);
}

widget
aqua_chooser_widget_rep::read (slot s, blackbox index) {
  switch (s) {
		case SLOT_WINDOW:
			check_type_void (index, "SLOT_WINDOW");
			return this;
		case SLOT_FORM_FIELD:
			check_type<int> (index, "SLOT_FORM_FIELD");
			return this;
		case SLOT_FILE:
			check_type_void (index, "SLOT_FILE");
			return this;
		case SLOT_DIRECTORY:
			check_type_void (index, "SLOT_DIRECTORY");
			return this;
		default:
			return aqua_widget_rep::read(s,index);
  }
}

void
aqua_chooser_widget_rep::write (slot s, blackbox index, widget w) {
  switch (s) {
		default:
			aqua_widget_rep::write(s,index,w);
  }
}


widget aqua_chooser_widget_rep::plain_window_widget (string s)
{
	win_title = s;
	return this;
}



widget file_chooser_widget (command cmd, string type, string mgn) 
// file chooser widget for files of a given type; for files of type "image",
// the widget includes a previsualizer and a default magnification
// for importation can be specified
{
	return tm_new <aqua_chooser_widget_rep> (cmd,type,mgn);
}

#if 0
void aqua_chooser_widget_rep::perform_dialog()
{
	int result;
	NSArray *fileTypes = [NSArray arrayWithObject:@"tm"];
	NSOpenPanel *oPanel = [NSOpenPanel openPanel];
    [oPanel setTitle:to_nsstring(win_title)];
	[oPanel setAllowsMultipleSelection:YES];
	result = [oPanel runModalForDirectory:NSHomeDirectory()
																	 file:nil types:fileTypes];
	if (result == NSOKButton) {
		NSArray *filesToOpen = [oPanel filenames];
		int i, count = [filesToOpen count];
		for (i=0; i<count; i++) {
			NSString *aFile = [filesToOpen objectAtIndex:i];
//			id currentDoc = [[ToDoDoc alloc] initWithFile:aFile];
		}
		if (count > 0) {
			file = from_nsstring([filesToOpen objectAtIndex:0]);
			url u= url_system (scm_unquote (file));
      file = "(url-system " * scm_quote (as_string (u)) * ")";

		}
	}
  cmd();	
}
#else
void aqua_chooser_widget_rep::perform_dialog()
{
  int result;
  NSArray *fileTypes = [NSArray arrayWithObject:@"tm"];
  NSSavePanel *oPanel = [NSSavePanel savePanel];
  [oPanel setTitle:to_nsstring(win_title)];
//  [oPanel setMessage:@"Choose a file."];
  [oPanel setNameFieldLabel:@"File:"];
  [oPanel setPrompt:@"Choose"];
  [oPanel setAllowedFileTypes:fileTypes];
 // [oPanel setAllowsMultipleSelection:YES];
  NSPoint pos = to_nspoint(position);
  NSRect r = NSMakeRect(0,0,0,0);
  r.size = [oPanel frame].size;
  NSOffsetRect(r, pos.x - r.size.width/2, pos.y - r.size.height/2);
  [oPanel setFrameOrigin:r.origin];
  
  result = [oPanel runModalForDirectory:to_nsstring(directory)
                                   file:nil ];
  if (result == NSOKButton) {
      file = from_nsstring([oPanel filename]);
      url u= url_system (scm_unquote (file));
    if (type == "image")
      file = "(list (url-system " *
      scm_quote (as_string (u)) *
      ") \"100\" \"100\" \"0\" \"0\" \"10\" \"10\")";
    //FIXME: fake image dimensions
    else
      file = "(url-system " * scm_quote (as_string (u)) * ")";
  } else {
    file = "#f";
  }
  cmd ();	
}

#endif
#pragma mark aqua_input_widget_rep

class aqua_field_widget;

class aqua_input_widget_rep: public aqua_widget_rep {
protected:	
  command cmd;
  array<aqua_field_widget> fields;
  coord2 size, position;
  string win_title; 	
public:
  aqua_input_widget_rep (command, array<string>);
  ~aqua_input_widget_rep ();
	
  virtual void send (slot s, blackbox val);
  virtual blackbox query (slot s, int type_id);
  virtual widget read (slot s, blackbox index);
  virtual void write (slot s, blackbox index, widget w);
  virtual void notify (slot s, blackbox new_val);
	//  virtual void connect (slot s, widget w2, slot s2);
	//  virtual void deconnect (slot s, widget w2, slot s2);
	virtual widget plain_window_widget (string s);

	void perform_dialog();
};

class aqua_field_widget_rep : public widget_rep {
  string prompt;
  string input;
  string type;
  array<string> proposals;
   aqua_input_widget_rep *parent;
public:
  aqua_field_widget_rep(aqua_input_widget_rep *_parent) : widget_rep(), prompt(""), input(""),  proposals(), parent(_parent) {};
  virtual void send (slot s, blackbox val);
  virtual blackbox query (slot s, int type_id);

  friend class aqua_input_widget_rep;
};


void
aqua_field_widget_rep::send (slot s, blackbox val) {
  switch (s) {
		case SLOT_STRING_INPUT:
	  {
		  if (type_box (val) != type_helper<string>::id)
			  fatal_error ("type mismatch", "SLOT_STRING_INPUT");
		  input =  open_box<string> (val);
	  }
	//		send_string (THIS, "input", val);
			break;
		case SLOT_INPUT_TYPE:
	  {
		  if (type_box (val) != type_helper<string>::id)
			  fatal_error ("type mismatch", "SLOT_INPUT_TYPE");
		  type =  open_box<string> (val);
	  }
		  break;
		case SLOT_INPUT_PROPOSAL:
	  {
		  if (type_box (val) != type_helper<string>::id)
			  fatal_error ("type mismatch", "SLOT_INPUT_PROPOSAL");
		  proposals <<  open_box<string> (val);
	  }
			//send_string (THIS, "default", val);
			break;
		case SLOT_KEYBOARD_FOCUS:
      {
	     parent->send(s,val);
	  }
	      break;
		default:
			widget_rep::send(s,val);
  }
}

blackbox
aqua_field_widget_rep::query (slot s, int type_id) {
  switch (s) {
		case SLOT_STRING_INPUT:
		{
			if (type_id != type_helper<string>::id)
				fatal_error ("type mismatch (SLOT_STRING_INPUT)", "aqua_field_widget_rep::query");
			return close_box<string> (input);
		}

		default:
			return widget_rep::query(s,type_id);
  }
}


class aqua_field_widget {
public:
ABSTRACT_NULL(aqua_field_widget);
};
ABSTRACT_NULL_CODE(aqua_field_widget);



aqua_input_widget_rep::aqua_input_widget_rep (command _cmd, array<string> _prompts) 
: aqua_widget_rep(), cmd(_cmd),  size(coord2(100,100)), position(coord2(0,0)), win_title(""), fields(N(_prompts))
{
  for(int i=0; i < N(_prompts); i++) {
   fields[i] = tm_new <aqua_field_widget_rep> (this);
   fields[i]->prompt = _prompts[i];
  }
}

aqua_input_widget_rep::~aqua_input_widget_rep()  {  }



void
aqua_input_widget_rep::send (slot s, blackbox val) {
  switch (s) {
		case SLOT_VISIBILITY:
		{	
			check_type<bool> (val, "SLOT_VISIBILITY");
			bool flag = open_box<bool> (val);
      (void) flag;
      NOT_IMPLEMENTED 
		}	
			break;
		case SLOT_SIZE:
		{
			if (type_box (val) != type_helper<coord2>::id)
				fatal_error ("type mismatch", "SLOT_SIZE");
			size = open_box<coord2> (val);
		}
			break;
		case SLOT_POSITION:
		{
			if (type_box (val) != type_helper<coord2>::id)
				fatal_error ("type mismatch", "SLOT_POSITION");
			position = open_box<coord2> (val);
		}
			break;
		case SLOT_KEYBOARD_FOCUS:
		{
			if (type_box (val) != type_helper<bool>::id)
				fatal_error ("type mismatch", "SLOT_KEYBOARD_FOCUS");
			perform_dialog();
		}
			break;
			

		default:
			aqua_widget_rep::send(s,val);
  }
}


blackbox
aqua_input_widget_rep::query (slot s, int type_id) {
  switch (s) {
		case SLOT_POSITION:  
		{
			typedef pair<SI,SI> coord2;
			if (type_id != type_helper<coord2>::id)
				fatal_error ("type mismatch (SLOT_POSITION)", "aqua_input_widget_rep::query");
			return close_box<coord2> (position);
		}
		case SLOT_SIZE:
		{
			typedef pair<SI,SI> coord2;
			if (type_id != type_helper<coord2>::id)
				fatal_error ("type mismatch (SLOT_SIZE)", "aqua_input_widget_rep::query");
			return close_box<coord2> (size);
		}
      case SLOT_STRING_INPUT:
        return fields[0]->query(s,type_id);
			

		default:
			return aqua_widget_rep::query(s,type_id);
  }
}


void
aqua_input_widget_rep::notify (slot s, blackbox new_val) {
  switch (s) {
			default: ;
  }
  widget_rep::notify (s, new_val);
}

widget
aqua_input_widget_rep::read (slot s, blackbox index) {
  switch (s) {
		case SLOT_WINDOW:
			check_type_void (index, "SLOT_WINDOW");
			return this;
		case SLOT_FORM_FIELD:
			check_type<int> (index, "SLOT_FORM_FIELD");
			return (widget_rep*)(fields[open_box<int>(index)].rep);
		default:
			return aqua_widget_rep::read(s,index);
  }
}

void
aqua_input_widget_rep::write (slot s, blackbox index, widget w) {
  switch (s) {
		default:
			aqua_widget_rep::write(s,index,w);
  }
}


widget aqua_input_widget_rep::plain_window_widget (string s)
{
	win_title = s;
	return this;
}


@interface TMInputHelper : NSObject
{
@public
  IBOutlet NSMatrix *form;
  IBOutlet NSWindow *dialog;
  aqua_tm_widget_rep *wid;
}
- (IBAction) doForm:(id)sender;
@end

@implementation TMInputHelper
- (id) init
{
	self = [super init];
	if (self != nil) {
		[NSBundle loadNibNamed:@"InputPanel" owner:self];
      [dialog setReleasedWhenClosed:NO];
      wid = NULL;
	}
	return self;
}
- (void) dealloc
{
	[dialog release];
	[super dealloc];
}

- (IBAction) doForm:(id)sender
{
  if ([sender tag] == 0)
  {
    [NSApp stopModalWithCode:0]; // OK button
   }
  else
  {
    [NSApp stopModalWithCode:1]; // Cancel button
  }
}

- (void) delayedRun
{
  int code = [NSApp runModalForWindow: dialog];
  // Dialog is up here.
  [NSApp endSheet: dialog];
  [dialog orderOut: self];
  
  
  if (code == 0) { // Ok button
    NSString *ans = [(NSComboBoxCell*)[form cellAtRow:0 column:1] stringValue];
    ((aqua_input_text_widget_rep*)wid->int_input.rep)->text = scm_quote(from_nsstring(ans));
    ((aqua_input_text_widget_rep*)wid->int_input.rep)->cmd();
  }
  else  { // Cancel button
  }
  
  
  [self release]; // autodestroy
}
@end


void aqua_input_widget_rep::perform_dialog()
{
  TMInputHelper *ih = [[TMInputHelper alloc] init];
  NSMatrix *form = ih->form;
  [form renewRows:N(fields) columns:2];
  for(int i=0; i<N(fields); i++) {
    NSCell *cell = [[[NSCell alloc] initTextCell:to_nsstring(fields[i]->prompt)] autorelease];
    [form putCell:cell atRow:i column:0];
    NSComboBoxCell *cell2 = [[[NSComboBoxCell alloc] initTextCell:to_nsstring(fields[i]->input)] autorelease];
    [cell2 setEditable:YES];
    [cell2 setCompletes:YES];
    [form putCell:cell2 atRow:i column:1];
  //  [cell2 addItemWithObjectValue:to_nsstring(fields[i]->input)];
    for(int j=0; j < N(fields[i]->proposals); j++)
    {
      [cell2 addItemWithObjectValue:to_nsstring(fields[i]->proposals[j])];
    }
  }
  
  NSRect rect0 = [form frame];
  [form sizeToFit];
  NSRect rect1 = [form frame];
  [form setFrame:rect0];
  NSRect frame = [ih->dialog frame];
  frame.size.width += rect1.size.width - rect0.size.width;
  frame.size.height += rect1.size.height - rect0.size.height;
  [ih->dialog setFrame:frame display:NO];
  
  NSModalSession session = [NSApp beginModalSessionForWindow:ih->dialog];
  NSInteger code;
  for (;;) {
    code = [NSApp runModalSession:session];
    if (code != NSRunContinuesResponse)
      break;
    //   [self doSomeWork];
  }
  [NSApp endModalSession:session];
  [ih->dialog close];

  if (code == 0) { // Ok button
    
    for(int i=0; i<N(fields); i++) {
      NSString *ans = [(NSComboBoxCell*)[form cellAtRow:i column:1] stringValue];
      fields[i]->input = scm_quote(from_nsstring(ans));
    }
  }
  else  { // Cancel button
    for(int i=0; i<N(fields); i++) {
      fields[i]->input = "#f";
    }
  }
 
  
  [ih release];
  cmd();
}




widget inputs_list_widget (command call_back, array<string> prompts)
// a dialogue widget with Ok and Cancel buttons and a series of textual
// input widgets with specified prompts
{
	return tm_new <aqua_input_widget_rep> (call_back,prompts);

}



widget input_text_widget (command call_back, string type, array<string> def) 
// a textual input widget for input of a given type and a list of suggested
// default inputs (the first one should be displayed, if there is one)
{
  return tm_new <aqua_input_text_widget_rep> (call_back, type, def);
}


void aqua_tm_widget_rep::do_interactive_prompt()
{
  TMInputHelper *ih = [[TMInputHelper alloc] init];
  ih->wid = this;
  NSMatrix *form = ih->form;
  [form renewRows:1 columns:2];
  NSCell *cell = [[[NSCell alloc] initTextCell:to_nsstring(((aqua_text_widget_rep*)int_prompt.rep)->str)] autorelease];
  [form putCell:cell atRow:0 column:0];
  NSComboBoxCell *cell2 = [[[NSComboBoxCell alloc] initTextCell:@""] autorelease];
  [cell2 setEditable:YES];
  [cell2 setCompletes:YES];
  [form putCell:cell2 atRow:0 column:1];
  [form setKeyCell:cell2];
  aqua_input_text_widget_rep *it = (aqua_input_text_widget_rep*)int_input.rep;
  for(int j=0; j < N(it->def); j++)
  {
    if (j==0) [cell2 setStringValue:to_nsstring(it->def[j])];
    [cell2 addItemWithObjectValue:to_nsstring(it->def[j])];
  }
  
  NSRect rect0 = [form frame];
  [form sizeToFit];
  NSRect rect1 = [form frame];
  [form setFrame:rect0];
  NSRect frame = [ih->dialog frame];
  frame.size.width += rect1.size.width - rect0.size.width;
  frame.size.height += rect1.size.height - rect0.size.height;
  [ih->dialog setFrame:frame display:NO];
  
  [NSApp beginSheet: ih->dialog
     modalForWindow: [view window]
      modalDelegate: nil
     didEndSelector: nil
        contextInfo: nil];
  [ih performSelector:@selector(delayedRun) withObject:nil afterDelay:0.0];
}

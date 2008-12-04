//---------------------------------------------------------------------------
//
// Name:        chessFrame.cpp
// Author:      gurkesaft
// Created:     6/23/2007 3:18:51 PM
// Description: chessFrame class implementation
//
//---------------------------------------------------------------------------

#include "chessFrame.h"
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/stopwatch.h>
#define TOP true
#define BOTTOM false

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// chessFrame
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(chessFrame,wxFrame)
	////Manual Code Start
	//EVT_KEY_DOWN(chessFrame::OnKeyDown)
	////Manual Code End
	
	EVT_CLOSE(chessFrame::OnClose)
	EVT_TIMER(ID_TIMERUPDATE,chessFrame::timerUpdateTimer)
	EVT_TIMER(ID_TIMERCLOCK,chessFrame::timerClockTimer)
	EVT_TIMER(ID_TIMERRELEASEMOUSE,chessFrame::timerReleaseMouseTimer)
	EVT_TIMER(ID_TIMERDRAW,chessFrame::timerDrawTimer)
	EVT_BUTTON(ID_BUTTONSWAP,chessFrame::buttonSwapClick)
	EVT_BUTTON(ID_BUTTONRESET,chessFrame::buttonResetClick)
	EVT_BUTTON(ID_BUTTONPAUSE,chessFrame::buttonPauseClick)
	EVT_TEXT_ENTER(ID_EDITTIMEBOTTOM,chessFrame::editTimeBottomEnter)
	EVT_TEXT(ID_EDITTIMEBOTTOM,chessFrame::editTimeBottomUpdated)
	EVT_TEXT_ENTER(ID_EDITTIMETOP,chessFrame::editTimeTopEnter)
	EVT_TEXT(ID_EDITTIMETOP,chessFrame::editTimeTopUpdated)
	EVT_TOGGLEBUTTON(ID_TOGGLETOP,chessFrame::toggleTopClick)
	EVT_TOGGLEBUTTON(ID_TOGGLEBOTTOM,chessFrame::toggleBottomClick)
	EVT_BUTTON(ID_BUTTONLOAD,chessFrame::buttonLoadClick)
	EVT_BUTTON(ID_BUTTONSAVE,chessFrame::buttonSaveClick)
	EVT_BUTTON(ID_BUTTONOPTIONS,chessFrame::buttonOptionsClick)
	EVT_BUTTON(ID_BUTTONTEST,chessFrame::buttonTestClick)
	EVT_CHECKBOX(ID_CHECKBOXREVERSED,chessFrame::checkBoxReversedClick)
	EVT_CHECKBOX(ID_CHECKBOXEDITMODE,chessFrame::checkBoxEditModeClick)
	EVT_BUTTON(ID_BUTTONDISCONNECT,chessFrame::buttonDisconnectClick)
	EVT_BUTTON(ID_BUTTONSETUP,chessFrame::buttonSetupClick)
	EVT_TEXT_ENTER(ID_EDITCHAT,chessFrame::editChatEnter)
	EVT_BUTTON(ID_BUTTONHOST,chessFrame::buttonHostClick)
	EVT_BUTTON(ID_BUTTONJOIN,chessFrame::buttonJoinClick)
END_EVENT_TABLE()
////Event Table End

chessFrame::chessFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
	options  = new optionsFrame(this);
    board    = new MyGLCanvas(this, panelBoard, ID_PANELBOARD, wxPoint(0,0), panelBoard->GetSize());
    hostjoin = new HostJoinDialog(this);
    
    // connect the hotkeys
    board->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyGLCanvas::OnKeyDown));
    board->SetFocus();

    // set some defaults
    client = NULL;
    server = NULL;
    
    // set the colors
    board->SetDefaultColors();
    
    // now load the last user settings.
    LoadSettings();
    
    // if we're on quiet debug mode, clear the last copy of debug.txt
    if(options->checkBoxDebug->GetValue())
    {
        wxFile f;
        f.Open("debug.txt", wxFile::write);
        f.Close();
    }
    
    // set the button colors on the options dialog
    options->UpdateColorButtons();
}

chessFrame::~chessFrame()
{
    Debug("~chessFrame()");
    //options->Hide();
    //options->Destroy();
}

void chessFrame::CreateGUIControls()
{
    loading_chessnuts = true;
    
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	panelBackground = new wxPanel(this, ID_PANELBACKGROUND, wxPoint(0,0), wxSize(867,514));
	panelBackground->SetBackgroundColour(wxColour(244,244,244));
	panelBackground->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	panelBoard = new wxPanel(panelBackground, ID_PANELBOARD, wxPoint(4,4), wxSize(508,508), wxWANTS_CHARS);
	panelBoard->SetBackgroundColour(wxColour(*wxWHITE));
	panelBoard->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	memoLog = new wxTextCtrl(panelBackground, ID_MEMOLOG, wxT(""), wxPoint(516,24), wxSize(245,231), wxTE_READONLY | wxTE_MULTILINE, wxDefaultValidator, wxT("memoLog"));
	memoLog->SetMaxLength(0);
	memoLog->AppendText(wxT(""));
	memoLog->SetFocus();
	memoLog->SetInsertionPointEnd();
	memoLog->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonJoin = new wxButton(panelBackground, ID_BUTTONJOIN, wxT("Join"), wxPoint(764,24), wxSize(50,21), 0, wxDefaultValidator, wxT("buttonJoin"));
	buttonJoin->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonHost = new wxButton(panelBackground, ID_BUTTONHOST, wxT("Host"), wxPoint(816,24), wxSize(50,21), 0, wxDefaultValidator, wxT("buttonHost"));
	buttonHost->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	editChat = new wxTextCtrl(panelBackground, ID_EDITCHAT, wxT(""), wxPoint(546,259), wxSize(319,19), wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("editChat"));
	editChat->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonSetup = new wxButton(panelBackground, ID_BUTTONSETUP, wxT("Set Up"), wxPoint(819,342), wxSize(47,21), 0, wxDefaultValidator, wxT("buttonSetup"));
	buttonSetup->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText3 = new wxStaticText(panelBackground, ID_WXSTATICTEXT3, wxT("Chat:"), wxPoint(517,262), wxDefaultSize, 0, wxT("WxStaticText3"));
	WxStaticText3->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	wxArrayString arrayStringFor_listPeople;
	listPeople = new wxListBox(panelBackground, ID_LISTPEOPLE, wxPoint(765,48), wxSize(100,184), arrayStringFor_listPeople, wxLB_SINGLE);
	listPeople->Enable(false);
	listPeople->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonDisconnect = new wxButton(panelBackground, ID_BUTTONDISCONNECT, wxT("Disconnect"), wxPoint(764,235), wxSize(102,21), 0, wxDefaultValidator, wxT("buttonDisconnect"));
	buttonDisconnect->Enable(false);
	buttonDisconnect->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	wxArrayString arrayStringFor_comboSetup;
	arrayStringFor_comboSetup.Add(wxT("Standard Chess"));
	arrayStringFor_comboSetup.Add(wxT("Basic Random"));
	arrayStringFor_comboSetup.Add(wxT("Fischer Random"));
	arrayStringFor_comboSetup.Add(wxT("Just Pawns"));
	arrayStringFor_comboSetup.Add(wxT("Clear Board"));
	comboSetup = new wxComboBox(panelBackground, ID_COMBOSETUP, wxT(""), wxPoint(516,342), wxSize(189,21), arrayStringFor_comboSetup, wxCB_READONLY, wxDefaultValidator, wxT("comboSetup"));
	comboSetup->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	checkBoxEditMode = new wxCheckBox(panelBackground, ID_CHECKBOXEDITMODE, wxT("Edit Mode"), wxPoint(584,320), wxSize(65,17), 0, wxDefaultValidator, wxT("checkBoxEditMode"));
	checkBoxEditMode->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	checkBoxReversed = new wxCheckBox(panelBackground, ID_CHECKBOXREVERSED, wxT("Reversed"), wxPoint(516,320), wxSize(63,17), 0, wxDefaultValidator, wxT("checkBoxReversed"));
	checkBoxReversed->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	wxArrayString arrayStringFor_comboSetupStyle;
	arrayStringFor_comboSetupStyle.Add(wxT("Normal"));
	arrayStringFor_comboSetupStyle.Add(wxT("Odd"));
	arrayStringFor_comboSetupStyle.Add(wxT("Uncorrelated"));
	comboSetupStyle = new wxComboBox(panelBackground, ID_COMBOSETUPSTYLE, wxT(""), wxPoint(709,342), wxSize(107,21), arrayStringFor_comboSetupStyle, 0, wxDefaultValidator, wxT("comboSetupStyle"));
	comboSetupStyle->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonTest = new wxButton(panelBackground, ID_BUTTONTEST, wxT("Test"), wxPoint(404,71), wxSize(44,25), 0, wxDefaultValidator, wxT("buttonTest"));
	buttonTest->Show(false);
	buttonTest->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonOptions = new wxButton(panelBackground, ID_BUTTONOPTIONS, wxT("Options"), wxPoint(819,318), wxSize(47,21), 0, wxDefaultValidator, wxT("buttonOptions"));
	buttonOptions->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonSave = new wxButton(panelBackground, ID_BUTTONSAVE, wxT("Save"), wxPoint(740,318), wxSize(37,21), 0, wxDefaultValidator, wxT("buttonSave"));
	buttonSave->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonLoad = new wxButton(panelBackground, ID_BUTTONLOAD, wxT("Load"), wxPoint(779,318), wxSize(38,21), 0, wxDefaultValidator, wxT("buttonLoad"));
	buttonLoad->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	toggleBottom = new wxToggleButton(panelBackground, ID_TOGGLEBOTTOM, wxT(""), wxPoint(515,461), wxSize(52,52), 0, wxDefaultValidator, wxT("toggleBottom"));
	toggleBottom->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	toggleTop = new wxToggleButton(panelBackground, ID_TOGGLETOP, wxT(""), wxPoint(515,405), wxSize(52,52), 0, wxDefaultValidator, wxT("toggleTop"));
	toggleTop->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	editTimeTop = new wxTextCtrl(panelBackground, ID_EDITTIMETOP, wxT("00:15:00"), wxPoint(571,406), wxSize(245,51), wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("editTimeTop"));
	editTimeTop->SetFont(wxFont(37, wxSWISS, wxNORMAL,wxBOLD, false, wxT("Courier New")));

	editTimeBottom = new wxTextCtrl(panelBackground, ID_EDITTIMEBOTTOM, wxT("00:15:00"), wxPoint(571,462), wxSize(245,50), wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("editTimeBottom"));
	editTimeBottom->SetFont(wxFont(37, wxSWISS, wxNORMAL,wxBOLD, false, wxT("Courier New")));

	buttonPause = new wxButton(panelBackground, ID_BUTTONPAUSE, wxT("Pause"), wxPoint(819,446), wxSize(47,20), 0, wxDefaultValidator, wxT("buttonPause"));
	buttonPause->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonReset = new wxButton(panelBackground, ID_BUTTONRESET, wxT("Reset"), wxPoint(819,469), wxSize(47,20), 0, wxDefaultValidator, wxT("buttonReset"));
	buttonReset->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonSwap = new wxButton(panelBackground, ID_BUTTONSWAP, wxT("Swap"), wxPoint(819,492), wxSize(47,20), 0, wxDefaultValidator, wxT("buttonSwap"));
	buttonSwap->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText2 = new wxStaticText(panelBackground, ID_WXSTATICTEXT2, wxT("Chess Clock                                "), wxPoint(516,383), wxDefaultSize, 0, wxT("WxStaticText2"));
	WxStaticText2->SetForegroundColour(wxColour(192,192,192));
	WxStaticText2->SetFont(wxFont(12, wxSWISS, wxNORMAL,wxBOLD, true, wxT("Tahoma")));

	WxStaticText6 = new wxStaticText(panelBackground, ID_WXSTATICTEXT6, wxT("Board Configuration                  "), wxPoint(516,296), wxDefaultSize, 0, wxT("WxStaticText6"));
	WxStaticText6->SetForegroundColour(wxColour(192,192,192));
	WxStaticText6->SetFont(wxFont(12, wxSWISS, wxNORMAL,wxBOLD, true, wxT("Tahoma")));

	WxStaticText1 = new wxStaticText(panelBackground, ID_WXSTATICTEXT1, wxT("                              "), wxPoint(715,296), wxDefaultSize, 0, wxT("WxStaticText1"));
	WxStaticText1->SetForegroundColour(wxColour(192,192,192));
	WxStaticText1->SetFont(wxFont(12, wxSWISS, wxNORMAL,wxBOLD, true, wxT("Tahoma")));

	WxStaticText4 = new wxStaticText(panelBackground, ID_WXSTATICTEXT4, wxT("                              "), wxPoint(715,383), wxDefaultSize, 0, wxT("WxStaticText4"));
	WxStaticText4->SetForegroundColour(wxColour(192,192,192));
	WxStaticText4->SetFont(wxFont(12, wxSWISS, wxNORMAL,wxBOLD, true, wxT("Tahoma")));

	WxStaticText5 = new wxStaticText(panelBackground, ID_WXSTATICTEXT5, wxT("Network ______________"), wxPoint(516,1), wxDefaultSize, 0, wxT("WxStaticText5"));
	WxStaticText5->SetForegroundColour(wxColour(192,192,192));
	WxStaticText5->SetFont(wxFont(12, wxSWISS, wxNORMAL,wxBOLD, true, wxT("Tahoma")));

	WxStaticText7 = new wxStaticText(panelBackground, ID_WXSTATICTEXT7, wxT("                              "), wxPoint(715,1), wxDefaultSize, 0, wxT("WxStaticText7"));
	WxStaticText7->SetForegroundColour(wxColour(192,192,192));
	WxStaticText7->SetFont(wxFont(12, wxSWISS, wxNORMAL,wxBOLD, true, wxT("Tahoma")));

	dialogSave =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.*"), wxSAVE);

	timerDraw = new wxTimer();
	timerDraw->SetOwner(this, ID_TIMERDRAW);
	timerDraw->Start(10);

	dialogOpen =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.*"), wxOPEN);

	timerReleaseMouse = new wxTimer();
	timerReleaseMouse->SetOwner(this, ID_TIMERRELEASEMOUSE);

	timerClock = new wxTimer();
	timerClock->SetOwner(this, ID_TIMERCLOCK);

	timerUpdate = new wxTimer();
	timerUpdate->SetOwner(this, ID_TIMERUPDATE);

	SetTitle(wxT("chessnuts"));
	SetIcon(wxNullIcon);
	SetSize(-389,-4,875,548);
	Center();
	
	////GUI Items Creation End


    // my setup stuff
    comboSetup->SetSelection(0);
    comboSetupStyle->SetSelection(0);

     // seed the random number with the time in seconds
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    
    clockblocker_top = false;
    clockblocker_bottom = false;
    
    ms_top    = StringToMilli(editTimeTop->GetValue());
    ms_bottom = StringToMilli(editTimeTop->GetValue());
    ms_top_start    = ms_top;
    ms_bottom_start = ms_bottom;
    
    loading_chessnuts = false;
}

void chessFrame::SaveSettings()
{
    wxFile f;
    if(f.Open("settings", wxFile::write))
    {
        f.Write(hostjoin->editName->GetValue().Strip() + "\n");
        f.Write(hostjoin->editAddress  ->GetValue().Strip() + "\n");
        f.Write(hostjoin->editPort->GetValue().Strip() + "\n");
        s.sprintf("%i\n%i\n%i\n%i\n", 
                    checkBoxEditMode->GetValue(),
                    checkBoxReversed->GetValue(),
                    comboSetupStyle->GetSelection(),
                    comboSetup->GetSelection()); 
        f.Write(s);
        f.Write(board->AssembleBoardString()+"\n");
        f.Write(board->PackHighlights()+"\n");
        
        s.sprintf("%i\n%i\n%i\n%i\n%i\n%i\n",
                    options->checkBoxDebug->GetValue(),
                    options->checkBoxHands->GetValue(),
                    options->checkBoxRefreshEnabled->GetValue(),
                    options->spinMax->GetValue(),
                    options->spinSpeed->GetValue(),
                    options->checkBoxNewMethod->GetValue());
        f.Write(s);

        s.sprintf("%ld\n%ld\n%ld\n%ld\n",
                    ms_top,
                    ms_top_start,
                    ms_bottom,
                    ms_bottom_start);
        f.Write(s);
        
        s.sprintf("%i\n", options->sliderSmoothing->GetValue());
        f.Write(s);

        f.Write(board->color_white_square.GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        f.Write(board->color_black_square.GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        f.Write(board->color_white_piece .GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        f.Write(board->color_black_piece .GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        f.Write(board->color_background  .GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        f.Write(board->color_border      .GetAsString(wxC2S_CSS_SYNTAX) + "\n");

        f.Write(board->highlight_pickup ->color.GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        f.Write(board->highlight_take   ->color.GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        f.Write(board->highlight_putdown->color.GetAsString(wxC2S_CSS_SYNTAX) + "\n");
        
        s.sprintf("%i\n", options->checkBoxLoudDebug->GetValue()); f.Write(s);

        f.Close();
    } else {Log("Could not save to settings file.");}
}

void chessFrame::LoadSettings()
{
    if(!wxFile::Exists("settings")) return;
    
    long n;
    size_t size;
    
    wxTextFile f;
    if(f.Open("settings"))
    {
        size = f.GetLineCount();
        
        if(size > 0) hostjoin->editName  -> SetValue(f.GetLine(0).Strip());
        if(size > 1) hostjoin->editAddress    -> SetValue(f.GetLine(1).Strip());
        if(size > 2) hostjoin->editPort  -> SetValue(f.GetLine(2).Strip());
        if(size > 3) {f.GetLine(3).ToLong(&n); checkBoxEditMode->SetValue(n);}
        if(size > 4) {f.GetLine(4).ToLong(&n); checkBoxReversed->SetValue(n);}
        if(size > 5) {f.GetLine(5).ToLong(&n); comboSetupStyle->SetSelection((int)n);}
        if(size > 6) {f.GetLine(6).ToLong(&n); comboSetup     ->SetSelection((int)n);}
        if(size > 7)
        {
            s = f.GetLine(7).Strip();
            board->UnpackBoardString(s);
        }
        if(size > 8)
        {
            s = f.GetLine(8).Strip();
            board->UnpackHighlights(s);
        }
        
        
        if(size > 9) {f.GetLine(9).ToLong(&n);  options->checkBoxDebug->SetValue(n);}
        if(size > 10){f.GetLine(10).ToLong(&n); options->checkBoxHands->SetValue(n);}
        if(size > 11){f.GetLine(11).ToLong(&n); options->checkBoxRefreshEnabled->SetValue(n);}
        if(size > 12){f.GetLine(12).ToLong(&n); options->spinMax->SetValue((int)n);}
        if(size > 13){f.GetLine(13).ToLong(&n); options->spinSpeed->SetValue((int)n);}
        if(size > 14){f.GetLine(14).ToLong(&n); options->checkBoxNewMethod->SetValue((int)n);}
        
        if(size > 15){f.GetLine(15).ToLong(&n); ms_top = n;}
        if(size > 16){f.GetLine(16).ToLong(&n); ms_top_start = n;}
        if(size > 17){f.GetLine(17).ToLong(&n); ms_bottom = n;}
        if(size > 18){f.GetLine(18).ToLong(&n); ms_bottom_start = n;}
        SetClocks();
        
        if(size > 19){f.GetLine(19).ToLong(&n); options->sliderSmoothing->SetValue((int)n); wxScrollEvent e; options->sliderSmoothingScroll(e);}
        
        if(size > 20){board->color_white_square .Set(f.GetLine(20).Strip());}
        if(size > 21){board->color_black_square .Set(f.GetLine(21).Strip());}
        if(size > 22){board->color_white_piece  .Set(f.GetLine(22).Strip());}
        if(size > 23){board->color_black_piece  .Set(f.GetLine(23).Strip());}
        if(size > 24){board->color_background   .Set(f.GetLine(24).Strip());}
        if(size > 25){board->color_border       .Set(f.GetLine(25).Strip());}

        if(size > 26){board->highlight_pickup ->color.Set(f.GetLine(26).Strip());}
        if(size > 27){board->highlight_take   ->color.Set(f.GetLine(27).Strip());}
        if(size > 28){board->highlight_putdown->color.Set(f.GetLine(28).Strip());}

        if(size > 29){f.GetLine(29).ToLong(&n); options->checkBoxLoudDebug->SetValue(n);} 

        f.Close();
    }
    else Log("Could not open settings file.");
}



void chessFrame::OnClose(wxCloseEvent& event)
{
    Debug("chessFrame::OnClose())");
    
    SaveSettings();
    Debug("settings saved");
    
    //delete board;
    //board = NULL;

    if(client)
    {
        wxCommandEvent e;
        buttonDisconnectClick(e);
    }
    if(server)
    {
        ShutdownServer();
    }
    Destroy();
}

void chessFrame::Log(wxString line)
{
    this->memoLog->AppendText("*** "+line+"\n");
    if(options->checkBoxDebug->GetValue())
    {
        // also write the output to a debug.txt
        wxFile f;
        f.Open("debug.txt", wxFile::write_append);
        f.Write("-------\n"+line+"\n");
        f.Close();
    }
}

void chessFrame::Debug(wxString line)
{
    if(options->checkBoxDebug->GetValue())
    {
        // if we're in loud mode, show it in the log too
        if(options->checkBoxLoudDebug->GetValue()) 
            this->memoLog->AppendText("-------\n"+line+"\n");
        
        // also write the output to a debug.txt
        wxFile f;
        f.Open("debug.txt", wxFile::write_append);
        f.Write("-------\n"+line+"\n");
        f.Close();
    }
}


void chessFrame::buttonJoinClick(wxCommandEvent& event)
{
    // Pop up the join dialog, and only join if the client clicks "okay"
    hostjoin->editAddress->Enable(); // we need all the fields for joining!
    if(!hostjoin->ShowModal()) return;
    
    if(client != NULL)
    {
        Debug("client not NULL, deleting.");
        delete client;
        client = NULL;
    }
    client = new MyClient(this);
    
    DisableConnect();
    
    Log("Trying to connect to "+hostjoin->editAddress->GetValue()+"...");
	client->Connect(hostjoin->editAddress->GetValue(), hostjoin->editPort->GetValue(), "chessticle");

    if(client->connection == NULL) 
    {
        Log("Connection FAILED. FAILURE!");
        delete client;
        client = NULL;
        EnableConnect();
    }
    else
    {
        Log("Success!");
        
        // Send your name, and the server should send back the user list and board
        client->connection->Poke("name", (wxChar *)hostjoin->editName->GetValue().GetData());
    }
}

void chessFrame::buttonHostClick(wxCommandEvent& event)
{
    // get the hosting info (address is not necessary for hosting)
    hostjoin->editAddress->Disable();
    if(!hostjoin->ShowModal()) return;
    
    // delete the old server object and create a new one
    if(server != NULL)
    {
        Debug("Server object already exists. Deleting.");
        delete server;
        server = NULL;
    }
    server = new MyServer(this);
    
    // actually start hosting!
    server->Create(hostjoin->editPort->GetValue());
    Log("Server started on TCP port "+hostjoin->editPort->GetValue());

    // add the first element of the people list (you)
    listPeople->Clear();
    listPeople->Append(hostjoin->editName->GetValue(), (wxClientData *)new MyClientData(this, 0));

    // dim/enable the appropriate buttons
    DisableConnect();
}

void chessFrame::DisableConnect()
{
    buttonJoin->Disable();
    buttonHost->Disable();
    hostjoin->editAddress->Disable();
    hostjoin->editPort->Disable();
    listPeople->Enable();
    buttonDisconnect->Enable();
    
    if(client) 
    {
        options->spinSpeed->Disable();
        options->spinMax->Disable();
        options->checkBoxHands->Disable();
        options->checkBoxRefreshEnabled->Disable();
        options->checkBoxNewMethod->Disable();
    }
}

void chessFrame::EnableConnect()
{
    client = NULL;
    server = NULL;
    
    options->spinMax->Enable();
    options->spinSpeed->Enable();
    options->checkBoxHands->Enable();
    options->checkBoxRefreshEnabled->Enable();
    options->checkBoxNewMethod->Enable();
    
    buttonJoin->Enable();
    buttonHost->Enable();
    hostjoin->editAddress->Enable();
    hostjoin->editPort->Enable();
    listPeople->Disable();
    buttonDisconnect->Disable();
}

void chessFrame::editChatEnter(wxCommandEvent& event)
{
    s = "<"+hostjoin->editName->GetValue()+"> "+editChat->GetValue();

    if(client != NULL)
    {
        client->connection->Poke("chat", (wxChar *)&s[0]);
    }
    if(server != NULL)
    {
        memoLog->AppendText(s+"\n");
        server->PokeEveryone("chat", (wxChar *)&s[0]);
    }
    
	editChat->Clear();
}

void chessFrame::buttonSetupClick(wxCommandEvent& event)
{
	board->SetupBoard(comboSetup->GetSelection(), comboSetupStyle->GetSelection()); // this does a draw and an update
}

void chessFrame::UpdateErrbody()
{
    // update the server/clients
    if(server != NULL) server->UpdateErrbody();
    if(client != NULL) client->UpdateErrbody();
}

void chessFrame::timerUpdateTimer(wxTimerEvent& event)
{
    // send the coordinates of the mouse piece
    if(server) server->SendMouse();
    if(client) client->SendMouse();
}

void chessFrame::ClearClientList()
{
    Debug("ClearClientList()");
    listPeople->Clear();
}

void chessFrame::buttonDisconnectClick(wxCommandEvent& event)
{
    // if you're a client, just disconnect!
    if(client)
    {
        Debug("Disconnecting client, deleting resources...");
        delete client; // this disconnects too
        client = NULL;
        
        // now we have to delete all the entries in the user list
        ClearClientList();
        
        EnableConnect();
    }
    
    // if you're a server, disconnect the selected client or shut it down
    else if(server != NULL)
    {
        // get the selection
        int n = listPeople->GetSelection();
        
        // if a client is selected, disconnect them
        if(n > 0) server->Disconnect((unsigned int)n);
        
        // otherwise, shut down the server!
        else
        {
            ShutdownServer();
        }
    }
}

void chessFrame::ToggleEditMode()
{
    // if we're in edit mode, the draw will take care of the clearing/adding pieces
    // if we're leaving edit mode, we should clear the border
    if(!checkBoxEditMode->GetValue())
    {
        for(int n=0; n<=9; n++)
        {
            board->pieces[n][0] = NULL;
            board->pieces[n][9] = NULL;
        }   
    }

    //board->Draw();
}

void chessFrame::checkBoxEditModeClick(wxCommandEvent& event)
{
    // enable or disable edit mode
    ToggleEditMode();
    
    // now tell everyone to go to edit mode
    if(client != NULL) client->connection->SendErrthing();
    if(server != NULL) server->UpdateErrbody();
}

void chessFrame::timerReleaseMouseTimer(wxTimerEvent& event)
{
	// This timer releases the mouse from being frozen after a half second,
	// in case you want to click the same square without moving first.
	board->last_clicked_square_x = -1;
	board->last_clicked_square_y = -1;
}

void chessFrame::checkBoxReversedClick(wxCommandEvent& event)
{
	board->Reverse();
	
	// also swap the timers
    SwapClocks();
}


void chessFrame::buttonTestClick(wxCommandEvent& event)
{
    
}



void chessFrame::ShutdownServer()
{
    if(server)
    {
        // disconnect everything and clean up
        server->PokeEveryone("chat", "Server is shutting down. Seeya!");

        // loop over everyone and disconnect them
        for(unsigned int m=listPeople->GetCount()-1; m>=1; m--) server->Disconnect(m);

        Log("Shutting down server...");

        delete server;
        server = NULL;

        // clear the client list
        listPeople->Clear();

        EnableConnect();
    }
}

void chessFrame::buttonOptionsClick(wxCommandEvent& event)
{
	options->Show();
	options->Raise();
}

void chessFrame::editUserUpdated(wxCommandEvent& event)
{
	if(client) client->connection->Poke("=", (wxChar *)event.GetString().GetData());
	if(server) 
    {
        s = "=";
        server->PokeEveryone(s+(char)0, (wxChar *)event.GetString().GetData());
        listPeople->SetString(0, event.GetString());
    }
}


void chessFrame::timerDrawTimer(wxTimerEvent& event)
{
    // First thing we do is update the interpolation if necessary
    double time;

    // update all the floater coordinates
	// the current coordinate should be (time/transit time)*(target-start)+start
	MyClientData *c;
    for(unsigned int n=0; n < listPeople->GetCount(); n++)
    {
        // our floater will always be NULL
        c = (MyClientData *)listPeople->GetClientData(n);

        // don't do any processing/drawing if the floater is nothing.
        if(c->floater) 
        {
            // get the elapsed time for this floater
            time = (double)c->timeypoo.Time();
    
            // get the current coordinates
            c->x = c->x_start + (c->x_target - c->x_start)*time/(options->floaty_smooth*(double)options->spinSpeed->GetValue());
            c->y = c->y_start + (c->y_target - c->y_start)*time/(options->floaty_smooth*(double)options->spinSpeed->GetValue());
    
            // if the time is greater than the transit time, then
            // set it to the the final position, and stop the timer
            if(time >= (double)options->spinSpeed->GetValue()/0.8)
            {
                c->x = c->x_target;
                c->y = c->y_target;
            }
        }
    }

    
    // now actually draw the board
    board->Draw();
}

void chessFrame::buttonSaveClick(wxCommandEvent& event)
{
    wxFile f;
    dialogSave->SetWildcard("nuts files (*.nuts)|*.nuts");
	if(dialogSave->ShowModal() == wxID_OK)
	{
        Debug("Saving board configuration to '"+dialogSave->GetPath()+"'");
        
        // open the file
        if(f.Open(dialogSave->GetPath(), wxFile::write))
        {
            f.Write(board->AssembleBoardString()+"\n");
            f.Write(board->PackHighlights());
            
            if(checkBoxReversed->GetValue()) s.sprintf("\n%ld\n%ld", ms_bottom, ms_top);
            else                             s.sprintf("\n%ld\n%ld", ms_top, ms_bottom);
            f.Write(s);
            
            f.Close();
        }
        else Log("Could not open file for writing.");
    }
}

void chessFrame::buttonLoadClick(wxCommandEvent& event)
{
	wxTextFile f;
	dialogOpen->SetWildcard("nuts files (*.nuts)|*.nuts");
	if(dialogOpen->ShowModal() == wxID_OK)
	{
        Debug("Loading board configuration from '"+dialogOpen->GetPath()+"'");
        
        // open the file
        if(f.Open(dialogOpen->GetPath()) && f.GetLineCount() >= 2)
        {
            board->UnpackBoardString(f.GetLine(0).Strip());
            board->UnpackHighlights(f.GetLine(1).Strip());
            
            if(checkBoxReversed->GetValue())
            {
                f.GetLine(2).Strip().ToLong(&ms_bottom);
                f.GetLine(3).Strip().ToLong(&ms_top);
            }
            else
            {
                f.GetLine(2).Strip().ToLong(&ms_top);
                f.GetLine(3).Strip().ToLong(&ms_bottom);
            }
            Pause();
            SetClocks();
            
            board->UpdateErrbody();
            f.Close();
        }
        else Log("Could not open file for reading.");
    }
}

void chessFrame::toggleBottomClick(wxCommandEvent& event)
{
    // make the GUI look right
    toggleBottom->SetValue(true);
    toggleTop->SetValue(false);
    
    // set the starting point so we can keep track of elapsed time
	ms_top_last = ms_top;
	
	SendToggles();
	
	// start a stopwatch at zero
	timeypoo.Start(0);
	timerClock->Start(100);
}
void chessFrame::toggleTopClick(wxCommandEvent& event)
{
	// make the GUI look right
    toggleBottom->SetValue(false);
    toggleTop->SetValue(true);
    
    // set the starting point so we can keep track of elapsed time
	ms_bottom_last = ms_bottom;

    SendToggles();
    
	// start a stopwatch (at zero)
	timeypoo.Start(0);
    timerClock->Start(100);
}

long chessFrame::StringToMilli(wxString time)
{
    long t;
    long ms;
    int sign = 1;
    
    // first split it by ":"'s
    wxArrayString a = SplitString(time, ":");
    
    // if we only get one element, try splitting by "."
    if(a.GetCount() == 1) 
    {
        a = SplitString(time, ".");
        if(a.GetCount() > 1) sign = -1;
    }
    
    if(a.GetCount() == 1) // minutes only
    {
        // if it's a bad number, quit out
        if(a[0] == "")       a[0] = "0";
        if(!a[0].ToLong(&t)) return 0;
        // return the tenths based on minutes
        return sign*t*60*1000;
    }
    else if(a.GetCount() == 2) // minutes:seconds
    {
        if(a[0] == "")       a[0] = "0";
        if(!a[0].ToLong(&t)) return 0;
        ms = t*60*1000;
        
        if(a[1] == "")       a[1] = "0";
        if(!a[1].ToLong(&t)) return 0;
        return sign*(ms+t*1000);
    }
    else if(a.GetCount() == 3) // hours:minutes:seconds
    {
        if(a[0] == "")       a[0] = "0";
        if(!a[0].ToLong(&t)) return 0;
        ms = t*60*60*1000;
        
        if(a[1] == "")       a[1] = "0";
        if(!a[1].ToLong(&t)) return 0;
        ms += t*60*1000;
        
        if(a[2] == "")       a[2] = "0";
        if(!a[2].ToLong(&t)) return 0;
        return sign*(ms + t*1000);
    }
    
    // no good format
    return 0;
}

wxString chessFrame::MilliToString(long ms)
{
    int hours   = ms / (1000*60*60);
    int minutes = (ms-hours*1000*60*60) / (1000*60);
    int seconds = (ms-hours*1000*60*60-minutes*1000*60) / 1000;
    
    if(ms > 0) s.sprintf("%02i:%02i:%02i", hours, minutes, seconds);
    else       s.sprintf("%02i.%02i.%02i", -hours, -minutes, -seconds);
    
    return s;
}

void chessFrame::editTimeBottomUpdated(wxCommandEvent& event)
{
	if(loading_chessnuts) return;
    
    // tell everyone what we're doing
    if(clockblocker_bottom) clockblocker_bottom = false;
    else
    {
        // if clockblocker is true, it means the program is setting the timer, not us
        
        // convert the string into milliseconds
        ms_bottom = StringToMilli(editTimeBottom->GetValue());
        ms_bottom_start = ms_bottom;

        // make sure the timer stops since we're editing stuff
        timerClock->Stop();
        Poke("ClockStop", "");
        if(checkBoxReversed->GetValue()) Poke("ClockTop",    (wxChar *)editTimeBottom->GetValue().GetData());
        else                             Poke("ClockBottom", (wxChar *)editTimeBottom->GetValue().GetData());
    }
    
}

void chessFrame::editTimeTopUpdated(wxCommandEvent& event)
{
	if(loading_chessnuts) return;

    // tell everyone what we're doing
    if(clockblocker_top) clockblocker_top = false;
    else
    {
        // convert the string into ms
        ms_top = StringToMilli(editTimeTop->GetValue());
        ms_top_start = ms_top;

        // make sure the timer stops since we're editing stuff
        timerClock->Stop();
        Poke("ClockStop", "");
        if(checkBoxReversed->GetValue()) Poke("ClockBottom", (wxChar *)editTimeTop->GetValue().GetData());
        else                             Poke("ClockTop",    (wxChar *)editTimeTop->GetValue().GetData());
    }
}

void chessFrame::Poke(const wxString &item, wxChar *data, int size, wxIPCFormat format)
{
    Debug("chessFrame::Poke()");
    if     (client) client->connection->Poke(item, data, size, format);
    else if(server) server->    PokeEveryone(item, data, size, format);
}

void chessFrame::timerClockTimer(wxTimerEvent& event)
{
	// get the current timer in ms
	long t = timeypoo.Time();
	
	// subtract this value from the appropriate ms
	if(toggleBottom->GetValue())   ms_top = ms_top_last - t;
    else if(toggleTop->GetValue()) ms_bottom = ms_bottom_last - t;
    else                           Log("Timer shouldn't be running, but it is!");

    SetClocks();
}

void chessFrame::Pause()
{
    // unpush the buttons
    toggleTop->SetValue(false);
    toggleBottom->SetValue(false);

    // stop the clock
	timerClock->Stop();
}

void chessFrame::SetClocks()
{
    // update the GUI
    clockblocker_top = true;
    clockblocker_bottom = true;
    editTimeTop   ->SetValue(MilliToString(ms_top));
    editTimeBottom->SetValue(MilliToString(ms_bottom));
    
    // update colors depending on +/-
    wxColour white(255,255,255);
    wxColour red  (255,200,200);
    
    if(ms_bottom > 0) editTimeBottom->SetBackgroundColour(white);
    else              editTimeBottom->SetBackgroundColour(red);
    if(ms_top    > 0) editTimeTop   ->SetBackgroundColour(white);
    else              editTimeTop   ->SetBackgroundColour(red);      
}

void chessFrame::SendTimes()
{
    // send times
	long times[2];
	if(checkBoxReversed->GetValue())
    {
        times[1] = ms_top;
	    times[0] = ms_bottom;
    }
    else
    {
        times[0] = ms_top;
	    times[1] = ms_bottom;
    }
    Poke("Clocks", (char *)times, 2*sizeof(long));
}

void chessFrame::SendToggles()
{
    long times[2];
    
    // send a toggle event, based on which toggle is pressed
    if(toggleBottom->GetValue())
    {
        if(checkBoxReversed->GetValue())
        {
            times[1] = ms_top;
    	    times[0] = ms_bottom;
            Poke("ToggleTop",    (wxChar *)times, sizeof(long)*2);
        }
        else
        {
            times[0] = ms_top;
    	    times[1] = ms_bottom;
            Poke("ToggleBottom", (wxChar *)times, sizeof(long)*2);
        }
    }
    else if(toggleTop->GetValue())
    {
    
    	if(checkBoxReversed->GetValue())
        {
            times[1] = ms_top;
    	    times[0] = ms_bottom;
            Poke("ToggleBottom", (wxChar *)times, sizeof(long)*2);
        }
        else
        {
            times[0] = ms_top;
    	    times[1] = ms_bottom;
            Poke("ToggleTop", (wxChar *)times, sizeof(long)*2);
        }
    }
    else // no toggles are pressed!
    {
        // make sure we're paused
        Pause();
        
        // send a clock stop so the buttons are unpressed and the clock is stopped
        Poke("ClockStop", "");
        
        // send the times
        SendTimes();
    }
}

void chessFrame::buttonPauseClick(wxCommandEvent& event)
{
    Pause();
    Poke("ClockStop", "");
    SetClocks();
    SendTimes();
}

void chessFrame::buttonResetClick(wxCommandEvent& event)
{
	Pause();
	Poke("ClockStop", "");
	
	// se the times to the last manually entered values
	ms_top    = ms_top_start;
	ms_bottom = ms_bottom_start;
	
	// update the GUI
    SetClocks();
    SendTimes();
}

void chessFrame::SwapClocks()
{
    s = editTimeBottom->GetValue();
    long ms       = ms_bottom;
    long ms_last  = ms_bottom_last;
    long ms_start = ms_bottom_start;
    bool toggle   = toggleBottom->GetValue();

    ms_bottom       = ms_top;
    ms_bottom_last  = ms_top_last;
    ms_bottom_start = ms_top_start;
    toggleBottom->SetValue(toggleTop->GetValue());
    clockblocker_bottom = true;
    editTimeBottom->SetValue(editTimeTop->GetValue());

    ms_top       = ms;
    ms_top_last  = ms_last;
    ms_top_start = ms_start;
    toggleTop->SetValue(toggle);
    clockblocker_top = true;
    editTimeTop->SetValue(s);
}

void chessFrame::buttonSwapClick(wxCommandEvent& event)
{
	SwapClocks();
	
    // also update everyone
	SendToggles();
}

void chessFrame::editTimeTopEnter(wxCommandEvent& event)
{
    // convert the string into ms
    ms_top = StringToMilli(editTimeTop->GetValue());
    ms_top_start = ms_top;

    wxCommandEvent e;
	buttonPauseClick(e);
}

void chessFrame::editTimeBottomEnter(wxCommandEvent& event)
{
    // convert the string into milliseconds
    ms_bottom = StringToMilli(editTimeBottom->GetValue());
    ms_bottom_start = ms_bottom;

    wxCommandEvent e;
    buttonPauseClick(e);
}

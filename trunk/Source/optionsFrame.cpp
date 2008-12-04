//---------------------------------------------------------------------------
//
// Name:        optionsFrame.cpp
// Author:      gurkesaft
// Created:     8/26/2007 1:04:57 PM
// Description: optionsFrame class implementation
//
//---------------------------------------------------------------------------

#include "optionsFrame.h"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// optionsFrame
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(optionsFrame,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(optionsFrame::OnClose)
	EVT_TIMER(ID_TIMERFLOATYCOUNT,optionsFrame::timerFloatyCountTimer)
	EVT_BUTTON(ID_BUTTONCOLORPICKUP,optionsFrame::buttonColorPickupClick)
	EVT_BUTTON(ID_BUTTONCOLORPUTDOWN,optionsFrame::buttonColorPutdownClick)
	EVT_BUTTON(ID_BUTTONCOLORTAKE,optionsFrame::buttonColorTakeClick)
	EVT_BUTTON(ID_BUTTONDEFAULTCOLORS,optionsFrame::buttonDefaultColorsClick)
	EVT_BUTTON(ID_BUTTONCOLORBLACKPIECE,optionsFrame::buttonColorBlackPieceClick)
	EVT_BUTTON(ID_BUTTONCOLORWHITEPIECE,optionsFrame::buttonColorWhitePieceClick)
	EVT_BUTTON(ID_BUTTONCOLORBLACK,optionsFrame::buttonColorBlackClick)
	EVT_BUTTON(ID_BUTTONCOLORWHITE,optionsFrame::buttonColorWhiteClick)
	EVT_BUTTON(ID_BUTTONCOLORBORDER,optionsFrame::buttonColorBorderClick)
	EVT_BUTTON(ID_BUTTONCOLORTABLE,optionsFrame::buttonColorTableClick)
	
	EVT_COMMAND_SCROLL(ID_SLIDERSMOOTHING,optionsFrame::sliderSmoothingScroll)
	EVT_CHECKBOX(ID_CHECKBOXNEWMETHOD,optionsFrame::checkBoxNewMethodClick)
	EVT_CHECKBOX(ID_CHECKBOXFPS,optionsFrame::checkBoxFPSClick)
	EVT_CHECKBOX(ID_CHECKBOXFLOATIESPERSECOND,optionsFrame::checkBoxFloatiesPerSecondClick)
	EVT_CHECKBOX(ID_CHECKBOXHANDS,optionsFrame::checkBoxHandsClick)
	EVT_CHECKBOX(ID_CHECKBOXREFRESHENABLED,optionsFrame::checkBoxRefreshEnabledClick)
	EVT_SPINCTRL(ID_SPINSPEED,optionsFrame::spinSpeedUpdated)
	EVT_SPINCTRL(ID_SPINMAX,optionsFrame::spinMaxUpdated)
END_EVENT_TABLE()
////Event Table End

optionsFrame::optionsFrame(chessFrame *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
    chess = parent;
	CreateGUIControls();
}

optionsFrame::~optionsFrame()
{
}

void optionsFrame::CreateGUIControls()
{
	loading_frame = true;
	
    //Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(0,0), wxSize(295,363));
	WxPanel1->SetBackgroundColour(wxColour(244,244,244));
	WxPanel1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	spinMax = new wxSpinCtrl(WxPanel1, ID_SPINMAX, wxT("7"), wxPoint(136,21), wxSize(57,22), wxSP_ARROW_KEYS, 1, 64, 7);
	spinMax->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText1 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT1, wxT("Max Connections"), wxPoint(49,25), wxDefaultSize, 0, wxT("WxStaticText1"));
	WxStaticText1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText2 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT2, wxT("Server Options:"), wxPoint(4,4), wxDefaultSize, 0, wxT("WxStaticText2"));
	WxStaticText2->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxBOLD, false, wxT("Tahoma")));

	spinSpeed = new wxSpinCtrl(WxPanel1, ID_SPINSPEED, wxT("150"), wxPoint(136,46), wxSize(57,22), wxSP_ARROW_KEYS, 1, 1000, 150);
	spinSpeed->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText3 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT3, wxT("Floaties Refresh Rate (ms)"), wxPoint(4,49), wxDefaultSize, 0, wxT("WxStaticText3"));
	WxStaticText3->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	checkBoxRefreshEnabled = new wxCheckBox(WxPanel1, ID_CHECKBOXREFRESHENABLED, wxT("Floaties Enabled"), wxPoint(197,49), wxSize(112,17), 0, wxDefaultValidator, wxT("checkBoxRefreshEnabled"));
	checkBoxRefreshEnabled->SetValue(true);
	checkBoxRefreshEnabled->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	checkBoxHands = new wxCheckBox(WxPanel1, ID_CHECKBOXHANDS, wxT("Hands"), wxPoint(233,9), wxSize(51,17), 0, wxDefaultValidator, wxT("checkBoxHands"));
	checkBoxHands->Show(false);
	checkBoxHands->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText5 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT5, wxT("General Options:"), wxPoint(4,104), wxDefaultSize, 0, wxT("WxStaticText5"));
	WxStaticText5->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxBOLD, false, wxT("Tahoma")));

	checkBoxDebug = new wxCheckBox(WxPanel1, ID_CHECKBOXDEBUG, wxT("Debug"), wxPoint(4,308), wxSize(51,17), 0, wxDefaultValidator, wxT("checkBoxDebug"));
	checkBoxDebug->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	checkBoxFloatiesPerSecond = new wxCheckBox(WxPanel1, ID_CHECKBOXFLOATIESPERSECOND, wxT("Floaties Per Second"), wxPoint(4,327), wxSize(159,17), 0, wxDefaultValidator, wxT("checkBoxFloatiesPerSecond"));
	checkBoxFloatiesPerSecond->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	checkBoxFPS = new wxCheckBox(WxPanel1, ID_CHECKBOXFPS, wxT("Frames Per Second"), wxPoint(4,346), wxSize(172,17), 0, wxDefaultValidator, wxT("checkBoxFPS"));
	checkBoxFPS->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	checkBoxNewMethod = new wxCheckBox(WxPanel1, ID_CHECKBOXNEWMETHOD, wxT("New Update Method (send piece, not whole board)"), wxPoint(4,71), wxSize(275,17), 0, wxDefaultValidator, wxT("checkBoxNewMethod"));
	checkBoxNewMethod->SetValue(true);
	checkBoxNewMethod->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	sliderSmoothing = new wxSlider(WxPanel1, ID_SLIDERSMOOTHING, 0, 0, 20, wxPoint(89,121), wxSize(145,28), wxSL_HORIZONTAL | wxSL_SELRANGE , wxDefaultValidator, wxT("sliderSmoothing"));
	sliderSmoothing->SetRange(0,20);
	sliderSmoothing->SetValue(0);
	sliderSmoothing->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText4 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT4, wxT("Floaty Smoothing"), wxPoint(4,124), wxDefaultSize, 0, wxT("WxStaticText4"));
	WxStaticText4->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText6 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT6, wxT("min"), wxPoint(92,148), wxDefaultSize, 0, wxT("WxStaticText6"));
	WxStaticText6->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText7 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT7, wxT("max"), wxPoint(213,148), wxDefaultSize, 0, wxT("WxStaticText7"));
	WxStaticText7->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	textSmooth = new wxStaticText(WxPanel1, ID_TEXTSMOOTH, wxT("textSmooth"), wxPoint(235,125), wxDefaultSize, 0, wxT("textSmooth"));
	textSmooth->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText8 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT8, wxT("Board Colors"), wxPoint(4,170), wxDefaultSize, 0, wxT("WxStaticText8"));
	WxStaticText8->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorTable = new wxButton(WxPanel1, ID_BUTTONCOLORTABLE, wxT(""), wxPoint(187,186), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorTable"));
	buttonColorTable->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorBorder = new wxButton(WxPanel1, ID_BUTTONCOLORBORDER, wxT(""), wxPoint(187,207), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorBorder"));
	buttonColorBorder->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorWhite = new wxButton(WxPanel1, ID_BUTTONCOLORWHITE, wxT(""), wxPoint(3,186), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorWhite"));
	buttonColorWhite->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorBlack = new wxButton(WxPanel1, ID_BUTTONCOLORBLACK, wxT(""), wxPoint(3,207), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorBlack"));
	buttonColorBlack->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText9 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT9, wxT("Table"), wxPoint(208,189), wxDefaultSize, 0, wxT("WxStaticText9"));
	WxStaticText9->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText10 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT10, wxT("Border"), wxPoint(208,210), wxDefaultSize, 0, wxT("WxStaticText10"));
	WxStaticText10->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText11 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT11, wxT("White Square"), wxPoint(24,189), wxDefaultSize, 0, wxT("WxStaticText11"));
	WxStaticText11->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText12 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT12, wxT("Black Square"), wxPoint(24,210), wxDefaultSize, 0, wxT("WxStaticText12"));
	WxStaticText12->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorWhitePiece = new wxButton(WxPanel1, ID_BUTTONCOLORWHITEPIECE, wxT(""), wxPoint(99,186), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorWhitePiece"));
	buttonColorWhitePiece->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText13 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT13, wxT("White Piece"), wxPoint(120,189), wxDefaultSize, 0, wxT("WxStaticText13"));
	WxStaticText13->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorBlackPiece = new wxButton(WxPanel1, ID_BUTTONCOLORBLACKPIECE, wxT(""), wxPoint(99,207), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorBlackPiece"));
	buttonColorBlackPiece->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText14 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT14, wxT("Black Piece"), wxPoint(120,210), wxDefaultSize, 0, wxT("WxStaticText14"));
	WxStaticText14->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonDefaultColors = new wxButton(WxPanel1, ID_BUTTONDEFAULTCOLORS, wxT("Default Colors"), wxPoint(3,275), wxSize(280,20), 0, wxDefaultValidator, wxT("buttonDefaultColors"));
	buttonDefaultColors->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText15 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT15, wxT("Pick Up Piece"), wxPoint(24,252), wxDefaultSize, 0, wxT("WxStaticText15"));
	WxStaticText15->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorTake = new wxButton(WxPanel1, ID_BUTTONCOLORTAKE, wxT(""), wxPoint(99,249), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorTake"));
	buttonColorTake->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText16 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT16, wxT("Take Piece"), wxPoint(120,252), wxDefaultSize, 0, wxT("WxStaticText16"));
	WxStaticText16->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorPutdown = new wxButton(WxPanel1, ID_BUTTONCOLORPUTDOWN, wxT(""), wxPoint(187,249), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorPutdown"));
	buttonColorPutdown->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText17 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT17, wxT("Put Down Piece"), wxPoint(208,252), wxDefaultSize, 0, wxT("WxStaticText17"));
	WxStaticText17->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText18 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT18, wxT("Cursor Colors"), wxPoint(4,234), wxDefaultSize, 0, wxT("WxStaticText18"));
	WxStaticText18->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonColorPickup = new wxButton(WxPanel1, ID_BUTTONCOLORPICKUP, wxT(""), wxPoint(3,249), wxSize(20,20), 0, wxDefaultValidator, wxT("buttonColorPickup"));
	buttonColorPickup->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	timerFloatyCount = new wxTimer();
	timerFloatyCount->SetOwner(this, ID_TIMERFLOATYCOUNT);

	colorDialog =  new wxColourDialog(this);

	checkBoxLoudDebug = new wxCheckBox(WxPanel1, ID_CHECKBOXLOUDDEBUG, wxT("Annoying Mode"), wxPoint(57,308), wxSize(91,17), 0, wxDefaultValidator, wxT("checkBoxLoudDebug"));
	checkBoxLoudDebug->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	SetTitle(wxT("Options"));
	SetIcon(wxNullIcon);
	SetSize(8,8,303,397);
	Center();
	
	////GUI Items Creation End
	
	loading_frame = false;
	
	// set the smoothing initial value
	sliderSmoothing->SetValue(3);
	wxScrollEvent e;
    sliderSmoothingScroll(e);
}

void optionsFrame::OnClose(wxCloseEvent& event)
{
	this->Hide();
}

void optionsFrame::spinSpeedUpdated(wxSpinEvent& event)
{
    if(loading_frame) return; 
    
    // update the floaty_smooth text
    wxScrollEvent e;
    sliderSmoothingScroll(e);
    
    int n = event.GetInt();
	if(chess->server) chess->server->PokeEveryone("Speed", (char *)&n, sizeof(int));
}

void optionsFrame::spinMaxUpdated(wxSpinEvent& event )
{
	int n = event.GetInt();
	if(chess->server) chess->server->PokeEveryone("Max", (char *)&n, sizeof(int));
}

void optionsFrame::checkBoxRefreshEnabledClick(wxCommandEvent& event)
{
    if(chess->server)
    {
	   if(checkBoxRefreshEnabled->GetValue()) chess->server->PokeEveryone("EnableRefresh", "");
	   else                                   chess->server->PokeEveryone("DisableRefresh", "");
    }
}

void optionsFrame::checkBoxHandsClick(wxCommandEvent& event)
{
	if(chess->server)
    {
	   if(checkBoxHands->GetValue()) chess->server->PokeEveryone("Hands", "");
	   else                          chess->server->PokeEveryone("NoHands", "");
    }
}

void optionsFrame::checkBoxNewMethodClick(wxCommandEvent& event)
{
	if(chess->server)
	{
        if(checkBoxNewMethod->GetValue()) chess->server->PokeEveryone("NewMethod","");
        else                              chess->server->PokeEveryone("OldMethod","");
    }
}



void optionsFrame::checkBoxFloatiesPerSecondClick(wxCommandEvent& event)
{
	if(event.GetInt()) 
    {
        floaty_count = 0;
        timerFloatyCount->Start(1000);
    }
	else timerFloatyCount->Stop();
}

void optionsFrame::timerFloatyCountTimer(wxTimerEvent& event)
{
	if(checkBoxFloatiesPerSecond->GetValue())
	{
       s.sprintf("Floaties Per Second: %i", floaty_count);
	   checkBoxFloatiesPerSecond->SetLabel(s);
	   floaty_count = 0;
    }
    if(checkBoxFPS->GetValue())
    {
        s.sprintf("Frames Per Second: %i", frame_count);
        checkBoxFPS->SetLabel(s);
        frame_count = 0;
    }
}

void optionsFrame::checkBoxFPSClick(wxCommandEvent& event)
{
    if(event.GetInt())
    {
        frame_count = 0;
        timerFloatyCount->Start(1000);
    }
    else timerFloatyCount->Stop();
}

void optionsFrame::sliderSmoothingScroll(wxScrollEvent& event)
{
	// get the value from the slider and map it onto 1.0-3.0
	floaty_smooth = 1.0+(float)sliderSmoothing->GetValue()*2.0/(float)sliderSmoothing->GetMax();
	
	s.sprintf("%.0f ms", floaty_smooth*spinSpeed->GetValue());
	textSmooth->SetLabel(s);
}

void optionsFrame::UpdateColorButtons()
{
    buttonColorTable  ->SetBackgroundColour(chess->board->color_background);
    buttonColorBorder ->SetBackgroundColour(chess->board->color_border);
    buttonColorWhite  ->SetBackgroundColour(chess->board->color_white_square);
    buttonColorBlack  ->SetBackgroundColour(chess->board->color_black_square);
    buttonColorWhitePiece ->SetBackgroundColour(chess->board->color_white_piece);
    buttonColorBlackPiece ->SetBackgroundColour(chess->board->color_black_piece);
    
    buttonColorPickup  ->SetBackgroundColour(chess->board->highlight_pickup ->color);
    buttonColorTake    ->SetBackgroundColour(chess->board->highlight_take   ->color);
    buttonColorPutdown ->SetBackgroundColour(chess->board->highlight_putdown->color);
    
    // set the piece colors
    chess->board->white_king   ->color = chess->board->color_white_piece;
    chess->board->white_queen  ->color = chess->board->color_white_piece;
    chess->board->white_bishop ->color = chess->board->color_white_piece;
    chess->board->white_knight ->color = chess->board->color_white_piece;
    chess->board->white_rook   ->color = chess->board->color_white_piece;
    chess->board->white_pawn   ->color = chess->board->color_white_piece;

    chess->board->black_king   ->color = chess->board->color_black_piece;
    chess->board->black_queen  ->color = chess->board->color_black_piece;
    chess->board->black_bishop ->color = chess->board->color_black_piece;
    chess->board->black_knight ->color = chess->board->color_black_piece;
    chess->board->black_rook   ->color = chess->board->color_black_piece;
    chess->board->black_pawn   ->color = chess->board->color_black_piece;
}

void optionsFrame::buttonColorWhiteClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->color_white_square);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->color_white_square = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}
void optionsFrame::buttonColorBlackClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->color_black_square);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->color_black_square = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

void optionsFrame::buttonColorTableClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->color_background);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->color_background = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

void optionsFrame::buttonColorBorderClick(wxCommandEvent& event)
{   
    colorDialog->GetColourData().SetColour(chess->board->color_border);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->color_border = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

void optionsFrame::buttonColorWhitePieceClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->color_white_piece);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->color_white_piece = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

void optionsFrame::buttonColorBlackPieceClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->color_black_piece);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->color_black_piece = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

void optionsFrame::buttonDefaultColorsClick(wxCommandEvent& event)
{
	chess->board->SetDefaultColors();
	UpdateColorButtons();
}

void optionsFrame::buttonColorPickupClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->highlight_pickup->color);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->highlight_pickup->color = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

void optionsFrame::buttonColorTakeClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->highlight_take->color);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->highlight_take->color = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

void optionsFrame::buttonColorPutdownClick(wxCommandEvent& event)
{
    colorDialog->GetColourData().SetColour(chess->board->highlight_putdown->color);
	if(colorDialog->ShowModal() == wxID_OK)
	{
        chess->board->highlight_putdown->color = colorDialog->GetColourData().GetColour();
	    UpdateColorButtons();
    }
}

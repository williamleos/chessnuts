//---------------------------------------------------------------------------
//
// Name:        HostJoinDialog.cpp
// Author:      gurkesaft
// Created:     12/30/2007 12:15:54 PM
// Description: HostJoinDialog class implementation
//
//---------------------------------------------------------------------------

#include "HostJoinDialog.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// HostJoinDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(HostJoinDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(HostJoinDialog::OnClose)
	EVT_BUTTON(ID_BUTTONCANCEL,HostJoinDialog::buttonCancelClick)
	EVT_BUTTON(ID_BUTTONGOFORIT,HostJoinDialog::buttonGoForItClick)
END_EVENT_TABLE()
////Event Table End

HostJoinDialog::HostJoinDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

HostJoinDialog::~HostJoinDialog()
{
} 

void HostJoinDialog::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(0,0), wxSize(227,100));
	WxPanel1->SetBackgroundColour(wxColour(244,244,244));
	WxPanel1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	editName = new wxTextCtrl(WxPanel1, ID_EDITNAME, wxT("Donny Dumbname"), wxPoint(87,4), wxSize(138,19), 0, wxDefaultValidator, wxT("editName"));
	editName->SetMaxLength(20);
	editName->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText3 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT3, wxT("Name"), wxPoint(56,7), wxDefaultSize, 0, wxT("WxStaticText3"));
	WxStaticText3->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	editAddress = new wxTextCtrl(WxPanel1, ID_EDITADDRESS, wxT("localhost"), wxPoint(87,27), wxSize(138,19), 0, wxDefaultValidator, wxT("editAddress"));
	editAddress->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText5 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT5, wxT("Remote Address"), wxPoint(4,30), wxDefaultSize, 0, wxT("WxStaticText5"));
	WxStaticText5->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	editPort = new wxTextCtrl(WxPanel1, ID_EDITPORT, wxT("17777"), wxPoint(87,50), wxSize(43,19), 0, wxDefaultValidator, wxT("editPort"));
	editPort->SetMaxLength(5);
	editPort->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	WxStaticText6 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT6, wxT("TCP/IP Port"), wxPoint(27,53), wxDefaultSize, 0, wxT("WxStaticText6"));
	WxStaticText6->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonGoForIt = new wxButton(WxPanel1, ID_BUTTONGOFORIT, wxT("Go for it!"), wxPoint(166,78), wxSize(60,21), 0, wxDefaultValidator, wxT("buttonGoForIt"));
	buttonGoForIt->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	buttonCancel = new wxButton(WxPanel1, ID_BUTTONCANCEL, wxT("Cancel"), wxPoint(104,78), wxSize(60,21), 0, wxDefaultValidator, wxT("buttonCancel"));
	buttonCancel->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	SetTitle(wxT("Network Setup"));
	SetIcon(wxNullIcon);
	SetSize(65,26,235,134);
	Center();
	
	////GUI Items Creation End
}

void HostJoinDialog::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

/*
 * buttonGoForItClick
 */
void HostJoinDialog::buttonGoForItClick(wxCommandEvent& event)
{
	// This returns a 1, for "keep at it tiger" in the ShowModal() command
	this->SetReturnCode(1);
	this->Hide();
}

/*
 * buttonCancelClick
 */
void HostJoinDialog::buttonCancelClick(wxCommandEvent& event)
{
	this->SetReturnCode(0);
	this->Hide();
}

//---------------------------------------------------------------------------
//
// Name:        chessFrame.h
// Author:      gurkesaft
// Created:     6/23/2007 3:18:51 PM
// Description: chessFrame class declaration
//
//---------------------------------------------------------------------------



#ifndef __CHESSFRAME_h__
#define __CHESSFRAME_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

#include <windows.h>
#include <gl\glu.h> /* GLU extention library */

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/timer.h>
#include <wx/filedlg.h>
#include <wx/tglbtn.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
////Header Include End

#include <vector>

#include "MyTCP.h"
class MyConnection;
class MyClient;
class MyServer;

#include "chessPiece.h"
#include "MyGLCanvas.h"
#include "optionsFrame.h"
#include "standalones.h"
#include "HostJoinDialog.h"
class HostJoinDialog;
class MyGLCanvas;
class optionsFrame;

////Dialog Style Start
#undef chessFrame_STYLE
#define chessFrame_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End


class chessFrame : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		chessFrame(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("chess"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = chessFrame_STYLE);
		virtual ~chessFrame();
		
		// Various functions
		void Log(wxString line);
		void Debug(wxString line);
		void ShutdownServer();
		void SaveSettings();
        void LoadSettings();
        void SendEditMode();
		
		wxString MilliToString(long ms);
		long StringToMilli(wxString s);
		
		void Pause();
		void SendToggles();
		void SendTimes();
		void SetClocks();
		void SwapClocks();
		
    public:
        MyGLCanvas *board;
        MyClient *client;
        MyServer *server;
        optionsFrame *options;
        HostJoinDialog *hostjoin;
        wxString s;
        
        bool loading_chessnuts;
        bool clockblocker_top, clockblocker_bottom;
        
        wxStopWatch timeypoo;
        
        long ms_top,    ms_top_last,    ms_top_start;
        long ms_bottom, ms_bottom_last, ms_bottom_start;
        
        
	public:
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxTimer *timerUpdate;
		wxTimer *timerClock;
		wxTimer *timerReleaseMouse;
		wxFileDialog *dialogOpen;
		wxTimer *timerDraw;
		wxFileDialog *dialogSave;
		wxStaticText *WxStaticText7;
		wxStaticText *WxStaticText5;
		wxStaticText *WxStaticText4;
		wxStaticText *WxStaticText1;
		wxStaticText *WxStaticText6;
		wxStaticText *WxStaticText2;
		wxButton *buttonSwap;
		wxButton *buttonReset;
		wxButton *buttonPause;
		wxTextCtrl *editTimeBottom;
		wxTextCtrl *editTimeTop;
		wxToggleButton *toggleTop;
		wxToggleButton *toggleBottom;
		wxButton *buttonLoad;
		wxButton *buttonSave;
		wxButton *buttonOptions;
		wxButton *buttonTest;
		wxComboBox *comboSetupStyle;
		wxCheckBox *checkBoxReversed;
		wxCheckBox *checkBoxEditMode;
		wxComboBox *comboSetup;
		wxButton *buttonDisconnect;
		wxListBox *listPeople;
		wxStaticText *WxStaticText3;
		wxButton *buttonSetup;
		wxTextCtrl *editChat;
		wxButton *buttonHost;
		wxButton *buttonJoin;
		wxTextCtrl *memoLog;
		wxPanel *panelBoard;
		wxPanel *panelBackground;
		////GUI Control Declaration End
		void buttonJoinClick(wxCommandEvent& event);
		void buttonHostClick(wxCommandEvent& event);
		void DisableConnect();
		void EnableConnect();
		void editChatEnter(wxCommandEvent& event);
		void buttonSetupClick(wxCommandEvent& event);
		void UpdateErrbody(); // sends all the board coordinates to everyone
		void timerUpdateTimer(wxTimerEvent& event);
		void buttonDisconnectClick(wxCommandEvent& event);
		void checkBoxEditModeClick(wxCommandEvent& event);
		void ToggleEditMode();
		void timerReleaseMouseTimer(wxTimerEvent& event);
		void checkBoxReversedClick(wxCommandEvent& event);
		void buttonTestClick(wxCommandEvent& event);
		void ClearClientList();
		void buttonOptionsClick(wxCommandEvent& event);
		void editUserUpdated(wxCommandEvent& event);
		void timerDrawTimer(wxTimerEvent& event);
		void buttonSaveClick(wxCommandEvent& event);
		void buttonLoadClick(wxCommandEvent& event);
		void toggleBottomClick(wxCommandEvent& event);
		void toggleTopClick(wxCommandEvent& event);
		void editTimeBottomUpdated(wxCommandEvent& event);
		
		void Poke(const wxString &item, wxChar *data, int size = -1, wxIPCFormat format = wxIPC_TEXT);
		void editTimeTopUpdated(wxCommandEvent& event);
		void timerClockTimer(wxTimerEvent& event);
		void buttonPauseClick(wxCommandEvent& event);
		void buttonResetClick(wxCommandEvent& event);
		void buttonSwapClick(wxCommandEvent& event);
		void editTimeTopEnter(wxCommandEvent& event);
		void editTimeBottomEnter(wxCommandEvent& event);
		
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_TIMERUPDATE = 1019,
			ID_TIMERCLOCK = 1061,
			ID_TIMERRELEASEMOUSE = 1030,
			ID_TIMERDRAW = 1048,
			ID_WXSTATICTEXT7 = 1079,
			ID_WXSTATICTEXT5 = 1078,
			ID_WXSTATICTEXT4 = 1076,
			ID_WXSTATICTEXT1 = 1075,
			ID_WXSTATICTEXT6 = 1074,
			ID_WXSTATICTEXT2 = 1072,
			ID_BUTTONSWAP = 1062,
			ID_BUTTONRESET = 1060,
			ID_BUTTONPAUSE = 1059,
			ID_EDITTIMEBOTTOM = 1057,
			ID_EDITTIMETOP = 1056,
			ID_TOGGLETOP = 1055,
			ID_TOGGLEBOTTOM = 1054,
			ID_BUTTONLOAD = 1052,
			ID_BUTTONSAVE = 1051,
			ID_BUTTONOPTIONS = 1046,
			ID_BUTTONTEST = 1034,
			ID_COMBOSETUPSTYLE = 1032,
			ID_CHECKBOXREVERSED = 1031,
			ID_CHECKBOXEDITMODE = 1028,
			ID_COMBOSETUP = 1024,
			ID_BUTTONDISCONNECT = 1021,
			ID_LISTPEOPLE = 1020,
			ID_WXSTATICTEXT3 = 1018,
			ID_BUTTONSETUP = 1017,
			ID_EDITCHAT = 1015,
			ID_BUTTONHOST = 1014,
			ID_BUTTONJOIN = 1013,
			ID_MEMOLOG = 1003,
			ID_PANELBOARD = 1002,
			ID_PANELBACKGROUND = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif

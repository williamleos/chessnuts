//---------------------------------------------------------------------------
//
// Name:        optionsFrame.h
// Author:      gurkesaft
// Created:     8/26/2007 1:04:57 PM
// Description: optionsFrame class declaration
//
//---------------------------------------------------------------------------

#ifndef __OPTIONSFRAME_h__
#define __OPTIONSFRAME_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/colordlg.h>
#include <wx/timer.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/panel.h>
////Header Include End


#include "chessFrame.h"
class chessFrame;


////Dialog Style Start
#undef optionsFrame_STYLE
#define optionsFrame_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class optionsFrame : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		optionsFrame(chessFrame *parent, wxWindowID id = 1, const wxString &title = wxT("Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = optionsFrame_STYLE);
		virtual ~optionsFrame();
		
		chessFrame *chess;
		
		void spinSpeedUpdated(wxSpinEvent& event);
		
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxCheckBox *checkBoxLoudDebug;
		wxColourDialog *colorDialog;
		wxTimer *timerFloatyCount;
		wxButton *buttonColorPickup;
		wxStaticText *WxStaticText18;
		wxStaticText *WxStaticText17;
		wxButton *buttonColorPutdown;
		wxStaticText *WxStaticText16;
		wxButton *buttonColorTake;
		wxStaticText *WxStaticText15;
		wxButton *buttonDefaultColors;
		wxStaticText *WxStaticText14;
		wxButton *buttonColorBlackPiece;
		wxStaticText *WxStaticText13;
		wxButton *buttonColorWhitePiece;
		wxStaticText *WxStaticText12;
		wxStaticText *WxStaticText11;
		wxStaticText *WxStaticText10;
		wxStaticText *WxStaticText9;
		wxButton *buttonColorBlack;
		wxButton *buttonColorWhite;
		wxButton *buttonColorBorder;
		wxButton *buttonColorTable;
		wxStaticText *WxStaticText8;
		wxStaticText *textSmooth;
		wxStaticText *WxStaticText7;
		wxStaticText *WxStaticText6;
		wxStaticText *WxStaticText4;
		wxSlider *sliderSmoothing;
		wxCheckBox *checkBoxNewMethod;
		wxCheckBox *checkBoxFPS;
		wxCheckBox *checkBoxFloatiesPerSecond;
		wxCheckBox *checkBoxDebug;
		wxStaticText *WxStaticText5;
		wxCheckBox *checkBoxHands;
		wxCheckBox *checkBoxRefreshEnabled;
		wxStaticText *WxStaticText3;
		wxSpinCtrl *spinSpeed;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticText1;
		wxSpinCtrl *spinMax;
		wxPanel *WxPanel1;
		////GUI Control Declaration End
		void spinMaxUpdated(wxSpinEvent& event );
		void checkBoxRefreshEnabledClick(wxCommandEvent& event);
		void checkBoxHandsClick(wxCommandEvent& event);
		void checkBoxFloatiesPerSecondClick(wxCommandEvent& event);
		
		int floaty_count;
		int frame_count;
		float floaty_smooth;
		bool loading_frame;
		
		wxString s;
		void timerFloatyCountTimer(wxTimerEvent& event);
		void checkBoxFPSClick(wxCommandEvent& event);
		void checkBoxNewMethodClick(wxCommandEvent& event);
		void sliderSmoothingScroll(wxScrollEvent& event);
		void UpdateColorButtons();
		void buttonColorWhiteClick(wxCommandEvent& event);
		void buttonColorBlackClick(wxCommandEvent& event);
		void buttonColorTableClick(wxCommandEvent& event);
		void buttonColorBorderClick(wxCommandEvent& event);
		void buttonColorWhitePieceClick(wxCommandEvent& event);
		void buttonColorBlackPieceClick(wxCommandEvent& event);
		void buttonDefaultColorsClick(wxCommandEvent& event);
		void buttonColorPickupClick(wxCommandEvent& event);
		void buttonColorTakeClick(wxCommandEvent& event);
		void buttonColorPutdownClick(wxCommandEvent& event);
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_CHECKBOXLOUDDEBUG = 1071,
			ID_TIMERFLOATYCOUNT = 1018,
			ID_BUTTONCOLORPICKUP = 1070,
			ID_WXSTATICTEXT18 = 1068,
			ID_WXSTATICTEXT17 = 1067,
			ID_BUTTONCOLORPUTDOWN = 1066,
			ID_WXSTATICTEXT16 = 1065,
			ID_BUTTONCOLORTAKE = 1064,
			ID_WXSTATICTEXT15 = 1063,
			ID_BUTTONDEFAULTCOLORS = 1046,
			ID_WXSTATICTEXT14 = 1045,
			ID_BUTTONCOLORBLACKPIECE = 1044,
			ID_WXSTATICTEXT13 = 1043,
			ID_BUTTONCOLORWHITEPIECE = 1042,
			ID_WXSTATICTEXT12 = 1035,
			ID_WXSTATICTEXT11 = 1034,
			ID_WXSTATICTEXT10 = 1033,
			ID_WXSTATICTEXT9 = 1032,
			ID_BUTTONCOLORBLACK = 1031,
			ID_BUTTONCOLORWHITE = 1030,
			ID_BUTTONCOLORBORDER = 1029,
			ID_BUTTONCOLORTABLE = 1028,
			ID_WXSTATICTEXT8 = 1027,
			ID_TEXTSMOOTH = 1025,
			ID_WXSTATICTEXT7 = 1024,
			ID_WXSTATICTEXT6 = 1023,
			ID_WXSTATICTEXT4 = 1022,
			ID_SLIDERSMOOTHING = 1021,
			ID_CHECKBOXNEWMETHOD = 1020,
			ID_CHECKBOXFPS = 1019,
			ID_CHECKBOXFLOATIESPERSECOND = 1016,
			ID_CHECKBOXDEBUG = 1012,
			ID_WXSTATICTEXT5 = 1011,
			ID_CHECKBOXHANDS = 1009,
			ID_CHECKBOXREFRESHENABLED = 1007,
			ID_WXSTATICTEXT3 = 1006,
			ID_SPINSPEED = 1005,
			ID_WXSTATICTEXT2 = 1004,
			ID_WXSTATICTEXT1 = 1003,
			ID_SPINMAX = 1002,
			ID_WXPANEL1 = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
		
		
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif

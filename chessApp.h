//---------------------------------------------------------------------------
//
// Name:        chessApp.h
// Author:      gurkesaft
// Created:     6/23/2007 3:18:50 PM
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __CHESSFRAMEApp_h__
#define __CHESSFRAMEApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class chessFrameApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif

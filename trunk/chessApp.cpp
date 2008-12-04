//---------------------------------------------------------------------------
//
// Name:        chessApp.cpp
// Author:      gurkesaft
// Created:     6/23/2007 3:18:50 PM
// Description: 
//
//---------------------------------------------------------------------------

#include "chessApp.h"
#include "chessFrame.h"

IMPLEMENT_APP(chessFrameApp)

bool chessFrameApp::OnInit()
{
    chessFrame* frame = new chessFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int chessFrameApp::OnExit()
{
	return 0;
}

/*
 *	MyGLCanvas.h
 *	OpenGL Demo
 *
 *	Created by Ross Ashman on Jan 1 2005.
 *	Copyright (c) 2005 Ross Ashman. All rights reserved.
 *
 */

// MyGLCanvas.h: interface for the wxMyGLCanvas class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MYGLCANVAS_H__INCLUDED_
#define _MYGLCANVAS_H__INCLUDED_

#include <wx/glcanvas.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <gl/glu.h>
#include <stdlib.h>
#include <time.h>

#include "chessFrame.h"
class chessFrame;
class ChessPiece;
class Highlight;

class MyGLCanvas : public wxGLCanvas  
{
    public:
    	MyGLCanvas(chessFrame *chess,
                   wxWindow *parent, wxWindowID id = -1, 
                   const wxPoint& pos = wxDefaultPosition,                    
                   const wxSize& size = wxDefaultSize);
    	virtual ~MyGLCanvas();
    
    	// should be overwritten to draw something
    	void Draw();
        
        // Event Handlers (These Functions Should NOT be Virtual)
        // They are to be overwritten in the derived class as needed
        void OnPaint(wxPaintEvent& event);
    	void OnLeftMouse(wxMouseEvent& event);
    	void OnLeftMouseUp(wxMouseEvent& event);
    	void OnRightMouse(wxMouseEvent& event);
    	void OnRightMouseUp(wxMouseEvent& event);
    	void OnMouseMove(wxMouseEvent& event);
    	void OnEraseBackground(wxEraseEvent& event);
    	void OnIdle(wxIdleEvent& event);
    
    
        // Chess-specific methods
        void DrawWhiteSquare(int x, int y); // these don't do a swap
        void DrawBlackSquare(int x, int y);
        void DrawBoard();
        float GetMouseSquareX(int xcoord);
        float GetMouseSquareY(int ycoord);
        void GetMouseBoardCoordinates();
        void SetMousePiece(ChessPiece *piece);
        
        float GLRed(wxColour &c);
        float GLGreen(wxColour &c);
        float GLBlue(wxColour &c);


        void SetDefaultColors();
        void LoadPieces();       // imports all the images and sets aside the gl memory
        void DestroyPieces();    // deletes all the memory associated with the pieces
        void ClearBoard();       // clears the chess board of pieces
        void SetupBoard(int type, int style);    // sets up the board
        void Reverse();          // reverses the board
        void SendMouse();        // sends the mouse to everyone if we're server, or the server
        void SendSquare(int x, int y);
        void SendHighlights();
        
        wxString AssembleBoardString();        // creates a string to describe the board for the TCP send
        void UnpackBoardString(wxString data); // unpacks a board string and setups the pieces
        void UpdateErrbody();
        
        wxString PackHighlights();
        void UnpackHighlights(wxString data);
        
        wxString    GetPieceChar(ChessPiece *piece);     // returns a special character for each piece
        char        GetMousePieceChar();
        ChessPiece *GetPieceFromChar(char piece); // returns a pointer to the piece, given a char
        
        void FillRow(int row, ChessPiece *piece);
        void FillRowString(int row, wxString order);
        wxString Randomize(wxString input);
        wxString FischerRandomize();
        
        void OnKeyDown(wxKeyEvent &event);
        
        // Chess-specific variables
        wxColour color_white_square;
        wxColour color_black_square;
        wxColour color_background;
        wxColour color_border;
        wxColour color_white_piece;
        wxColour color_black_piece;
        
        chessFrame *chess;  // handle of the log for debugging, etc...

        // board piece locations
        ChessPiece *pieces[10][10];
        ChessPiece *mouse_piece;
        ChessPiece *haze;

        ChessPiece *white_king, *white_queen, *white_bishop, *white_knight, *white_rook, *white_pawn,
                   *black_king, *black_queen, *black_bishop, *black_knight, *black_rook, *black_pawn;
        Highlight *highlight_pickup, *highlight_take, *highlight_putdown;

        bool enable_leftmouseup;

        // all purpose string
        wxString s;
        
        // mouse location in terms of the board
        float x, y; 
        int last_clicked_square_x, last_clicked_square_y;
    
    private:
    	//DECLARE_CLASS(wxGLViewCanvas)
    	// any class wishing to process wxWindows events must use this macro
    	DECLARE_EVENT_TABLE()
          int raw_x, raw_y;
          int MouseButton;
};

#endif // _MYGLCANVAS_H__INCLUDED_

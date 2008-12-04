/*
 *	MyGLCanvas.cpp
 *	OpenGL Animation Demo
 *
 *	Ross Ashman 4 Jan 2005.
 *	Copyright (c) 2005 Ross Ashman. All rights reserved.
 *  
 */

// MyGLCanvas.cpp: implementation of the MyGLCanvas class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////
#include "wx/dir.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "MyGLCanvas.h"

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Event Tables and Other Macros for wxWindows
//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
	EVT_SIZE(MyGLCanvas::OnSize)
	EVT_PAINT(MyGLCanvas::OnPaint)
	EVT_LEFT_DOWN(MyGLCanvas::OnLeftMouse)
    EVT_LEFT_UP(MyGLCanvas::OnLeftMouseUp)	
	EVT_RIGHT_DOWN(MyGLCanvas::OnRightMouse)
    EVT_RIGHT_UP(MyGLCanvas::OnRightMouseUp)	
	EVT_MOTION(MyGLCanvas::OnMouseMove)	
	EVT_ERASE_BACKGROUND(MyGLCanvas::OnEraseBackground)
	EVT_IDLE(MyGLCanvas::OnIdle)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int Max(int x, int y) {if(x > y) return x; return y;}
int Min(int x, int y) {if(x < y) return x; return y;}

MyGLCanvas::MyGLCanvas(chessFrame *chess,
                       wxWindow *parent, 
                       wxWindowID id, 
                       const wxPoint& pos, 
                       const wxSize& size)
: wxGLCanvas(parent, id, pos, size, 0, "GL Frickin Canvas")
{   
    // get a handle on the host
    this->chess = chess;
    
    // initialize the board pieces and the mouse piece
    for(int n=0; n<=9; n++) for(int m=0; m<=9; m++) pieces[n][m] = NULL;
    SetMousePiece(NULL);

    int w,h;
    GetClientSize(&w,&h);
    if(GetContext())
	{
        SetCurrent();
    	glViewport(0, 0, (GLint)w, (GLint)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0,10.0,0,10.0);
        glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

    // load all the images
    LoadPieces();
    SetupBoard(chess->comboSetup->GetSelection(), chess->comboSetupStyle->GetSelection());
    
    last_clicked_square_x = -1;
    last_clicked_square_y = -1;
}

MyGLCanvas::~MyGLCanvas()
{
    DestroyPieces();
}

void MyGLCanvas::SetMousePiece(ChessPiece *piece)
{
    // set the mouse piece
    mouse_piece = piece;
    
    // if the piece isn't NULL (and the timer's not already going), start the timer
    if(chess->options->checkBoxRefreshEnabled->GetValue())
    {
        // if we're holding a piece or we're supposed to send our hand
        // then enable the timer!
        if(mouse_piece || chess->options->checkBoxHands->GetValue()) 
            chess->timerUpdate->Start(chess->options->spinSpeed->GetValue());
        else chess->timerUpdate->Stop();

        SendMouse(); // send the first iteration rather than waiting for the timer
    }
    else chess->timerUpdate->Stop(); // make sure it's stopped. Save bandwidth
    
}

void MyGLCanvas::SendMouse()
{
    // now send the first iteration (the clock doesn't kick in for one cycle
    if (chess->server)
    {
        chess->Debug("sending mouse floater as server");

        // update everyone
        chess->server->SendMouse();
    }
    else if (chess->client)
    {
        chess->Debug("sending mouse floater as client");

        // update the server
        chess->client->SendMouse();
    }
}

void MyGLCanvas::Draw()
{
    chess->options->frame_count++;
    
    // if we're in edit mode, make sure there are pieces to create everywhere!
    if(chess->checkBoxEditMode->GetValue())
    {
        // clear the border pieces
        for(int n=0; n<=9; n++)
        {
            pieces[0][n] = NULL;
            pieces[9][n] = NULL;
            pieces[n][0] = NULL;
            pieces[n][9] = NULL;
        }
        
        // now, depending on if we're in reversed mode, set up the piece selector
        int w=0, b=9;
        if(chess->checkBoxReversed->GetValue()) {w=9; b=0;}
        
        // actually place the resevoirs
        pieces[2][w] = white_king;
        pieces[3][w] = white_queen;
        pieces[4][w] = white_rook;
        pieces[5][w] = white_knight;
        pieces[6][w] = white_bishop;
        pieces[7][w] = white_pawn;
        
        // actually place the resevoirs
        pieces[2][b] = black_king;
        pieces[3][b] = black_queen;
        pieces[4][b] = black_rook;
        pieces[5][b] = black_knight;
        pieces[6][b] = black_bishop;
        pieces[7][b] = black_pawn;
    }
    
    // clear the window and put in the background color
    glClearColor(GLRed(color_background), GLGreen(color_background), GLBlue(color_background), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw the actual chess board squares
    DrawBoard();
    
    // draw the piece we're carrying if we're supposed to
    if(mouse_piece) mouse_piece->Draw(x-0.5, y-0.5);
    
    // also draw all the floaters
    MyClientData *c;
    for(unsigned int n=0; n<chess->listPeople->GetCount(); n++)
    {
        // our floater will always be NULL
        c = (MyClientData *)chess->listPeople->GetClientData(n);
        
        // if it's not null, draw it
        if(c->floater) c->floater->Draw(c->x, c->y, 0.7);
    }
    
    
    // flush the GL buffer and then swap in a new one
    glFlush();
    SwapBuffers();
}


//////////////////////////////////////////////////////////////////////
// Event Handler Implementations
//////////////////////////////////////////////////////////////////////

void MyGLCanvas::OnPaint(wxPaintEvent& event)
{
	//Draw();
	event.Skip();
}


void MyGLCanvas::OnLeftMouse(wxMouseEvent& event)
{        
    SetFocus();
    
    // if there's already a piece there, swap it with the mouse
    ChessPiece *temp;

    // only do the piece swap and stuff if we've moved since the last click
    if(last_clicked_square_x != (int)x || last_clicked_square_y != (int)y)
    {
        // set the last clicked square so we don't double click
        last_clicked_square_x = (int)x;
        last_clicked_square_y = (int)y;
        
        // start the timer (one shot) to wait a half second to release this hold
        chess->timerReleaseMouse->Start(500, true);
        
        // if we're not holding anything, we should do a pickup
        if (!mouse_piece) 
        {
            highlight_pickup->Put((int)x,(int)y);
            highlight_putdown->Remove();
            highlight_take->Remove();
        }
        // otherwise we're holding something. If there is nothing below, it's a drop
        else if(pieces[(int)x][(int)y] == NULL) 
        {
            highlight_putdown->Put((int)x, (int)y);
        }
        // otherwise we're swapping and it's a take
        else
        {
            highlight_take->Put((int)x, (int)y);
        }
        
        // swap pieces
        temp                   = pieces[(int)x][(int)y];
        pieces[(int)x][(int)y] = mouse_piece;
        SetMousePiece(temp); // also updates the internet floaters
    
        enable_leftmouseup=0;
        
        //Draw();
        // new method = just send the square we're fixing up
        // old method = send the whole board with each click
        if(chess->options->checkBoxNewMethod->GetValue()) 
        {
            SendHighlights();
            SendSquare((int)x, (int)y);
        }
        else UpdateErrbody();
        
        // now tell everyone
        //SendSquare((int)x, (int)y);
    }
}

void MyGLCanvas::OnRightMouse(wxMouseEvent& event)
{        
    SetFocus();
    
    // right mouse duplicates a piece (or deletes what you're holding)
    SetMousePiece(pieces[(int)x][(int)y]);   
    
    // If we're not deleting a piece, get rid of all the cursors
    if(mouse_piece != NULL)
    {
        highlight_take->Remove();
        highlight_putdown->Remove();
        highlight_pickup->Remove();
    }
    
    // no need to update everyone. That happens with the left click to put it down.
    //Draw();
}

void MyGLCanvas::OnLeftMouseUp(wxMouseEvent& event)
{        
    SetFocus();
    
    if(enable_leftmouseup) OnLeftMouse(event);
    
    // untie the mouse
    last_clicked_square_x = -1;
    last_clicked_square_y = -1;
}

void MyGLCanvas::OnRightMouseUp(wxMouseEvent& event)
{        
    SetFocus();
    
    MouseButton = 0;
}

void MyGLCanvas::OnMouseMove(wxMouseEvent& event)
{
    enable_leftmouseup = 1;
    
    // get the raw mouse coordinate
    event.GetPosition(&raw_x, &raw_y);
    
    // convert these into board coordinates
    GetMouseBoardCoordinates();
    
    // if we've moved out of the last clicked square, reset this memory
    if(last_clicked_square_x != (int)x || last_clicked_square_y != (int)y)
    {
        last_clicked_square_x = -1;
        last_clicked_square_y = -1;
    }
    
    //Draw();
    
    if (event.Dragging() == true)
    {
        
        if (MouseButton == 1)
        {
                
        }
        if (MouseButton == 2)
        {
                
        }
    }    
}

void MyGLCanvas::OnEraseBackground(wxEraseEvent& event)
{
	// Do Nothing to Avoid Flashing on MSW
}

void MyGLCanvas::OnIdle(wxIdleEvent& event)
{
	//this->Draw();
}




/////////////////////////////////////
// Chess-specific routines
/////////////////////////////////////

void MyGLCanvas::SetDefaultColors()
{
    color_white_square.Set(255, 255, 255);
    color_black_square.Set(192, 192, 192);
    color_white_piece.Set (170, 180, 255);
    color_black_piece.Set (244, 000, 000);
    color_background.Set  (63,  128, 128);
    color_border.Set      (177, 177, 177);
    
    highlight_pickup ->color.Set(100, 100, 255);
    highlight_take   ->color.Set(255, 050, 050);
    highlight_putdown->color.Set(100, 100, 255);
}

float MyGLCanvas::GLRed  (wxColour &c) {return (float)c.Red()/255.0;}
float MyGLCanvas::GLGreen(wxColour &c) {return (float)c.Green()/255.0;}
float MyGLCanvas::GLBlue (wxColour &c) {return (float)c.Blue()/255.0;}

void MyGLCanvas::DrawWhiteSquare(int x, int y)
{
    double w = 1.0;
	glBegin(GL_QUADS);
		glColor3ub(color_white_square.Red(), color_white_square.Green(), color_white_square.Blue());
		glVertex3f((x)*w, (y)*w, 0);
		glVertex3f((x)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y)*w, 0);
	glEnd();

}
void MyGLCanvas::DrawBlackSquare(int x, int y)
{
    double w = 1.0;
	glBegin(GL_QUADS);
		glColor3ub(color_black_square.Red(), color_black_square.Green(), color_black_square.Blue());
		glVertex3f((x)*w, (y)*w, 0);
		glVertex3f((x)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y)*w, 0);
	glEnd();

}

void MyGLCanvas::DrawBoard()
{
    glBegin(GL_QUADS);
		glColor3ub(color_border.Red(), color_border.Green(), color_border.Blue());
		glVertex3f(0.93, 0.93, 0);
		glVertex3f(9.07, 0.93, 0);
		glVertex3f(9.07, 9.07, 0);
		glVertex3f(0.93, 9.07, 0);
	glEnd();
	
    for(int n=1; n<=8; n+=1)
    {
        for(int m=1; m<=8; m+=1)
        {
            if((n+m)%2==0) DrawBlackSquare(n,m);
            else           DrawWhiteSquare(n,m);
        }
    }
    // if I don't do this, then the last square drawn (black) affects the piece color!
    glColor4f(1, 1, 1, 1);
		

    // now draw the highlight squares
    highlight_pickup->Draw();
    highlight_putdown->Draw();
    highlight_take->Draw();

    // I have to do this in this order for some reason or I get strange 
    // colors...
    
    // if there's a piece here, draw it
    for(int n=0; n<=9; n++)
        for(int m=0; m<=9; m++)
            if(pieces[n][m]) pieces[n][m]->Draw(n, m);

    // if we're in edit mode, kinda put a haze over the outer guys
    if(chess->checkBoxEditMode->GetValue())
    {
        for(int n=0; n<=9; n++) {haze->Draw(n,0); haze->Draw(n,9);}
        for(int n=1; n<=8; n++) {haze->Draw(0,n); haze->Draw(9,n);}
    }
}

float MyGLCanvas::GetMouseSquareX(int raw_x)
{
    int w, h;
    chess->panelBoard->GetSize(&w, &h);
    return (10*(float)raw_x/(float)w);
}

float MyGLCanvas::GetMouseSquareY(int raw_y)
{
    int w, h;
    chess->panelBoard->GetSize(&w, &h);
    return (10-10*(float)raw_y/(float)h);
}

void MyGLCanvas::GetMouseBoardCoordinates()
{
    x = GetMouseSquareX(raw_x);
    y = GetMouseSquareY(raw_y);
}

void MyGLCanvas::ClearBoard()
{
    for(int n=0; n<=9; n++)
        for(int m=0; m<=9; m++)
            pieces[n][m] = NULL;   
}

void MyGLCanvas::FillRow(int row, ChessPiece *piece)
{
    for(int n=1; n<=8; n++) pieces[n][row] = piece;
}

void MyGLCanvas::FillRowString(int row, wxString order)
{
    for(unsigned int n=1; n<=8 && n<=order.Length(); n++)
        pieces[n][row] = GetPieceFromChar(order[n-1]);
}

wxString MyGLCanvas::Randomize(wxString input)
{
    int r;

    // keep track of how many pieces we've used
    wxString remaining_pieces=input;
    s = "";
    for(unsigned int i=1; i<=input.Length(); i++)
    {
        // get a random number between 0 and 8-i
        r = rand() % (8-i+1);

        // pop the letter out of the string
        s = s + remaining_pieces[r];
        remaining_pieces.Remove((size_t)r, 1);
    }

    return s;    
}

wxString MyGLCanvas::FischerRandomize()
{
    std::vector<int> remaining_squares;
    int r1, r2;
    wxString output = "xxxxxxxx";

    // generate a list of the squares we have not used yet
    remaining_squares.clear();
    for(int n=0; n<=7; n++) remaining_squares.push_back(n);

    // first we place the black and white bishops
    r1 = rand()%4;
    r2 = rand()%4;
    output[r1*2] = 'b'; 
    output[r2*2+1] = 'b';

    // now remove these from the remaining squares
    remaining_squares.erase(remaining_squares.begin()+Max(2*r1,2*r2+1));
    remaining_squares.erase(remaining_squares.begin()+Min(2*r1,2*r2+1));

    // now add the queen and the two knights
    r1 = rand()%remaining_squares.size();
    output[remaining_squares[r1]] = 'q';
    remaining_squares.erase(remaining_squares.begin()+r1);

    r1 = rand()%remaining_squares.size();
    output[remaining_squares[r1]] = 'n';
    remaining_squares.erase(remaining_squares.begin()+r1);

    r1 = rand()%remaining_squares.size();
    output[remaining_squares[r1]] = 'n';
    remaining_squares.erase(remaining_squares.begin()+r1);

    // now put in the rook, king and rook
    output[remaining_squares[0]] = 'r'; 
    output[remaining_squares[1]] = 'k'; 
    output[remaining_squares[2]] = 'r'; 
    
    return output;
}    

void MyGLCanvas::SetupBoard(int type, int style)
{
    ChessPiece *temp;
    
    ClearBoard();
    
    if(type==0) // standard setup
    {
        FillRow(2, white_pawn);
        FillRow(7, black_pawn);
        
        FillRowString(1, "rnbqkbnr");
        FillRowString(8, "RNBQKBNR");
    }
    
    
    else if(type==1) // basic random
    {
        FillRow(2, white_pawn);
        FillRow(7, black_pawn);

        s = Randomize("rnbqkbnr");
        FillRowString(1, s);
        if     (style==0 || style==1) FillRowString(8, s.Upper());
        else if(style==2) FillRowString(8, Randomize("RNBQKBNR"));
    }
    
    
    else if(type==2) // fischer random / chess960
    {
        FillRow(2, white_pawn);
        FillRow(7, black_pawn);

        s = FischerRandomize();
        FillRowString(1, s);
        if     (style==0 || style==1) FillRowString(8, s.Upper());
        else if(style==2) FillRowString(8, FischerRandomize().Upper());
    }
    
    
    else if(type==3) // just pawns / manual setup
    {
        FillRow(2, white_pawn);
        FillRow(7, black_pawn);
    }
    
    else if(type==4) {} // clear the board
        




    // remove the highlights
    highlight_pickup->Remove();
    highlight_putdown->Remove();
    highlight_take->Remove();
    
    // if we're in odd mode, reverse the order of the top-half pieces
    if(style==1) 
    {
        for(int n=0; n<=4; n++)
        {
            for(int m=5; m<=9; m++)
            {
                temp           = pieces[n][m];
                pieces[n][m]   = pieces[9-n][m];
                pieces[9-n][m] = temp;
            }
        }
    }
    
    if(chess->checkBoxReversed->GetValue()) Reverse(); 
    //else                                    Draw();

    UpdateErrbody();
}

void MyGLCanvas::LoadPieces()
{
    // first find out if this piece directory is full of blank_...'s or white_ and black_
    wxDir d("Sets/xchess");
    
    // use the blank_ files if they're there. Otherwise, look for a traditional mode set
    if(d.HasFiles("blank_king.png"))
    {
        white_king   = new ChessPiece("Sets/xchess/blank_king.png", true);
        white_queen  = new ChessPiece("Sets/xchess/blank_queen.png", true);
        white_bishop = new ChessPiece("Sets/xchess/blank_bishop.png", true);
        white_knight = new ChessPiece("Sets/xchess/blank_knight.png", true);
        white_rook   = new ChessPiece("Sets/xchess/blank_rook.png", true);
        white_pawn   = new ChessPiece("Sets/xchess/blank_pawn.png", true);

        black_king   = new ChessPiece("Sets/xchess/blank_king.png", true);
        black_queen  = new ChessPiece("Sets/xchess/blank_queen.png", true);
        black_bishop = new ChessPiece("Sets/xchess/blank_bishop.png", true);
        black_knight = new ChessPiece("Sets/xchess/blank_knight.png", true);
        black_rook   = new ChessPiece("Sets/xchess/blank_rook.png", true);
        black_pawn   = new ChessPiece("Sets/xchess/blank_pawn.png", true);
        
        chess->options->buttonColorBlackPiece->Enable();
        chess->options->buttonColorWhitePiece->Enable();
    }
    else
    {
        white_king   = new ChessPiece("Sets/xchess/white_king.png", false);
        white_queen  = new ChessPiece("Sets/xchess/white_queen.png", false);
        white_bishop = new ChessPiece("Sets/xchess/white_bishop.png", false);
        white_knight = new ChessPiece("Sets/xchess/white_knight.png", false);
        white_rook   = new ChessPiece("Sets/xchess/white_rook.png", false);
        white_pawn   = new ChessPiece("Sets/xchess/white_pawn.png", false);

        black_king   = new ChessPiece("Sets/xchess/black_king.png", false);
        black_queen  = new ChessPiece("Sets/xchess/black_queen.png", false);
        black_bishop = new ChessPiece("Sets/xchess/black_bishop.png", false);
        black_knight = new ChessPiece("Sets/xchess/black_knight.png", false);
        black_rook   = new ChessPiece("Sets/xchess/black_rook.png", false);
        black_pawn   = new ChessPiece("Sets/xchess/black_pawn.png", false);

        chess->options->buttonColorBlackPiece->Disable();
        chess->options->buttonColorWhitePiece->Disable();
    }
    
    if(d.HasFiles("blank_highlight.png"))
    {
        highlight_pickup  = new Highlight("Sets/xchess/blank_highlight.png", true);
        highlight_take    = new Highlight("Sets/xchess/blank_highlight.png", true);
        highlight_putdown = new Highlight("Sets/xchess/blank_highlight.png", true);
        
        chess->options->buttonColorPickup ->Enable();
        chess->options->buttonColorPutdown->Enable();
        chess->options->buttonColorTake   ->Enable();
    }
    else
    { 
        highlight_pickup  = new Highlight("Sets/xchess/highlight_pickup.png");
        highlight_take    = new Highlight("Sets/xchess/highlight_take.png");
        highlight_putdown = new Highlight("Sets/xchess/highlight_putdown.png");
        
        chess->options->buttonColorPickup ->Disable();
        chess->options->buttonColorPutdown->Disable();
        chess->options->buttonColorTake   ->Disable();
    }
        
    haze = new ChessPiece("Sets/xchess/haze.png");
}

void MyGLCanvas::DestroyPieces()
{
    delete white_king;
    delete white_queen;
    delete white_bishop;
    delete white_knight;
    delete white_rook;
    delete white_pawn;
    
    delete black_king;
    delete black_queen;
    delete black_bishop;
    delete black_knight;
    delete black_rook;
    delete black_pawn;
    
    delete highlight_pickup;
    delete highlight_putdown;
    delete highlight_take;
    
    delete haze;
}

void MyGLCanvas::Reverse()
{
    ChessPiece *temp;

    // reverse the pieces
    for(int n=0; n<=9; n++)
        for(int m=0; m<=4; m++)
        {
            temp = pieces[n][m];
            pieces[n][m] = pieces[9-n][9-m];
            pieces[9-n][9-m] = temp;
        }

    // reverse the cursors
    highlight_pickup->x = 9-highlight_pickup->x;
    highlight_pickup->y = 9-highlight_pickup->y;
    highlight_putdown->x= 9-highlight_putdown->x;
    highlight_putdown->y= 9-highlight_putdown->y;
    highlight_take->x   = 9-highlight_take->x;
    highlight_take->y   = 9-highlight_take->y;    

    
}

char MyGLCanvas::GetMousePieceChar()
{
    return GetPieceChar(mouse_piece)[0];
}    

wxString MyGLCanvas::GetPieceChar(ChessPiece *piece)
{
    if     (piece == white_king)   return 'k';
    else if(piece == white_queen)  return 'q';
    else if(piece == white_bishop) return 'b';
    else if(piece == white_knight) return 'n';
    else if(piece == white_rook)   return 'r';
    else if(piece == white_pawn)   return 'p';
    
    else if(piece == black_king)   return 'K';
    else if(piece == black_queen)  return 'Q';
    else if(piece == black_bishop) return 'B';
    else if(piece == black_knight) return 'N';
    else if(piece == black_rook)   return 'R';
    else if(piece == black_pawn)   return 'P';
    
    return 'x';
}
ChessPiece *MyGLCanvas::GetPieceFromChar(char piece)
{
    if     (piece == 'k') return white_king;
    else if(piece == 'q') return white_queen;
    else if(piece == 'b') return white_bishop;
    else if(piece == 'n') return white_knight;
    else if(piece == 'r') return white_rook;
    else if(piece == 'p') return white_pawn;

    else if(piece == 'K') return black_king;
    else if(piece == 'Q') return black_queen;
    else if(piece == 'B') return black_bishop;
    else if(piece == 'N') return black_knight;
    else if(piece == 'R') return black_rook;
    else if(piece == 'P') return black_pawn;

    return NULL;
}

void MyGLCanvas::UpdateErrbody()
{
    chess->UpdateErrbody();
}

wxString MyGLCanvas::AssembleBoardString()
{
    // first come up with a data string to give the coordinates of all pieces
    wxString data = "";
    for(int n=0; n<=9; n++)
    {
        for(int m=0; m<=9; m++)
        {
            if(chess->checkBoxReversed->GetValue()) data = data + GetPieceChar(pieces[9-n][9-m]);
            else                                    data = data + GetPieceChar(pieces[n][m]);
        }
    }
    return data;
}
void MyGLCanvas::UnpackBoardString(wxString data)
{
    // Take a string that describes the board layout and setup the board accordingly
    for(int n=0; n<=9; n++)
    {
        for(int m=0; m<=9; m++)
        {
            if(chess->checkBoxReversed->GetValue()) pieces[9-n][9-m] = GetPieceFromChar(data[10*n+m]);
            else                                    pieces[n][m]     = GetPieceFromChar(data[10*n+m]);
        }
    }
}

wxString MyGLCanvas::PackHighlights()
{
    if(chess->checkBoxReversed->GetValue())
    {
        s.sprintf("%i%i%i%i%i%i",
              9-highlight_pickup->x,
              9-highlight_pickup->y,
              9-highlight_putdown->x,
              9-highlight_putdown->y,
              9-highlight_take->x,
              9-highlight_take->y);
    }
    else
    {
        s.sprintf("%i%i%i%i%i%i",
              highlight_pickup->x,
              highlight_pickup->y,
              highlight_putdown->x,
              highlight_putdown->y,
              highlight_take->x,
              highlight_take->y);
    }
    return s;
}

void MyGLCanvas::UnpackHighlights(wxString data)
{
    long x, y;

    s = data[0]; s.ToLong(&x);
    s = data[1]; s.ToLong(&y);
    if(chess->checkBoxReversed->GetValue()) highlight_pickup ->Put(9-x, 9-y);
    else                                    highlight_pickup ->Put(x,   y);

    s = data[2]; s.ToLong(&x);
    s = data[3]; s.ToLong(&y);
    if(chess->checkBoxReversed->GetValue()) highlight_putdown ->Put(9-x, 9-y);
    else                                    highlight_putdown ->Put(x,   y);

    s = data[4]; s.ToLong(&x);
    s = data[5]; s.ToLong(&y);
    if(chess->checkBoxReversed->GetValue()) highlight_take ->Put(9-x, 9-y);
    else                                    highlight_take ->Put(x,   y);

    //Draw();
}

void MyGLCanvas::SendSquare(int x, int y)
{
    // this just sends the info about this square
    // of the form "+k" with the data being the integers
    s = "+";
    s = s + GetPieceChar(pieces[x][y]);
    
    if(chess->checkBoxReversed->GetValue())
    {
        x = 9-x;
        y = 9-y;
    }
    
    int coords[2];
    coords[0] = x;
    coords[1] = y;
    if     (chess->client) chess->client->connection->Poke(s, (char *)coords, 2*sizeof(int));
    else if(chess->server) chess->server->PokeEveryone    (s, (char *)coords, 2*sizeof(int));
}

void MyGLCanvas::SendHighlights()
{
    // format the string for the highlight coordinates
    s = chess->board->PackHighlights();
    if(chess->client)      chess->client->connection->Poke("highlights", (wxChar *)s.GetData());   
    else if(chess->server) chess->server->PokeEveryone(    "highlights", (wxChar *)s.GetData());
}


void MyGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    if(event.GetKeyCode() == WXK_SPACE)
    {
        // When a key is pressed and the board has focus, push the bottom toggle
        chess->toggleBottom->SetValue(true);
        chess->toggleTop->SetValue(false);
        wxCommandEvent e;
        chess->toggleBottomClick(e);
    }
    event.Skip();
}

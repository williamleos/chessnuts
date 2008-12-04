#include "ChessGLCanvas.h"

ChessGLCanvas::ChessGLCanvas(wxTextCtrl  *log,
                             wxWindow *parent, wxWindowID id, 
                             const wxPoint& pos, const wxSize& size)
: MyGLCanvas(parent, id, pos, size)
{
    this->log = log;
    
    int w,h;
    GetClientSize(&w,&h);

    s.sprintf("-------\nwindow size = %i %i\n", w, h);
    log->AppendText(s); 

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

    // set the white and black square colors
    wR=1.0;
    wB=1.0;
    wG=1.0;
    bR=0.9;
    bB=0.9;
    bG=0.9;

    // set the background color
    bgR=0.4;
    bgB=0.5;
    bgG=0.7;
    
    glClearColor(bgR, bgG, bgB, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
    SwapBuffers();
}

ChessGLCanvas::~ChessGLCanvas()
{

}

//////////////////////////////////////////////////////////////////////
// Method Implementations
//////////////////////////////////////////////////////////////////////


void ChessGLCanvas::Draw()
{
    log->AppendText("-------\ndrawing the table...");
    
    // clear the window and put in the background color
    glClearColor(bgR, bgG, bgB, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw the actual chess board squares
    DrawBoard();

    // load a test image
    wxImage image;
    wxInitAllImageHandlers();
    image.LoadFile("..\\..\\Images\\test.png");

    // now convert rgb and a arrays into rgba array
    unsigned char *rgba_data;
    rgba_data = (unsigned char *) malloc(image.GetWidth()*image.GetHeight()*4);
    for(int n=0; n<image.GetWidth()*image.GetHeight(); n++)
    {
        rgba_data[4*n]   = image.GetData()[3*n];   // red channel
        rgba_data[4*n+1] = image.GetData()[3*n+1]; // green channel
        rgba_data[4*n+2] = image.GetData()[3*n+2]; // blue channel
        rgba_data[4*n+3] = image.GetAlpha()[n];     // alpha channel
    }

    // come up with the places in GL to store the image data
    GLuint texture[1];
    glGenTextures(1, texture); // sets aside some global integers that aren't in use by GL elsewhere for storing textures
    glBindTexture(GL_TEXTURE_2D, texture[0]); // until otherwise told, work with texture[0]

    // set up some parameters for handling this texture
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear interpolation of colors
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear interpolation of colors
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // if we want to learn lighting later

    // actually store the image data in the GL memory somewhere (can now delete the image object memory)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_data);

    free(rgba_data);

    // draw one of the textures
    glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
       glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.5, 0);	// Bottom Left Of The Texture and Quad
	   glTexCoord2f(1.0, 1.0); glVertex3f(1.5, 0.5, 0);	// Bottom Right Of The Texture and Quad
	   glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 1.5, 0);	// Top Right Of The Texture and Quad
	   glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 1.5, 0);	// Top Left Of The Texture and Quad
	glEnd();
    glBegin(GL_QUADS);
       glTexCoord2f(0, 1); glVertex3f(0, 0, 0);	// Bottom Left Of The Texture and Quad
	   glTexCoord2f(1, 1); glVertex3f(1, 0, 0);	// Bottom Right Of The Texture and Quad
	   glTexCoord2f(1, 0); glVertex3f(1, 1, 0);	// Top Right Of The Texture and Quad
	   glTexCoord2f(0, 0); glVertex3f(0, 1, 0);	// Top Left Of The Texture and Quad
	glEnd();
    glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);

    // flush the GL buffer and then swap in a new one
    glFlush();
    SwapBuffers();
}


//////////////////////////////////////////////////////////////////////
// Event Handler Implementations
//////////////////////////////////////////////////////////////////////


void ChessGLCanvas::OnLeftMouse(wxMouseEvent& event)
{
        log->AppendText("-------\nleft mouse button.");
        MouseButton = 1;
        event.GetPosition(&Xcoord, &Ycoord);
}

void ChessGLCanvas::OnRightMouse(wxMouseEvent& event)
{
        MouseButton = 2;
        event.GetPosition(&Dummy, &Zcoord);
}

void ChessGLCanvas::OnLeftMouseUp(wxMouseEvent& event)
{
        MouseButton = 0;
}

void ChessGLCanvas::OnRightMouseUp(wxMouseEvent& event)
{
        MouseButton = 0;
}

void ChessGLCanvas::OnMouseMove(wxMouseEvent& event)
{

    if (event.Dragging() == true)
    {
        event.GetPosition(&X, &Y);
        if (MouseButton == 1)
        {

        }
        if (MouseButton == 2)
        {

        }
        Draw(); // Causes an event to be generated to repaint the window.

    }
}


/////////////////////////////////////
// Chess-specific routines
/////////////////////////////////////

void ChessGLCanvas::SetWhiteSquareColor(double R, double B, double G) {wR = R; wB = B; wG = G;}
void ChessGLCanvas::SetBlackSquareColor(double R, double B, double G) {bR = R; bB = B; bG = G;}

void ChessGLCanvas::DrawWhiteSquare(int x, int y)
{
    double w = 1.0;
	glBegin(GL_QUADS);
		glColor3f(wR, wB, wG);
		glVertex3f((x)*w, (y)*w, 0);
		glVertex3f((x)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y)*w, 0);
	glEnd();

}
void ChessGLCanvas::DrawBlackSquare(int x, int y)
{
    double w = 1.0;
	glBegin(GL_QUADS);
		glColor3f(bR, bB, bG);
		glVertex3f((x)*w, (y)*w, 0);
		glVertex3f((x)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y+1)*w, 0);
		glVertex3f((x+1)*w, (y)*w, 0);
	glEnd();

}

void ChessGLCanvas::DrawBoard()
{
    int x, y;
    for(x=1; x<=8; x+=1)
    {
        for(y=1; y<=8; y+=1)
        {
            if((x+y)%2==0) DrawWhiteSquare(x,y);
            else           DrawBlackSquare(x,y);
        }
    }
}

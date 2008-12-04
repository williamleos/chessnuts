#include <wx/wx.h>

#include "MyGLCanvas.h"
class MyGLCanvas;

class ChessGLCanvas : public MyGLCanvas
{
    public:
    	ChessGLCanvas(wxTextCtrl *log, // things I want a handle on
                      wxWindow *parent, wxWindowID id = -1,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize);
    	virtual ~ChessGLCanvas();

    	// Overwritten Methods
    	void Draw();
        
        // Overwritten Event Handlers (These Functions Should NOT be Virtual)
        void OnLeftMouse(wxMouseEvent& event);
    	void OnLeftMouseUp(wxMouseEvent& event);
    	void OnRightMouse(wxMouseEvent& event);
    	void OnRightMouseUp(wxMouseEvent& event);
    	void OnMouseMove(wxMouseEvent& event);
    	
        // Chess-specific methods
        void SetWhiteSquareColor(double R, double B, double G);
        void SetBlackSquareColor(double R, double B, double G);
        void DrawWhiteSquare(int x, int y); // these don't do a swap
        void DrawBlackSquare(int x, int y);
        void DrawBoard();
        
        // Chess-specific variables
        double wR, wB, wG;  // square colors
        double bR, bB, bG;
        double bgR,bgB,bgG; // background color
        wxTextCtrl *log;     // handle of the log for debugging, etc...

        // all purpose string
        wxString s;

};

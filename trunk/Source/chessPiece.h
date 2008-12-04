#include <wx/wx.h>
#include <gl/glu.h>

class ChessPiece
{
    public:
        ChessPiece(wxString image_path, bool blank=true);
        virtual ~ChessPiece();

        void Draw(float x=-1, float y=-1, float alpha=1.0); // this draws it on the board (but doesn't update it).
        
        GLuint gl_index; // gl_index linked to the texture
        bool blank;      // if we need to supply a color
        wxColour color;
};
        
class Highlight
{
    public:
        Highlight(wxString image_path, bool blank=true);
        virtual ~Highlight();

        void Draw(float alpha=1.0); // this draws it on the board (but doesn't update it.
        void Put(int x=-1, int y=-1); // sets x and y
        void Remove();          // puts the highlight off the board
        
        int x, y; // gl coordinates of the highlight

        wxString s;
        GLuint gl_index; // gl_index linked to the texture
        bool blank;
        wxColour color;
};

#include "chessPiece.h"

ChessPiece::ChessPiece(wxString image_path, bool blank)
{
    // store whether this is a blank piece or not
    this->blank = blank;
    color.Set(255, 255, 255); // set the default color
    
    // load the image
    wxImage image;
    wxInitAllImageHandlers();
    image.LoadFile(image_path);

    // now convert rgb and a arrays into rgba array
    unsigned char *rgba_data;
    rgba_data = (unsigned char *) malloc(image.GetWidth()*image.GetHeight()*4);
    for(int n=0; n<image.GetWidth()*image.GetHeight(); n++)
    {
        rgba_data[4*n]   = image.GetData()[3*n];   // red channel
        rgba_data[4*n+1] = image.GetData()[3*n+1]; // green channel
        rgba_data[4*n+2] = image.GetData()[3*n+2]; // blue channel
        rgba_data[4*n+3] = image.GetAlpha()[n];    // alpha channel
    }
    
    // until otherwise told, work with this texture
    // gl_index is a constant of this piece for accessing the memory
    glGenTextures(1, &gl_index);
    glBindTexture(GL_TEXTURE_2D, gl_index); 

    // set up some parameters for handling this texture
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear interpolation of colors
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear interpolation of colors
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // if we want to learn lighting later

    // actually store the image data in the GL memory somewhere (can now delete the image object memory)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_data);
    free(rgba_data);
}
ChessPiece::~ChessPiece()
{
    
}

void ChessPiece::Draw(float x, float y, float alpha)
{
    // set the color
    if(!blank) glColor4ub(255,         255,           255,          (int)(alpha*255));
    else       glColor4ub(color.Red(), color.Green(), color.Blue(), (int)(alpha*255));
    
    // bind the texture
    glBindTexture(GL_TEXTURE_2D, gl_index);
    
    // draw one of the textures
    glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        // map the texture square to the board
        glTexCoord2f(0.0, 1.0); glVertex3f(x,     y,     0);	
	    glTexCoord2f(1.0, 1.0); glVertex3f(x+1.0, y,     0);	
	    glTexCoord2f(1.0, 0.0); glVertex3f(x+1.0, y+1.0, 0);	
	    glTexCoord2f(0.0, 0.0); glVertex3f(x,     y+1.0, 0);	
	glEnd();
    glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);
}






Highlight::Highlight(wxString image_path, bool blank)
{
    this->blank = blank;
    
    // load the image
    wxImage image;
    wxInitAllImageHandlers();
    image.LoadFile(image_path);

    // now convert rgb and a arrays into rgba array
    unsigned char *rgba_data;
    rgba_data = (unsigned char *) malloc(image.GetWidth()*image.GetHeight()*4);
    for(int n=0; n<image.GetWidth()*image.GetHeight(); n++)
    {
        rgba_data[4*n]   = image.GetData()[3*n];   // red channel
        rgba_data[4*n+1] = image.GetData()[3*n+1]; // green channel
        rgba_data[4*n+2] = image.GetData()[3*n+2]; // blue channel
        rgba_data[4*n+3] = image.GetAlpha()[n];    // alpha channel
    }

    // until otherwise told, work with this texture
    glGenTextures(1, &gl_index);
    glBindTexture(GL_TEXTURE_2D, gl_index);

    // set up some parameters for handling this texture
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear interpolation of colors
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear interpolation of colors
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // if we want to learn lighting later

    // actually store the image data in the GL memory somewhere (can now delete the image object memory)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_data);
    free(rgba_data);
    
    Remove();
}
Highlight::~Highlight()
{

}

void Highlight::Draw(float alpha)
{
    // only draw if we're on the board
    if(x<=0 || y<=0) return;
    if(x>=9 || y>=9) return;
    
    // set the color
    if(!blank) glColor4ub(255,         255,           255,          (int)(alpha*255));
    else       glColor4ub(color.Red(), color.Green(), color.Blue(), (int)(alpha*255));
    
    glBindTexture(GL_TEXTURE_2D, gl_index);

    // draw one of the textures
    glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        // map the texture square to the board
        glTexCoord2f(0.0, 1.0); glVertex3f(x,     y,     0);
	    glTexCoord2f(1.0, 1.0); glVertex3f(x+1.0, y,     0);
	    glTexCoord2f(1.0, 0.0); glVertex3f(x+1.0, y+1.0, 0);
	    glTexCoord2f(0.0, 0.0); glVertex3f(x,     y+1.0, 0);
	glEnd();
    glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);
}

void Highlight::Remove()
{
    this->x = -1;
    this->y = -1;
}
void Highlight::Put(int x, int y)
{
    this->x = x;
    this->y = y;
}

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/log.h>
#include <wx/textfile.h>

#ifndef __STANDALONES_h__
#define __STANDALONES_h__

wxArrayString SplitString(wxString s, wxString delimiter);
wxString SafeGetLine(wxTextFile &f, unsigned long n);

#endif

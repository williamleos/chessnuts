#include "standalones.h"

wxArrayString SplitString(wxString s, wxString delimiter)
{
    wxArrayString a;

    wxStringTokenizer tox0r(s, delimiter);
    while (tox0r.HasMoreTokens())  a.Add(tox0r.GetNextToken());

    return a;
}
    

wxString SafeGetLine(wxTextFile &f, unsigned long n)
{
    if(f.GetLineCount() > n) return f.GetLine(n);
    else return "";
}

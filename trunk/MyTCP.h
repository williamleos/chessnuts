#include "wx/sckipc.h"
#include "chessFrame.h"
#include "MyGLCanvas.h"
#include <wx/stopwatch.h>

#ifndef _MYTCP_H
#define _MYTCP_H


class chessFrame;
class ChessPiece;

class MyConnection: public wxTCPConnection
{
    public:
        MyConnection(chessFrame *chess=NULL, unsigned int index=0);
        virtual ~MyConnection();
        virtual bool Poke(const wxString& item, wxChar *data, int size = -1, wxIPCFormat format = wxIPC_TEXT);
        virtual bool OnPoke(const wxString& topic, const wxString& item, wxChar *data, int size, wxIPCFormat format);
        virtual bool OnDisconnect();
        
        void SendBoard();
        void SendHighlights();
        void SendMouse();
        void SendClientList();
        void SendErrthing();
        void UpdateEveryoneElse(); // if we're the server, update all the other connections
        void PokeEveryoneElse(const wxString& item, wxChar *data, int size=-1, wxIPCFormat format=wxIPC_TEXT); // if we're the server, poke all other connections

        chessFrame *chess;
        wxString s;
        unsigned int index;
};




class MyClient: public wxTCPClient
{
    public:
        MyClient(chessFrame *chess);
        ~MyClient();
        
        bool Connect(const wxString& sHost, const wxString& sService, const wxString& sTopic);
        void Disconnect();
        void UpdateErrbody();
        void SendMouse();
        
        wxConnectionBase *OnMakeConnection();
        
        MyConnection *connection;
        chessFrame   *chess;
        wxString s;
};



class MyServer: public wxTCPServer
{
    public:
        MyServer(chessFrame *chess);
        ~MyServer();
        
        void UpdateErrbody();
        void Disconnect(unsigned int n);
        void SendEveryoneClientList();
        void PokeEveryone(const wxString& item, wxChar *data, int size=-1, wxIPCFormat format=wxIPC_TEXT);
        void SendMouse();
        void SetIndices();
        
        wxConnectionBase *OnAcceptConnection(const wxString& topic);
    
        chessFrame *chess;
        wxString s;
};



class MyClientData: public wxClientData
{
    public:
        MyClientData(chessFrame *chess, unsigned int index); 
        virtual ~MyClientData();
        
        MyConnection *connection;
        chessFrame *chess;   // this is the owner window
        ChessPiece *floater; // this is the floating piece
        float x, y;         // piece coordinates
        float x_start,  y_start;
        float x_target, y_target;
        wxString s;
        unsigned int index;
        wxStopWatch timeypoo;
};

#endif

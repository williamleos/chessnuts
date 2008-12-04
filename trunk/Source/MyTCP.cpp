#include "MyTCP.h"


MyConnection::MyConnection(chessFrame *chess, unsigned int index) : wxTCPConnection()
{
    this->chess = chess;
    this->index = index;
}
MyConnection::~MyConnection() 
{
    chess->Debug("~MyConnection()");
}

bool MyConnection::OnDisconnect()
{    
    chess->Debug("MyConnection::OnDisconnect()");
    
    if(chess->client)
    {
        chess->Debug("(client)");
        // this only happens when we GET disconnected somehow, not when 
        // we actually disconnect ourselves
        chess->Log("Disconnected from server.");
        chess->ClearClientList();
        chess->EnableConnect();
    }
    else if(chess->server) 
    {
        chess->Debug("(server)");
        
        // remove this dude from the list
        if(index != 0) 
        {
            s = chess->listPeople->GetString(index) + " disconnected.";
            chess->Log(s);

            // now delete the element
            chess->Debug("server is deleting client from list");
            chess->listPeople->Delete(index);
            chess->server->SetIndices();
            chess->server->SendEveryoneClientList();
            
            // tell everyone
            s = "*** "+s;
            chess->server->PokeEveryone("chat", (wxChar *)s.GetData());
        }
        else chess->Log("Here we are trying to disconnect the zero'th element. Someday I should figure out why.");
    }
    
    return true;
}

bool MyConnection::Poke(const wxString& item, wxChar *data, int size, wxIPCFormat format)
{
    if(item[0] != '_') chess->Debug("MyConnection::Poke(): Sending a '"+item+"'.");
    
    return wxTCPConnection::Poke(item, data, size, format);
}

bool MyConnection::OnPoke(const wxString& topic,
    const wxString& item, wxChar *data, int size, wxIPCFormat format)
{
    s.sprintf("%i", size);
    if(item[0] != '_') chess->Debug("MyConnection::OnPoke() Received a '"+item+"', size="+s);
    
    if(item[0] == '_')
    {
        unsigned int n;
        
        // get the coordinate array from the char array
        float *coords = (float *)data;
        
        if(chess->options->checkBoxFloatiesPerSecond->GetValue())
            chess->options->floaty_count++;
        
        // if we're the server, the clients are sending the format "_k"
        // where '_' says "floater event!", and 'k' says "king"
        // the data is a 2*sizeof(float) byte array containting two floats, x, y
        if(chess->server)
        {
            // get the player index
            n = index;
            
            // convert the index to a char, and stick it on the item for everyone else
            s = "_";
            s = s + item[1] + (char)n;
            
            // now relay this info
            PokeEveryoneElse(s, data, size);
            
        }
        else // we're the client
        {
            // if we're the client, the item should arrive with format "_k#"
            // where '_' says "mouse event!", 'k' says "king", and '#' says the player index
            // note # is a char, so it ranges from 0-256 or something like that.

            // get the player index
            n = (unsigned int)item[2];
        }

        // get the connection object
        MyClientData *c = (MyClientData *)chess->listPeople->GetClientData(n);

        // set the floater piece
        ChessPiece *previous = c->floater;
        c->floater = chess->board->GetPieceFromChar(item[1]);
        
        // reverse if necessary
        if(chess->checkBoxReversed->GetValue())
        {
            coords[0] = 9.0-coords[0];
            coords[1] = 9.0-coords[1];
        }
        
        
        // deal with the interpolation
        
        // if this is the first in the interpolation, set the start to here as well
        if(previous == NULL)
        {
            c->x_start = coords[0];
            c->y_start = coords[1];
            c->x = coords[0];
            c->y = coords[1];
        }
        else // otherwise, use the current floater position as the start
        {
            c->x_start = c->x;
            c->y_start = c->y;
        }
        
        // set the end goal of the interpolation    
        c->x_target = coords[0];
        c->y_target = coords[1];

        // start this so we can know what time we're at in the interpolation loop
        c->timeypoo.Start(0);
    }
    
    else if(item[0] == '+') // single square change
    {
        int *coords = (int *)data;
        
        // if we're the server, relay the info
        if(chess->server) PokeEveryoneElse(item, data, size, format);
        
        if(chess->checkBoxReversed->GetValue())
        {
            coords[0] = 9-coords[0];
            coords[1] = 9-coords[1];
        }
        
        // the second char is the piece, the data is two chars, representing the coords
        chess->board->pieces[coords[0]][coords[1]] = chess->board->GetPieceFromChar(item[1]);
        //chess->board->Draw();
    }
    
    else if(item[0] == '=') // name change
    {
        unsigned int n;
        
        if(chess->server) 
        {
            n = index;
            
            // now tell everyone else
            s = "=";
            chess->server->PokeEveryone(s+(char)index, data, size, format);
        }
        else // we're the client
        {
            // we're being told to change this entry's name
            n = (unsigned int)item[1];
        }
        chess->listPeople->SetString(n, data);
    }
    
    else if(item == "ClockTop")
    {
        // this causes an update event, so we need to block it from resending the data
        chess->clockblocker_top = true;
        if(chess->checkBoxReversed->GetValue())
        {
            chess->editTimeBottom->SetValue(data);
            chess->ms_bottom = chess->StringToMilli(data);
        }
        else
        {
            chess->editTimeTop->SetValue(data);
            chess->ms_top = chess->StringToMilli(data);
        }
        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }
    
    else if(item == "ClockBottom")
    {
        chess->clockblocker_bottom = true;
        if(!chess->checkBoxReversed->GetValue())
        {
            chess->editTimeBottom->SetValue(data);
            chess->ms_bottom = chess->StringToMilli(data);
        }
        else
        {
            chess->editTimeTop->SetValue(data);
            chess->ms_top = chess->StringToMilli(data);
        }
        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }
        
    else if(item == "ClockStop")
    {
        // push the pause button (takes care of the GUI) but don't send anything
        chess->Pause();
        
        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }
    
    else if(item == "ToggleTop")
    {
        long *times = (long *)data;
        
        if(chess->checkBoxReversed->GetValue())
        {
            // update the GUI
            chess->toggleBottom->SetValue(true);
            chess->toggleTop->   SetValue(false);
               
            // set the times (they come as top,bottom unreversed)
            chess->ms_top    = times[1];
            chess->ms_bottom = times[0];
        }
        else
        {
            // update the GUI
            chess->toggleBottom->SetValue(false);
            chess->toggleTop->   SetValue(true);

            // set the times (they come as top,bottom unreversed)
            chess->ms_top    = times[0];
            chess->ms_bottom = times[1];            
        }
        // start a stopwatch at zero
    	chess->ms_top_last    = chess->ms_top;
    	chess->ms_bottom_last = chess->ms_bottom;
        chess->timeypoo.Start(0);
    	chess->timerClock->Start(100);
            
        
        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }
    
    else if(item == "ToggleBottom")
    {
        long *times = (long *)data;

        if(chess->checkBoxReversed->GetValue())
        {
            // update the GUI
            chess->toggleBottom->SetValue(false);
            chess->toggleTop->   SetValue(true);

            // set the times (they come as top,bottom when unreversed)
            chess->ms_top    = times[1];
            chess->ms_bottom = times[0];
        }
        else
        {
            // update the GUI
            chess->toggleBottom->SetValue(true);
            chess->toggleTop->   SetValue(false);

            // set the times (they come as top,bottom unreversed)
            chess->ms_top    = times[0];
            chess->ms_bottom = times[1];
        }
        // start a stopwatch at zero
    	chess->ms_top_last    = chess->ms_top;
    	chess->ms_bottom_last = chess->ms_bottom;
        chess->timeypoo.Start(0);
    	chess->timerClock->Start(100);


        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }   
    
    else if(item == "Clocks")
    {
        long *times = (long *)data;

        if(chess->checkBoxReversed->GetValue())
        {
            // set the times (they come as top,bottom when unreversed)
            chess->ms_top    = times[1];
            chess->ms_bottom = times[0];
        }
        else
        {
            // set the times (they come as top,bottom unreversed)
            chess->ms_top    = times[0];
            chess->ms_bottom = times[1];
        }
        
        chess->SetClocks();
        
        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }
    
    else if(item == "EditMode")
    {
        // first set the check box appropriately
        bool previous = chess->checkBoxEditMode->GetValue();

        if(data[0] == '0') chess->checkBoxEditMode->SetValue(false);
        else               chess->checkBoxEditMode->SetValue(true);
        
        // now pretend we pushed it (but only if it changed)
        if(previous != chess->checkBoxEditMode->GetValue()) chess->ToggleEditMode();
                
        // now, if we're the server, update everyone else!
        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }
    
    else if(item == "ClearClientList" && !chess->server) 
    {
        // since we're not the server, clear the client list (and free the resources)
        chess->ClearClientList();
    }
    
    else if(item == "AppendClient" && !chess->server)
    {
        // if we're not a server, append a new client
        // Since we're the client, the connection object in listPeople needn't be created
        chess->listPeople->Append(data, (wxClientData *)new MyClientData(chess, chess->listPeople->GetCount()));
    }
    
    else if(item == "Speed" && !chess->server)
    {
        chess->Debug("Server is changing the data rate.");
        chess->options->spinSpeed->SetValue(*(int *)data);   
    }
    
    else if(item == "Max" && !chess->server)
    {
        chess->Debug("Server is changing the maximum number of clients.");
        chess->options->spinMax->SetValue(*(int *)data);
    }
    
    else if(item == "EnableRefresh" && !chess->server)
    {
        chess->Debug("Server has enabled floaty piece mode.");
        chess->options->checkBoxRefreshEnabled->SetValue(true);
    }
    
    else if(item == "DisableRefresh" && !chess->server)
    {
        chess->Debug("Server has disabled floaty piece mode.");
        chess->options->checkBoxRefreshEnabled->SetValue(false);
    }
    
    else if(item == "Hands" && !chess->server)
    {
        chess->Debug("Server has enabled floaty hand mode.");
        chess->options->checkBoxHands->SetValue(true);
    }

    else if(item == "NoHands" && !chess->server)
    {
        chess->Debug("Server has disabled floaty hand mode.");
        chess->options->checkBoxHands->SetValue(false);
    }
    
    else if(item == "NewMethod" && !chess->server)
    {
        chess->Debug("Server has changed piece update to new method.");
        chess->options->checkBoxNewMethod->SetValue(true);
    }
    
    else if(item == "OldMethod" && !chess->server)
    {
        chess->Debug("Server has switched piece update to old method.");
        chess->options->checkBoxNewMethod->SetValue(false);
    }
    
    else if(item == "name" && chess->server)
    {
        // only the client will send a "name". It means they just joined. 
        // to change their name, they send an "=". Update the list, and send
        // the client the user list along with the board   
        
        // update the user's name
        s = chess->listPeople->GetString(index);
        chess->listPeople->SetString(index, data);
        
        // tell everyone
        s = chess->listPeople->GetString(index) + " joined the fray.";
        chess->Log(s);
        
        s = "*** " + s;
        chess->server->PokeEveryone("chat", (wxChar *)s.GetData());
        
        // send everyone the new client list
        chess->server->SendEveryoneClientList();   
        
        // send the data rate
        int m = chess->options->spinSpeed->GetValue();
	    Poke("Speed", (char *)&m, sizeof(int));
        
        // send the max clients
        m = chess->options->spinMax->GetValue();
        Poke("Max", (char *)&m, sizeof(int));
        
        // send the other options
        wxCommandEvent e;
        chess->options->checkBoxRefreshEnabledClick(e);
        chess->options->checkBoxHandsClick(e);
        
        // send the board and stuff (just to this client)
        // this is redundant if they're just changing their name, but what the hay
        SendErrthing();
    }
    
    else if(item == "chat")
    {
        s.sprintf("%s\n", data);
        chess->memoLog->AppendText(s);
        if(chess->server) chess->server->PokeEveryone("chat", data);
    }

    
    else if(item == "board")
    {
        chess->Debug(data);
     
        // now unpack the board string
        chess->board->UnpackBoardString(data);
        //chess->board->Draw();
        
        // if we're a server, relay this info to the clients
        if(chess->server) this->PokeEveryoneElse(item, data, size, format);
    }
    
    
    else if(item == "highlights")
    {
        s = data;
        chess->Debug("received highlights: " + s);
        chess->board->UnpackHighlights(data);
        
        // if we're a server, relay this info to the clients
        if(chess->server) PokeEveryoneElse(item, data, size, format);
    }
    
    
    
    
    else chess->Debug("Unknown topic: "+topic);
    
    return wxTCPConnection::OnPoke(topic, item, data, size, format);
}

void MyConnection::SendBoard()
{
    // send the board coordinates
    s = chess->board->AssembleBoardString();
    Poke("board", (wxChar *)s.GetData());
}

void MyConnection::SendHighlights()
{
    // format the string for the highlight coordinates
    s = chess->board->PackHighlights();
    Poke("highlights", (wxChar *)s.GetData());
}

void MyConnection::SendMouse()
{
    // get the coordinates of the mouse from the board
    float x[2];
    x[0] = chess->board->x-0.5;
    x[1] = chess->board->y-0.5;

    if(chess->checkBoxReversed->GetValue())
    {
        x[0] = 9.0-x[0];
        x[1] = 9.0-x[1];
    }
    
    // now assemble the mouse poke string in the form "_k"
    s = "_";
    s = s + chess->board->GetMousePieceChar();
        
    Poke(s, (char *)x, 2*sizeof(float));
}
void MyConnection::SendClientList()
{
    // make sure we're the server
    if(chess->server)
    {
        chess->Debug("SendClientList()");
    
        // first tell the client to clear the list
        Poke("ClearClientList", "");
    
        // send the client name, and they will append it to the list
        for(unsigned int n=0; n<chess->listPeople->GetCount(); n++)
            Poke("AppendClient", (wxChar *)chess->listPeople->GetString(n).GetData());
    }
}

void MyConnection::SendErrthing()
{
    chess->Debug("Sending everything on one connection...");
    SendBoard();
    SendHighlights();
    
    // send the edit mode
    s.sprintf("%i", (int)chess->checkBoxEditMode->GetValue());
    Poke("EditMode", (char *)s.GetData());
    
    // send which toggle is pressed
    chess->SendToggles();
    
    // send the server stuff if we should
    if(chess->server)
    {
        int i = chess->options->spinSpeed->GetValue();
        Poke("Speed", (char *)(&i), sizeof(int));
        i = chess->options->spinMax->GetValue();
        Poke("Max",   (char *)(&i),   sizeof(int));
        
        wxCommandEvent e;
        chess->options->checkBoxRefreshEnabledClick(e);
        chess->options->checkBoxHandsClick(e);
        chess->options->checkBoxNewMethodClick(e);
    }
}

void MyConnection::UpdateEveryoneElse()
{
    MyClientData *c;
    
    // only if we're the server, update all other connections
    if(chess->server)
    {
        for(unsigned int n=1; n<chess->listPeople->GetCount(); n++)
        {
            c = (MyClientData *)chess->listPeople->GetClientData(n);
            if(c->connection != this) c->connection->SendErrthing();
        }
    }
}

void MyConnection::PokeEveryoneElse(const wxString& item, wxChar *data, int size, wxIPCFormat format)
{
    MyClientData *c;

    // only if we're the server, poke all other connections
    if(chess->server != NULL)
    {
        for(unsigned int n=1; n<chess->listPeople->GetCount(); n++)
        {
            c = (MyClientData *)chess->listPeople->GetClientData(n);
            if(c->connection != this) c->connection->Poke(item, data, size, format);
        }
    }
}







MyClient::MyClient(chessFrame *chess) : wxTCPClient()
{
    this->chess = chess;
    connection = NULL;
}

bool MyClient::Connect(const wxString& sHost, const wxString& sService, const wxString& sTopic)
{
    connection = (MyConnection *)MakeConnection(sHost, sService, sTopic);
    
    return connection != NULL;
}

MyClient::~MyClient()
{
    Disconnect();
}

wxConnectionBase *MyClient::OnMakeConnection()
{
    return new MyConnection(chess);
}

void MyClient::Disconnect()
{
    chess->Debug("MyClient::Disconnect()");
    if (connection)
    {
        connection->Disconnect();
        chess->Debug("deleting connection");
        delete connection;
        connection = NULL;
        chess->Log("You have disconnected from the server.");
    }
}

void MyClient::SendMouse()
{
    if(connection) connection->SendMouse();
}

void MyClient::UpdateErrbody()
{
    if(connection) connection->SendErrthing();
}





MyServer::MyServer(chessFrame *chess) : wxTCPServer()
{
    this->chess = chess;
}

MyServer::~MyServer()
{
    for(unsigned int n=1; n<chess->listPeople->GetCount(); n++) Disconnect(n); // dico also deletes
}

wxConnectionBase *MyServer::OnAcceptConnection(const wxString& topic)
{
    chess->Log("Some dude is trying to connect.");
    
    // if the connector has the right topic and we're not full, go for it!
    if(topic == "chessticle")
    {
        if(chess->listPeople->GetCount() <= (unsigned int)chess->options->spinMax->GetValue())
        {
            // append this new guy to the user list
            int n = chess->listPeople->GetCount();
            chess->listPeople->Append("New guy", (wxClientData *)new MyClientData(chess, n));
            
            chess->Log("Accepting connection...");
            
            // now create a new connection object for the client data
            MyClientData *c = (MyClientData *)chess->listPeople->GetClientData(n);
            
            // make sure the new connection object has a handle on the chess frame
            // and on its own index
            c->connection = new MyConnection(c->chess, c->index);
            
            return c->connection;
        }
        else
        {
            chess->Log("Too many connections. Denied!");
            return NULL;
        }
    }
    chess->Log("Wrong topic. Denying access.");
    return NULL;
}


void MyServer::UpdateErrbody()
{
    MyClientData *c;
    
    // loop over all the connected clients and upload this coordinates string
    for(unsigned int n=1; n<chess->listPeople->GetCount(); n++)
    {
        // get the connection object
        c = (MyClientData *)chess->listPeople->GetClientData(n);
        
        // send everything to this client!
        if(c->connection) c->connection->SendErrthing();
        else chess->Log("WTF? Client "+chess->listPeople->GetString(n)+" is a NULL pointer!");
    }
}


void MyServer::Disconnect(unsigned int n)
{
    chess->Debug("MyServer::Disconnect(n)");
    s = chess->listPeople->GetString(n);
    
    // to disconnect someone, first do the TCP disconnect
    MyClientData *c = (MyClientData *)chess->listPeople->GetClientData(n);
    
    // if it's not a NULL connection, dico it
    c->connection->Disconnect();

    // now remove it from the list
    chess->listPeople->Delete(n);

    // now log it
    chess->Log(s + " = pwnx0rd, sir.");  
    
    // now tell everyone
    s = "*** "+chess->listPeople->GetString(n) + " was pwnx0rd by the server.";
    chess->server->PokeEveryone("chat", (wxChar *)s.GetData());
    chess->server->SendEveryoneClientList();    
}

void MyServer::SendEveryoneClientList()
{
    MyClientData *c;
    
    for(unsigned int n=1; n<chess->listPeople->GetCount(); n++)
    {
        c = (MyClientData *)chess->listPeople->GetClientData(n);
        if(c->connection) c->connection->SendClientList();
        else
        {
            s = "SendEveryoneClientList(): Client connection ";
            chess->Log(s << n << " is NULL!");
        }
    }
}

void MyServer::PokeEveryone(const wxString& item, wxChar *data, int size, wxIPCFormat format)
{
    MyClientData *c;
    
    for(unsigned int n=1; n<chess->listPeople->GetCount(); n++)
    {
        c = (MyClientData *)chess->listPeople->GetClientData(n);
        if(c->connection) c->connection->Poke(item, data, size, format);
        else
        {
            s = "PokeEveryone(): Client connection ";
            chess->Log(s << n << " is NULL!");
        }
    
    }       
}

void MyServer::SetIndices()
{
    chess->Debug("MyServer::SetIndices()");
    
    MyClientData *c;
    // loop over all the clients and update their indices
    for(unsigned int n=0; n<chess->listPeople->GetCount(); n++)
    {
        c = (MyClientData *)chess->listPeople->GetClientData(n);
       
        c->index = n;
        if(c->connection) c->connection->index = n;
        
        chess->Debug(chess->listPeople->GetString(n)<<" = "<<n);
    }
}

void MyServer::SendMouse()
{
    // we're the server so we have to send an item "_k#" with # being the char for 0
    s = "_";
    s = s+chess->board->GetMousePieceChar()+(char)0;
    
    // get the float data in the right format
    float x[2];
    x[0] = chess->board->x-0.5;
    x[1] = chess->board->y-0.5;
    
    // reverse the coords if necessary
    if(chess->checkBoxReversed->GetValue())
    {
        x[0] = 9.0-x[0];
        x[1] = 9.0-x[1];
    }
    
    // poke the data
    PokeEveryone(s, (char *)x, 2*sizeof(float));
}



MyClientData::MyClientData(chessFrame *chess, unsigned int index)
{
    chess->Debug("MyClientData()");
    this->chess=chess; 
    this->index=index; 
    connection = NULL;
    floater    = NULL;
        
    x = -1;
    y = -1;
    x_start  = -1;
    y_start  = -1;
    x_target = -1;
    y_target = -1;
}
        
MyClientData::~MyClientData()
{
    chess->Debug("~MyClientData()");
    if(connection != NULL)
    {
        chess->Debug("Connection still exists. Deleting...");
        delete connection;
        connection = NULL;
    }
}



// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include "Server.h"

	string socketIO::read(){
		char c=0;
		string line="";
		while(c!='\n'){
			recv(clientID,&c,sizeof(char),0);
			line +=c;
		}
		return line;
	}

	void socketIO::write(string text){
		const char* txt=text.c_str();
		send(clientID,txt,strlen(txt),0);		
	}

	void socketIO::write(float f){
		ostringstream outString;
		outString <<f;
		string s(outString.str());
		write(s);
	}

	void socketIO::read(float* f){}


Server::Server(int port)throw (const char*) {
	  stopped=false;
	  fd=socket(AF_INET,SOCK_STREAM,0);
	  if(fd<0)
		throw "socket failed";

	  server.sin_family = AF_INET;
	  server.sin_addr.s_addr = INADDR_ANY;
	  server.sin_port = htons(port);

	  if(bind(fd,(struct sockaddr*)&server, sizeof(server))<0)
		throw "bind failure";

	  if(listen(fd, 3)<0)
		throw "listen failure";
}


void Server::start(ClientHandler& ch)throw(const char*){	
	  t=new thread([&ch,this](){
          while(!stopped){
              socklen_t clientSize=sizeof(client);
				int aClient = accept(fd,(struct sockaddr*)&client,&clientSize);
                if(aClient>0){
                    ch.handle(aClient);
						close(aClient);
				}
		    }
		    close(fd);
	  });
}

void Server::stop(){
	stopped=true;
	t->join(); // do not delete this!
}

Server::~Server() {
	// TODO Auto-generated destructor stub
}


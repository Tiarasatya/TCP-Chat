#include<iostream>
#include<WS2tcpip.h>
#include<string>
#include<string.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

void main()
{
	//Initialize winsock (Windows socket API)
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "Can't initialize winsock! Quitting" << endl;
		return;
	}

	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	//Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(5400);  /*htons = Host to Network Short, 5400 = port*/
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //Could also use inet_pton ...

	if (bind(listening, (sockaddr*)&hint, sizeof(hint)) != 0)
	{
		cout<<"Error Binding"<<endl;
	}
	//Tell winsock the socket is for listening
	if (listen(listening, SOMAXCONN) != 0)
	{
		cout << "Error Listening" << endl;
	}
	else
	{
		cout << "Server is Listening..." << endl;
	}

	//Wait for a connection
	sockaddr_in client;
	int clientsize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*) &client, &clientsize);

	char host[NI_MAXHOST]; //Client's remote name
	char service[NI_MAXHOST]; //port for connected the client

	ZeroMemory(host, NI_MAXHOST); //same as memset(host, 0, NI_MAXHOST
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) ==0)
	{
		cout << host << " connected to port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}
	//Close listening socket
	closesocket(listening);
	//While loop for accept and echo message back to client
	//so that the Server can send messages to the Client
	char buff[4096];
	string userInput;
	cout << "Client join in chatroom" << endl;
	cout << "---------------------------" << endl;
	while (true)
	{
		ZeroMemory(buff, 4096);
		//wait for client
		int bytesReceived = recv(clientSocket, buff, 4096, 0);
		cout << "Client : ";
		if (bytesReceived == SOCKET_ERROR)
		{
			cout << "Error in recv(). Quit!" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "Client disconnected" << endl;
			break;
		}
		cout << string(buff, 0, bytesReceived) << endl;
		//response message for client
		cout << "...... : ";
		getline(cin, userInput);
		send(clientSocket, userInput.c_str(), bytesReceived + 1, 0);
	}
	//Close the sock
	closesocket(clientSocket);
	//Shutdown winsock
	WSACleanup;
}
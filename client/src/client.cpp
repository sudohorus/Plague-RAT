#include <winsock2.h>
#include <iostream>

int main(){
    WSADATA wsa; //structure to hold winsock data
    SOCKET s; //socket descriptor
    struct sockaddr_in server; //structure to store server address information

    //initialize WinSock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        std::cerr << "[ERROR] Winsock initialization failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    //creating the socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        std::cerr << "[ERROR] Socket creation failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    //setting up the server address
    server.sin_addr.s_addr = inet_addr("192.168.3.10"); //server IP address
    server.sin_family = AF_INET; //IPv4
    server.sin_port = htons(8080); //port 8080
    
    //connecting to the server
    if(connect(s, (struct sockaddr*)&server, sizeof(server)) < 0){
        std::cerr << "[ERROR] Connection to server failed" << std::endl;
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    //sending message to the server
    const char *message = "Client connected";
    send(s, message, strlen(message), 0); //sending message over the socket
    
    //closing the socket and cleaning up winsock
    closesocket(s);
    WSACleanup();
    return 0;
}

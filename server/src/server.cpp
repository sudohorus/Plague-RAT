#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

//main function that runs the server
int main(){
    int server_fd, new_socket; //file descriptors for the server socket and the new socket that will accept connections
    struct sockaddr_in address; //structure to store servber address information
    int addrlen = sizeof(address); //length of the address structure
    char buffer[1024] = {0}; //buffer to store data received from the client
    
    //creating the socket
    //AF_INET = IPv4
    //SOCK_STREAM = Transmission Control Protocol (TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    //checking if the sockett was created sucessfully
    if (server_fd == 0) {
        std::cerr << "[ERROR] Socket creation failed" << std::endl;
        return -1; //if it fails, return error
    }

    //setting up the server address
    address.sin_family = AF_INET; //setting the IPv4
    address.sin_addr.s_addr = INADDR_ANY; //accept connections from any IP
    address.sin_port = htons(8080); //port 8080

    //binding the socket to the specified address (ip and port)
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        std::cerr << "[ERROR] Bind failed" << std::endl;
        return -1;
    }

    //listening for incoming connections (maximum queue size of 3)
    if (listen(server_fd, 3) < 0){
        std::cerr << "[ERROR] Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Waiting for connections..." << std::endl;

    //accepting a new connection from a client
    //this will block until a connection is received
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0){
        std::cerr << "[ERROR] Accept failed" << std::endl;
        return -1;
    }

    //reading data from the client into the buffer
    read(new_socket, buffer, 1024);
    std::cout << "Received: " << buffer << std::endl; //display the received message
    
    //closing the new socket after communication is done
    close(new_socket);
    return 0;
}

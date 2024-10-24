#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

// Função para enviar um comando ao cliente
void sendCommand(int socket, const std::string& command) {
    send(socket, command.c_str(), command.length(), 0);
}

int main() {
    int server_fd, new_socket;// Descritores de arquivo para o socket do servidor e o novo socket que aceitará conexões
    struct sockaddr_in address; // Estrutura para armazenar informações do endereço do servidor
    int addrlen = sizeof(address); // Comprimento da estrutura de endereço
    char buffer[1024] = {0}; // Buffer para armazenar dados recebidos do cliente

    // Criando o socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "[ERROR] Socket creation failed" << std::endl;
        return -1; // Se falhar, retorna erro
    }

    // Configurando o endereço do servidor
    address.sin_family = AF_INET; // Configurando o IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Aceitando conexões de qualquer IP
    address.sin_port = htons(8080); // Porta 8080

    // Vinculando o socket ao endereço especificado (IP e porta)
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "[ERROR] Bind failed" << std::endl;
        return -1;
    }

    // Escutando por conexões (tamanho máximo da fila de 3)
    if (listen(server_fd, 3) < 0) {
        std::cerr << "[ERROR] Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Waiting for connections..." << std::endl;

    // Aceitando uma nova conexão de um cliente
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "[ERROR] Accept failed" << std::endl;
        return -1;
    }

    // Enviando comandos para o cliente
    std::string screenshotCommand = "screenshot"; // Comando para capturar a tela
    sendCommand(new_socket, screenshotCommand);
    std::cout << "Screenshot command sent!" << std::endl;

    std::string cameraCommand = "capture_camera"; // Comando para capturar a imagem da câmera
    sendCommand(new_socket, cameraCommand);
    std::cout << "Camera capture command sent!" << std::endl;

    // Lendo dados do cliente (opcional, se você quiser receber dados de volta)
    read(new_socket, buffer, 1024);
    std::cout << "Received from client: " << buffer << std::endl; // Exibindo a mensagem recebida do cliente

    // Fechando o novo socket após a comunicação
    close(new_socket);
    close(server_fd); // Fechar o socket do servidor
    return 0;
}


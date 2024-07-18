#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <arpa/inet.h>

int tcp_socket_fd = -1;
int client_socket_fd = -1;

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
    if (tcp_socket_fd != -1)
    {
        std::cout << "Closing the listening socket." << std::endl;
        close(tcp_socket_fd);
    }
    if (client_socket_fd != -1)
    {
        std::cout << "Closing the client socket." << std::endl;
        close(client_socket_fd);
    }
    exit(signum);
}

int main()
{
    signal(SIGINT, signalHandler);

    tcp_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket_fd == -1)
    {
        perror("Socket creation failed.");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket created successfully." << std::endl;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    int bind_result = bind(tcp_socket_fd, (const struct sockaddr *)&address, sizeof(address));
    if (bind_result == -1)
    {
        perror("Socket bind failed.");
        close(tcp_socket_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Socket bounded successfully." << std::endl;

    int listen_status = listen(tcp_socket_fd, 3);
    if (listen_status == -1)
    {
        perror("Listen failed on socket.");
        close(tcp_socket_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Socket created and listening on port 8080." << std::endl;

    struct sockaddr_in client_address;
    socklen_t client_addrlen = sizeof(client_address);

    client_socket_fd = accept(tcp_socket_fd, (struct sockaddr *)&client_address, &client_addrlen);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_address.sin_port);

    std::cout << "Accepted connection from " << client_ip << ":" << client_port << std::endl;

    char recv_buffer[1024] = {0};
    ssize_t bytes_read = read(client_socket_fd, recv_buffer, 1024);
    if (bytes_read > 0)
    {
        std::cout << "Received message: " << std::string(recv_buffer, bytes_read) << std::endl;
    }

    close(client_socket_fd);

    close(tcp_socket_fd);
    return 0;
}
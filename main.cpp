#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

int tcp_socket_fd = -1;

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
    if (tcp_socket_fd != -1)
    {
        std::cout << "Closing the socket." << std::endl;
        close(tcp_socket_fd);
    }
    exit(signum);
}

int main()
{
    signal(SIGINT, signalHandler);

    int tcp_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

    close(tcp_socket_fd);
    return 0;
}
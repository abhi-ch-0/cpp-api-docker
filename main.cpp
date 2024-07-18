#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int tcp_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket_fd == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    close(tcp_socket_fd);
    return 0;
}
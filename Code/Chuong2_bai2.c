#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <port> <greeting_file> <data_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, client_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    // Tạo socket cho server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Thiết lập địa chỉ và cổng cho server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));

    // Bind socket với địa chỉ và cổng
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) == -1) {
        perror("Error listening for incoming connections");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len)) == -1) {
        perror("Error accepting incoming connection");
        exit(EXIT_FAILURE);
    }

    // Gửi xâu chào đến client
    FILE *greeting_file = fopen(argv[2], "r");
    if (greeting_file == NULL) {
        perror("Error opening greeting file");
        exit(EXIT_FAILURE);
    }
    fgets(buffer, BUFFER_SIZE, greeting_file);
    send(client_fd, buffer, strlen(buffer), 0);
    fclose(greeting_file);

    // Ghi nội dung client gửi đến vào file
    FILE *data_file = fopen(argv[3], "w");
    if (data_file == NULL) {
        perror("Error opening data file");
        exit(EXIT_FAILURE);
    }
    int recv_bytes;
    while ((recv_bytes = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), recv_bytes, data_file);
    }
    if (recv_bytes == -1) {
        perror("Error receiving data from client");
        exit(EXIT_FAILURE);
    }
    fclose(data_file);

    // Đóng kết nối
    close(client_fd);
    close(server_fd);

    return 0;
}
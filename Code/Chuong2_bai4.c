#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024

void log_message(FILE *file, char *message) {
    time_t current_time;
    char *formatted_time;
    
    current_time = time(NULL);
    formatted_time = ctime(&current_time);
    formatted_time[strlen(formatted_time)-1] = '\0';
    
    fprintf(file, "[%s] %s\n", formatted_time, message);
}

int main(int argc, char *argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[MAX_BUFFER_SIZE];
    FILE *log_file;
    int port;
    char *log_file_name;

    if (argc < 3) {
        printf("Usage: %s port log_file_name\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);
    log_file_name = argv[2];

    log_file = fopen(log_file_name, "a");
    if (log_file == NULL) {
        printf("Error: Cannot open log file.\n");
        exit(1);
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error: Failed to create socket.\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error: Failed to bind socket.\n");
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        printf("Error: Failed to listen on socket.\n");
        exit(1);
    }

    printf("Server is listening on port %d.\n", port);
    log_message(log_file, "Server is started.");

    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &addr_size);
        if (client_socket == -1) {
            printf("Error: Failed to accept client connection.\n");
            continue;
        }

        inet_ntop(AF_INET, &(client_addr.sin_addr), buffer, MAX_BUFFER_SIZE);
        log_message(log_file, "Accepted connection from: ");
        log_message(log_file, buffer);

        memset(buffer, 0, MAX_BUFFER_SIZE);
        if (recv(client_socket, buffer, MAX_BUFFER_SIZE, 0) == -1) {
            printf("Error: Failed to receive data from client.\n");
            close(client_socket);
            continue;
        }

        printf("%s\n", buffer);
        log_message(log_file, buffer);

        close(client_socket);
    }

    close(server_socket);
    fclose(log_file);
    return 0;
}
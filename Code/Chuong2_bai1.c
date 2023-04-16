#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // Kiểm tra định dạng tham số dòng lệnh
    if (argc != 3) {
        printf("Sử dụng: %s <địa chỉ IP> <cổng>\n", argv[0]);
        exit(1);
    }

    // Lấy địa chỉ IP và cổng từ tham số dòng lệnh
    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    // Tạo socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Không thể tạo socket");
        exit(1);
    }

    // Kết nối tới server
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Không thể kết nối tới server");
        exit(1);
    }

    // Gửi dữ liệu từ bàn phím đến server
    char buffer[BUFFER_SIZE];
    int bytes_sent, bytes_received;

    while (1) {
        // Nhận dữ liệu từ bàn phím
        printf("Nhập dữ liệu: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Gửi dữ liệu tới server
        bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            perror("Lỗi khi gửi dữ liệu tới server");
            break;
        }

        // Nhận phản hồi từ server
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("Lỗi khi nhận phản hồi từ server");
            break;
        }

        // Hiển thị phản hồi từ server trên màn hình
        buffer[bytes_received] = '\0';
        printf("Phản hồi từ server: %s\n", buffer);
    }

    // Đóng socket
    close(client_socket);

    return 0;
}

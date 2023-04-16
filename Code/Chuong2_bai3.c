#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

typedef struct {
    char mssv[11];
    char ho_ten[50];
    char ngay_sinh[11];
    float diem_tb;
} SinhVien;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int server_port = 8888;

    // Tạo socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        return 1;
    }

    // Định nghĩa địa chỉ server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // Kết nối tới server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        return 1;
    }

    // Nhập thông tin sinh viên
    SinhVien sv;
    printf("Nhap MSSV: ");
    fgets(sv.mssv, sizeof(sv.mssv), stdin);
    printf("Nhap ho ten: ");
    fgets(sv.ho_ten, sizeof(sv.ho_ten), stdin);
    printf("Nhap ngay sinh (dd/mm/yyyy): ");
    fgets(sv.ngay_sinh, sizeof(sv.ngay_sinh), stdin);
    printf("Nhap diem trung binh: ");
    scanf("%f", &sv.diem_tb);
    getchar(); // Đọc kí tự '\n' còn lại trong bộ đệm

    // Đóng gói dữ liệu
    char buffer[BUFFER_SIZE];
    int len = sprintf(buffer, "%s,%s,%s,%.2f", sv.mssv, sv.ho_ten, sv.ngay_sinh, sv.diem_tb);

    // Gửi dữ liệu tới server
    if (send(sockfd, buffer, len, 0) == -1) {
        perror("Error sending data to server");
        return 1;
    }

    // Đóng kết nối
    close(sockfd);

    return 0;
}
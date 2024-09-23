#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in broadcast_addr; //ip ve port numarasini tutuyor
    int broadcast_permission = 1;
    int kordinat_X1, kordinat_Y1, hiz1, ivme1;

    char message[50];  

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock baslatma hatasi: %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Socket oluşturma hatasi: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast_permission, sizeof(broadcast_permission)) < 0) {
        printf("Broadcast izni alma hatasi: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Broadcast ile agdaki tum cihazlara mesaj gonderiyoruz
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(12345);  //alici ile verici icin ortak port belirliyorum
    broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");  

    // ondeki aracin degerlerini aliyoruz
    while (1) {
        printf("birinci aracin kordinat_X1, kordinat_Y1, Hiz1, ivme1 degerlerini giriniz : ");
        scanf("%d %d %d %d", &kordinat_X1, &kordinat_Y1, &hiz1, &ivme1);

        sprintf(message, "%d %d %d %d", kordinat_X1, kordinat_Y1, hiz1, ivme1);
    //alinan verileri ikinci cihaza gonderme
        if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) == SOCKET_ERROR) {
            printf("Mesaj gönderme hatasi: %d\n", WSAGetLastError());
        } else {
            printf("Broadcast mesaji gönderildi: %s\n", message);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

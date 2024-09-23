#include <stdio.h>
#include <string.h>
#include <math.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  

#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in recv_addr;
    char buffer[BUFFER_SIZE];
    int recv_len;
    int addr_len = sizeof(recv_addr);
    int kordinat_X1, kordinat_Y1, hiz1, ivme1;
    int kordinat_X2, kordinat_Y2, hiz2;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock baslatma hatasi: %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Socket olusturma hatasi: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // gonderici adresi ve portu ortak ayarliyoruz
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(12345); 
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Herhangi bir ip'den gelen yayınları dinler

    if (bind(sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) == SOCKET_ERROR) {
        printf("Bind hatasi: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("UDP broadcast mesajlari dinleniyor...\n");

    while (1) {
        // verileri alma
        recv_len = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&recv_addr, &addr_len);
        if (recv_len == SOCKET_ERROR) {
            printf("Mesaj alma hatasi: %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return 1;
        }
        //öndeki aractan alinan verileri yazdirip arkadaki aracin verilerini istiyoruz
        buffer[recv_len] = '\0';  
        sscanf(buffer, "%d %d %d %d", &kordinat_X1, &kordinat_Y1, &hiz1, &ivme1);  
        printf("Ondeki aractan alinan degerler: kordinat_X1=%d, kordinat_Y1=%d, hiz1=%d ivme1=%d\n", kordinat_X1, kordinat_Y1, hiz1, ivme1);

        printf("arkadaki aracin kordinat_X2, kordinat_Y2, hiz2 degerlerini giriniz:");
        scanf("%d %d %d", &kordinat_X2, &kordinat_Y2, &hiz2);

       //algoritma burda basliyor
       //öndeki aracin ivmesi 30 m/s2 üzeriyse ve arkadaki arac 2 sn uzakliktaysa      
        if (ivme1 > 6) {  
            //burda araclar arasindaki mesafenin kac sn sonra kapanacagını hesapliyorum. km/h m/s cevrimi var.
            double ani_fren_konumuna_kalan_sure = (double)(kordinat_Y1 - kordinat_Y2) / ((hiz2 - hiz1) / 3.6);
            printf("ani fren konumuna kalan sure: = %.2f sn\n", ani_fren_konumuna_kalan_sure);
            if (0 < ani_fren_konumuna_kalan_sure && ani_fren_konumuna_kalan_sure < 3) {
                printf("!!!DIKKAT ANI FREN!!!\n");
            } else {
                printf("ani fren yok\n");
                   }
                        } else {
                printf("ani fren yok\n");
            if (abs(kordinat_X2 - kordinat_X1) > 3) {
                printf("tehlike yok\n");
            } else { 

                    //2. algoritma burada basliyor     
                    // arkadaki arac daha hizli ve öndeki araca 2 sn uzakliktan yakin ise uyari gelmeli
                if (hiz1 < hiz2) {  
                    //burda araclar arasindaki mesafenin kac sn sonra kapanacagını hesapliyorum. km/h m/s cevrimi var.
                    double carpmaya_kalan_sure = (double)(kordinat_Y1 - kordinat_Y2) / ((hiz2 - hiz1) / 3.6); 
                        printf("carpmaya kalan sure: = %.2f sn\n", carpmaya_kalan_sure);

                    if (0 < carpmaya_kalan_sure && carpmaya_kalan_sure < 2) {
                        printf("YAVASLA!\n");
                    } else {
                        printf("Tehlike yok\n");
                    }
                } else {
                        printf("Hizlar esit veya ondeki daha hizli, tehlike yok.\n");
                       }
            }
        }
        printf("UDP mesajlari bekleniyor...\n");
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}
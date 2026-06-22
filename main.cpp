#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8080

struct Message {
    uint16_t size;        // total message size (type + payload)
    uint8_t messageType;
    uint8_t* payload;
};

void Message_init(Message* m, uint8_t type, const uint8_t* data, size_t len) {
    m->size = (uint16_t)(len + 1); // +1 for type
    m->messageType = type;

    m->payload = (uint8_t*)malloc(len);
    if (m->payload && len > 0) {
        memcpy(m->payload, data, len);
    }
}

void Message_destroy(Message* m) {
    free(m->payload);
    m->payload = nullptr;
}

Message convBytesToMessage(uint8_t* bytes) {
    // big-endian size
    uint16_t total_size = (bytes[0] << 8) | bytes[1];

    uint8_t type = bytes[2];

    // payload size = total - type
    uint16_t payload_len = total_size - 1;

    uint8_t* payload = nullptr;

    if (payload_len > 0) {
        payload = (uint8_t*)malloc(payload_len);
        memcpy(payload, &bytes[3], payload_len);
    }

    Message m;
    Message_init(&m, type, payload, payload_len);

    free(payload);

    return m;
}

int main() {
  int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        printf("Client Connected!"); 
        char buffer[6] = {0};
        valread = read( new_socket , buffer, 6);
        for (auto elem : buffer){
          printf("%u\n",elem);
        }
        close(new_socket);
    }
    return 0;
}

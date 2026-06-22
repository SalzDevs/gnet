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
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  bind(server_fd, (struct sockaddr*)&address, sizeof(address));
  listen(server_fd, 10);

  while (1) {
    printf("Waiting for connection...\n");

    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    printf("Client connected!\n");

    // Read header (2 bytes)
    uint8_t header[2];
    read(new_socket, header, 2);

    uint16_t size = (header[0] << 8) | header[1];

    // Read rest of message
    uint8_t* buffer = (uint8_t*)malloc(size);

    size_t total_read = 0;
    while (total_read < size) {
      ssize_t r = read(new_socket, buffer + total_read, size - total_read);
      if (r <= 0) break;
      total_read += r;
    }

    // Build full packet (header + rest)
    uint8_t* full = (uint8_t*)malloc(size + 2);
    memcpy(full, header, 2);
    memcpy(full + 2, buffer, size);

    // Parse message
    Message m = convBytesToMessage(full);

    printf("Message: size=%u type=%u\n", m.size, m.messageType);

    for (int i = 0; i < m.size - 1; i++) {
      printf("%u\n", m.payload[i]);
    }

    free(buffer);
    free(full);
    free(m.payload);

    close(new_socket);
  }
}

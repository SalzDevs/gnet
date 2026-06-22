#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

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
    uint8_t message_bytes[] = {
        0x00, 0x05,   // total size = 5 (1 type + 4 payload)
        0x01,         // type
        0x01, 0x01, 0x01, 0x01
    };

    Message test_message = convBytesToMessage(message_bytes);

    printf("Message { size: %u, type: %u }\n",
           test_message.size,
           test_message.messageType);

    for (size_t i = 0; i < test_message.size - 1; i++) {
        printf("%u\n", test_message.payload[i]);
    }

    Message_destroy(&test_message);
    return 0;
}

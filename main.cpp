#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

struct Message {
    uint16_t size;
    uint8_t messageType;
    uint8_t* payload;
};

void Message_init(struct Message* m, uint8_t type, const uint8_t* data, size_t len) {
    m->size = (uint16_t)(len + 1);
    m->messageType = type;
    m->payload = (uint8_t*)malloc(len);
    if (m->payload) {
        memcpy(m->payload, data, len);
    }
}

void Message_destroy(struct Message* m) {
    free(m->payload);
    m->payload = NULL;
}

int main() {
    const char* text = "Hello man";
    size_t len = strlen(text);

    struct Message m;
    Message_init(&m, 1, (const uint8_t*)text, len);

    printf("Message { size: %u, messageType: %u, payload size: %zu }\n", 
           m.size, m.messageType, len);
    printf("size: %u\n", m.size);
    printf("messageType size: %zu\n", sizeof(m.messageType));
    printf("payload: %zu\n", len);

    Message_destroy(&m);
    return 0;
}
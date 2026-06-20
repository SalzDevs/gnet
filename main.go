package main

type Message struct {
	// size is the total number of bytes that follow this field in the stream.
	// It includes:
	//   - messageType (1 byte)
	//   - payload (variable length)
	// Maximum value: 65535 (uint16)
	size uint16

	// messageType identifies how the payload should be interpreted.
	messageType uint8

	// payload contains the raw message data.
	// Its maximum size is derived from the size field:
	// maxPayload = 65535 - 1 = 65534 bytes
	// because 1 byte is used by messageType.
	payload []byte
}

func main(){
	println("Hello seamen!")
}

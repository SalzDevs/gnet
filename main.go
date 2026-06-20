package main

import (
	"fmt"
	"unsafe"
)

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

func NewMessage(messageType uint8, payload []byte) Message{
	return Message {
		size: uint16(len(payload)+1),
		messageType: messageType,
		payload: payload,
	}	
}

func main(){
	m := NewMessage(1, []byte("Hello man")) 
	fmt.Printf("%+v\n", m)

	println("size:",m.size)
	println("messageType size:",unsafe.Sizeof(m.messageType))
	println("payload:",len(m.payload))
}

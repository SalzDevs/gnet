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

func validSize(m Message) error {
	expected := int(m.size)
	actual := 1 + len(m.payload)
	
	if expected == actual {
		return nil
	}

	if actual < expected {
		return fmt.Errorf("size mismatch: expected %d, got %d (short)", expected, actual)
	}
	
	return fmt.Errorf("size mismatch: expected %d, got %d (long)", expected, actual)
}

func main(){
	m := Message{1,1,[]byte("Hello man")}
	fmt.Printf("%+v\n", m)
	println("size:",m.size)
	println("messageType size:",unsafe.Sizeof(m.messageType))
	println("payload:",len(m.payload))
	err := validSize(m)
	if err != nil{
		println(err.Error())
		return
	}
	println("Nice the size is correct!!!")
}

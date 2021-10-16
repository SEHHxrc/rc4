package main

import (
	"errors"
	"fmt"
)

type RC4 struct {
	s    [256]uint8
	t    [256]uint8
	key  []byte
	i, j uint8
}

func New(key []byte) (*RC4, error) {
	if len(key) > 256 {
		return nil, errors.New("incorrect key length")
	}
	rc4 := &RC4{key: key}
	rc4.ksa()
	return rc4, nil
}

func (rc4 *RC4) Reset() {
	rc4.i, rc4.j = 0, 0
	rc4.ksa()
}

func (rc4 *RC4) ksa() { //initialization
	for i := 0; i < 256; i++ {
		rc4.s[i] = uint8(i)
		rc4.t[i] = rc4.key[i%len(rc4.key)]
	}
	j := uint8(0)
	for i := 0; i < 256; i++ {
		j = (j + rc4.s[i] + rc4.t[i])
		rc4.s[i], rc4.s[j] = rc4.s[j], rc4.s[i]
	}
}

func (rc4 *RC4) prga() byte { //Generate stream key
	rc4.i += 1
	rc4.j += rc4.s[rc4.i]
	rc4.s[rc4.i], rc4.s[rc4.j] = rc4.s[rc4.j], rc4.s[rc4.i]
	t := rc4.s[rc4.i] + rc4.s[rc4.j]
	return rc4.s[t]
}

func (rc4 *RC4) Encrypt(plaintext []byte) (ciphertext []byte) {
	ciphertext = rc4.xor(plaintext)
	return
}

func (rc4 *RC4) Decrypt(ciphertext []byte) (plaintext []byte) {
	plaintext = rc4.xor(ciphertext)
	return
}

func (rc4 *RC4) xor(text []byte) (result []byte) { //Encryption and decryption use the same algorithm
	for _, v := range text {
		result = append(result, v^rc4.prga())
	}
	return
}

func Test() {
	key := []byte{0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}
	rc4, _ := New(key)
	plaintext := []byte{0x74, 0x94, 0xc2, 0xe7, 0x10, 0x4b, 0x08, 0x79}
	cipher := rc4.Encrypt(plaintext)
	fmt.Println(cipher)
	rc4.Reset() //Reset s box
	fmt.Println(rc4.Decrypt(cipher))
}

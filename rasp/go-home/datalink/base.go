package datalink

import "fmt"

type baseConf struct {
	dest       int
	src        int
	packetType int
	body       []int
}

type baseProtocol struct {
	dest       int
	src        int
	packetType int
	length     int
	crc        string

	body    []int
	bodyCrc string
}

func NewBase(conf baseConf) baseProtocol {
	return baseProtocol{
		dest:       conf.dest,
		src:        conf.src,
		packetType: conf.packetType,
		length:     0,
		crc:        "",

		body:    conf.body,
		bodyCrc: "",
	}
}

func (b *baseProtocol) Generate() []int {
	dest := intToBin(b.dest, 16)
	src := intToBin(b.src, 16)
	packetType := intToBin(b.packetType, 8)
	length := intToBin(8, 8)

	// append information to header
	header := []int{}
	header = append(header, dest...)
	header = append(header, src...)
	header = append(header, packetType...)
	header = append(header, length...)

	// calculate checksum of the header
	crc, _ := sumBin(header, 8)
	crc = complementBin(crc)

	// append checksum to our header
	header = append(header, crc...)

	// body
	crc, _ = sumBin(b.body, len(b.body))
	crc = complementBin(crc)

	final := []int{}
	final = append(final, header...)
	final = append(final, b.body...)
	final = append(final, crc...)

	// return our header
	fmt.Println(final)
	return final
}

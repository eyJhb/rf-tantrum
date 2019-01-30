package datalink

import (
	"fmt"

	"go-home/physical"
)

type DatalinkConfig struct {
	Physical physical.Phy
	Src      int
}

type Datalink struct {
	phy physical.Phy
	src int
}

func New(conf DatalinkConfig) Datalink {

	return Datalink{
		phy: conf.Physical,
		src: conf.Src,
	}
}

type basepacket struct {
	Dest       int
	Src        int
	PacketType int
	Length     int
	Headercrc  []int
	Body       []int
	Bodycrc    []int
}

func (d *Datalink) SendData(dest int, packetType int, data []int) {
	p := NewBase(baseConf{
		dest:       dest,
		src:        0,
		packetType: packetType,
		body:       data,
	})

	d.phy.TransmitData(p.Generate())
}

func (d *Datalink) ReadData() (basepacket, error) {
	// wait until we have syncronised
	err := d.phy.Sync()
	if err != nil {
		return basepacket{}, err
	}

	// init our struct
	p := basepacket{}

	header, err := d.phy.ReceiveData(56)

	if err != nil {
		return p, err
	}

	p.Dest = binToInt(header[:16])
	p.Length = binToInt(header[40:48])

	// if p.Dest != 0 {
	// 	fmt.Println("Not our device id..")
	// 	return
	// }

	fmt.Printf("Length: %d\n", p.Length)
	p.Body, err = d.phy.ReceiveData(p.Length)
	if err != nil {
		return p, err
	}

	p.Bodycrc, err = d.phy.ReceiveData(8)
	if err != nil {
		return p, err
	}

	headerSum, err := sumBin(header[:56], 8)
	if err != nil {
		return p, err
	}

	if !complementVerify(headerSum) {
		return p, err
	}
	fmt.Println("Verified header!")

	bodySum, err := sumBin(p.Body, 8)
	if err != nil {
		return p, err
	}

	bodyAdd := addBin(bodySum, p.Bodycrc)
	if !complementVerify(bodyAdd) {
		return p, err
	}
	fmt.Println("Verified body!")

	p.Src = binToInt(header[16:32])
	p.PacketType = binToInt(header[32:40])

	return p, nil
}

package main

import (
	"fmt"
	"time"

	"go-home/datalink"
	"go-home/physical"
)

func main() {
	c, err := physical.New(physical.PhysicalConfig{
		Rx_pin: 2,
		Tx_pin: 3,
	})

	if err != nil {
		fmt.Println("A error occured")
	}

	// initialize datalink
	d := datalink.New(datalink.DatalinkConfig{
		Physical: c,
		Src:      0,
	})

	// functions somewhat as our application layer
	// as it requests data from the datalink layer
	testReceive(d)
	// testTransmit(d)
}

func testReceive(d datalink.Datalink) {
	for {
		// returns a basepacket
		pack, err := d.ReadData()
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(pack.Dest)
	}

}

func testTransmit(d datalink.Datalink) {
	for {
		d.SendData(1000, 2, []int{0, 0, 1, 1, 0, 0, 1, 0})
		time.Sleep(1000 * time.Millisecond)
	}

}

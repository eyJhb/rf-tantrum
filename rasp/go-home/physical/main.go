package physical

import (
	"errors"
	"fmt"
	"time"

	rpio "github.com/stianeikeland/go-rpio"
)

// This program is just going to be
// a POC of the sender working.
// This should and WILL be cleaned up
// I do not care, I will not fucking
// write the report without having
// the time to do this properly

// Raspberry Pi 3 GPIO
// https://i.imgur.com/PPFf2ul.png

const SAMPLE_SIZE = 16

var sfd = [8]int{1, 0, 1, 0, 1, 0, 1, 1}
var preamble = [8]int{1, 0, 1, 0, 1, 0, 1, 0}

type PhysicalConfig struct {
	Rx_pin int
	Tx_pin int
}

type Phy struct {
	rx_pin rpio.Pin
	tx_pin rpio.Pin

	avgTime  time.Duration
	syncLast int64
}

func New(conf PhysicalConfig) (Phy, error) {
	// initialize go-rpio
	err := rpio.Open()

	if err != nil {
		return Phy{}, err
	}

	// set our pin as a input
	rx_pin := rpio.Pin(conf.Rx_pin)
	rx_pin.Input()

	// set our pin as a output
	tx_pin := rpio.Pin(conf.Tx_pin)
	tx_pin.Output()

	return Phy{
		rx_pin:  rx_pin,
		tx_pin:  tx_pin,
		avgTime: 0,
	}, nil
}

func (c *Phy) readBit() int {
	state := c.rx_pin.Read()
	for (getMicros() - c.syncLast) < int64(c.avgTime) {
	}
	c.syncLast = getMicros()
	return int(state)
}

func (c *Phy) sendBit(bit int) {
	if bit == 1 {
		c.tx_pin.High()
	} else {
		c.tx_pin.Low()
	}

	time.Sleep(10 * time.Millisecond)
}

// reads x bits
func (c *Phy) ReceiveData(readings int) ([]int, error) {
	// multiply by 2 => manchester encoded
	readings = readings * 2
	manBits := []int{}
	ticker := time.NewTicker(c.avgTime * time.Microsecond)
	go func() {
		for _ = range ticker.C {
			manBits = append(manBits, int(c.rx_pin.Read()))
		}
	}()

	for {
		if len(manBits) >= readings {
			ticker.Stop()
			break
		}
	}

	bits, err := decodeManchester(manBits)

	if err != nil {
		return nil, err
	}

	return bits, nil
}

func (c *Phy) TransmitData(data []int) {
	fmt.Println("Sending data..")
	// calculate our manchester data
	manData := encodeManchester(data)

	final_data := make([]int, 500)
	// send our preamble
	for i := 0; i < 7; i++ {
		// final_data = append(final_data, preamble...)
		for j := 0; j < 8; j++ {
			final_data[(i*8)+j] = preamble[j]
		}
	}

	// send our sfd
	for j := 0; j < 8; j++ {
		final_data[56+j] = sfd[j]
	}

	// send our manchester encoded data
	for k, v := range manData {
		final_data[64+k] = v
	}

	// send our sfd
	for j := 0; j < 8; j++ {
		final_data[64+len(manData)+j] = sfd[j]
	}

	for _, v := range final_data {
		c.sendBit(v)
	}

	fmt.Println("Sent data..")
}

func (c *Phy) checkPreamble() int64 {
	count := 0
	lastChange := getMicros()
	lastState := c.rx_pin.Read()
	samples := [SAMPLE_SIZE]int64{}

	for {
		state := c.rx_pin.Read()

		if state != lastState {
			var changeTime int64 = getMicros() - lastChange

			if checkBetween(changeTime, 9000, 11000) {
				// ringbuffer
				samples[count%SAMPLE_SIZE] = changeTime
				count++
			} else {
				count = 0
			}

			// (7*8) we send 7 times our preamble (8 bits)
			if count == 56 {
				break
			}

			lastChange = getMicros()
			lastState = state
		}
	}

	// calculate our average time
	var avg int64 = 0
	for _, v := range samples {
		avg += v
	}

	return avg / SAMPLE_SIZE
}

func (c *Phy) checkSfd() bool {
	ticker := time.NewTicker(c.avgTime * time.Microsecond)
	bits := []int{}
	go func() {
		for _ = range ticker.C {
			bits = append(bits, int(c.rx_pin.Read()))
		}
	}()

	index := 0
	con := 0
	for {
		if len(bits) >= 8 {
			return false
		}
		if len(bits)-1 < index {
			continue
		}
		if bits[index] == 1 {
			con++
		} else {
			con = 0
		}
		if con == 2 {
			ticker.Stop()
			return true
		}

		index++
	}

	return false
}

func (c *Phy) Sync() error {
	// wait until we have a sync so we have the average
	c.avgTime = time.Duration(c.checkPreamble())
	c.syncLast = getMicros()

	fmt.Println("We have synced!")
	fmt.Printf("Sleep: %d, Average: %d\n", c.avgTime/2, c.avgTime)

	// wait the avg / 2 - hit the middle
	time.Sleep((c.avgTime / 2) * time.Microsecond)

	if !c.checkSfd() {
		errors.New("Could not get SFD")
	}
	fmt.Println("Passed SFD!")
	fmt.Println("----")
	return nil
}

package physical

import (
	"time"
)

func encodeManchester(data []int) []int {
	manData := []int{}

	for _, v := range data {
		if v == 1 {
			manData = append(manData, 0)
			manData = append(manData, 1)
		} else {
			manData = append(manData, 1)
			manData = append(manData, 0)
		}
	}

	return manData
}

func decodeManchester(data []int) ([]int, error) {
	manData := []int{}

	for i := 0; i < len(data)/2; i++ {
		if data[(i*2)] == 0 && data[(i*2)+1] == 1 {
			manData = append(manData, 1)
		} else if data[(i*2)] == 1 && data[(i*2)+1] == 0 {
			manData = append(manData, 0)
		} else {
			manData = append(manData, 8)
			// return nil, errors.New("decode manchester failed")
		}
	}

	return manData, nil
}

func checkBetween(v, lower, upper int64) bool {
	if (v > lower) && (v < upper) {
		return true
	}
	return false
}

func getMicros() int64 {
	return time.Now().UnixNano() / int64(time.Microsecond)
}

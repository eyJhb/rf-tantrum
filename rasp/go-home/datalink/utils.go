package datalink

import (
	"errors"
)

func reverse(v []int) []int {
	for i := 0; i < len(v)/2; i++ {
		j := len(v) - i - 1
		v[i], v[j] = v[j], v[i]
	}
	return v
}

func pad(v []int, pad int) []int {
	rv := []int{}
	for i := 0; i < pad-len(v); i++ {
		rv = append(rv, 0)
	}

	for _, vv := range v {
		rv = append(rv, vv)
	}

	return rv
}

func intToBin(v int, padding int) []int {
	binary := []int{}
	for {
		binary = append(binary, v%2)
		v = v / 2
		if v == 0 {
			break
		}
	}

	return pad(reverse(binary), padding)
}

func binToInt(v []int) int {
	sum := 0
	for _, vv := range v {
		sum = (2 * sum) + vv
	}
	return sum
}

func addBin(x []int, y []int) []int {
	carry := 0
	res := make([]int, len(x))
	for j := 7; j >= 0; j-- {
		add := y[j] + x[j] + carry
		carry = 0

		if add == 3 {
			carry = 1
			res[j] = 1
		} else if add == 2 {
			carry = 1
			res[j] = 0
		} else {
			res[j] = add
		}
	}

	if carry == 1 {
		if res[7] == 1 {
			res[7] = 0
		} else {
			res[7] = 1
		}
	}

	return res
}

func sumBin(v []int, splitSize int) ([]int, error) {
	if len(v)%splitSize != 0 {
		errors.New("value is not dividable vy the splisize")
	}

	// set initial previous word
	prevWord := v[0:8]

	for i := 1; i < (len(v) / splitSize); i++ {
		curWord := v[i*8 : (i*8)+8]
		prevWord = addBin(curWord, prevWord)
	}

	return prevWord, nil
}

func complementBin(v []int) []int {
	vv := []int{}
	for _, v := range v {
		if v == 1 {
			vv = append(vv, 0)
		} else {
			vv = append(vv, 1)
		}
	}

	return vv
}

func complementVerify(v []int) bool {
	for _, v := range v {
		if v != 1 {
			return false
		}
	}
	return true
}

// Copyright © 2013 Galvanized Logic Inc.
// Use is governed by a FreeBSD license found in the LICENSE file.

// Package lin provides a linear math library that includes vectors,
// matrices, quaternions, and transforms. Linear math operations are useful
// in 3D applications for describing virtual objects and simulating physics.
//
// Package lin is provided as part of the vu (virtual universe) 3D engine.
package lin

// Design Notes:
//
// 1) This is a CPU based 3D math library. It is most often called from
//    rendering loops where performance is key. Some general guidelines,
//    verified with benchmarks, can be seen throughout the library.
//     - avoid instantiating new structures
//     - use pointers to structures
//     - prefer multiply over divide
//
// 2) Optimized/performant 3D math is expected to be done on the GPU.
//    A future library may address this.
//
// 3) Wikipedia states: "In linear algebra, real numbers are called scalars...".
//    Currently the default scalar size is float64 since the underlying go math
//    package uses this size.

import "math"

// Various linear math constants.
const (

	// PI and its commonly needed varients.
	PI      float64 = math.Pi
	PIx2    float64 = PI * 2
	HALF_PI float64 = PIx2 * 0.25
	DEG_RAD float64 = PIx2 / 360.0 // X degrees * DEG_RAD = Y radians
	RAD_DEG float64 = 360.0 / PIx2 // Y radians * RAD_DEG = X degrees

	// Convenience numbers.
	LARGE float64 = math.MaxFloat32
	SQRT2 float64 = math.Sqrt2
	SQRT3 float64 = 1.73205

	// EPSILON is used to distinguish when a float is close enough to a number.
	// Wikipedia: "In set theory epsilon is the limit ordinal of the sequence..."
	EPSILON float64 = 0.000001
)

// Rad converts degrees to radians.
func Rad(deg float64) float64 { return deg * DEG_RAD }

// Degrees converts radians to degrees.
func Deg(rad float64) float64 { return rad * RAD_DEG }

// IsNeg returns true if x is less than zero.
func IsNeg(x float64) bool { return x < 0 }

// AeqZ (~=) almost-equals returns true if the difference between x and zero
// is so small that it doesn't matter.
func AeqZ(x float64) bool { return math.Abs(x) < EPSILON }

// Aeq (~=) almost-equals returns true if the difference between a and b is
// so small that it doesn't matter.
func Aeq(a, b float64) bool { return math.Abs(a-b) < EPSILON }

// Lerp returns the linear interpolation of a to b by the given ratio.
func Lerp(a, b, ratio float64) float64 { return (b-a)*ratio + a }

// Max3 returns the largest of the 3 numbers.
func Max3(a, b, c float64) float64 { return math.Max(a, math.Max(b, c)) }

// Min3 returns the smallest of the 3 numbers.
func Min3(a, b, c float64) float64 { return math.Min(a, math.Min(b, c)) }

// Atan2F is a fast approximation of Atan2. Source was posted at:
//    http://www.gamedev.net/topic/441464-manually-implementing-atan2-or-atan/
func Atan2F(y, x float64) float64 {
	coeff_1 := PI / 4
	coeff_2 := 3 * coeff_1
	abs_y := math.Abs(y)
	var angle float64
	if x >= 0 {
		r := (x - abs_y) / (x + abs_y)
		angle = coeff_1 - coeff_1*r
	} else {
		r := (x + abs_y) / (abs_y - x)
		angle = coeff_2 - coeff_1*r
	}
	if y < 0 {
		return -angle
	}
	return angle
}

// Clamp returns a scalar value (one of: s, lb, ub) guaranteed to be within
// the range given by lower bound lb and upper bound ub.
func Clamp(s, lb, ub float64) float64 {
	switch {
	case s < lb:
		return lb
	case s > ub:
		return ub
	}
	return s
}

// Nang (normalize angle) ensures a rotation angle in radians is within the
// range [-PI, PI] (2PI*radians is 360 degrees).
func Nang(radians float64) float64 {
	radians = math.Mod(radians, PIx2)
	switch {
	case radians < -PI:
		return radians + PIx2
	case radians > PI:
		return radians - PIx2
	}
	return radians
}

// Round return rounded version of x with prec precision.
// Special cases are:
//	  Round(±0) = ±0
//	  Round(±Inf) = ±Inf
//	  Round(NaN) = NaN
func Round(val float64, prec int) float64 {
	var rounder float64
	pow := math.Pow(10, float64(prec))
	intermed := val * pow
	if intermed < 0.0 {
		intermed -= 0.5
	} else {
		intermed += 0.5
	}
	rounder = float64(int64(intermed))
	return rounder / float64(pow)
}

// AbsMax returns the index of the largest absolute value of the 4 given values.
// The returned index is always from 0-3.
func AbsMax(a0, a1, a2, a3 float64) int {
	maxIndex := -1
	maxVal := -LARGE
	if math.Abs(a0) > maxVal {
		maxIndex = 0
		maxVal = math.Abs(a0)
	}
	if math.Abs(a1) > maxVal {
		maxIndex = 1
		maxVal = math.Abs(a1)
	}
	if math.Abs(a2) > maxVal {
		maxIndex = 2
		maxVal = math.Abs(a2)
	}
	if math.Abs(a3) > maxVal {
		maxIndex = 3
	}
	return maxIndex
}

// Copyright © 2013 Galvanized Logic Inc.
// Use is governed by a FreeBSD license found in the LICENSE file.

package move

import (
	"fmt"
	"testing"
	"vu/math/lin"
)

// Check that broadphase doesn't duplicate comparisons.
// Each compare should cause an overlap.
func TestBroadphaseUniqueCompare(t *testing.T) {
	mov, sp := newMover(), NewSphere(1)
	bodies := []Body{newBody(sp), newBody(sp), newBody(sp), newBody(sp), newBody(sp)}
	for _, bod := range bodies {
		bod.SetMaterial(1, 0)
	}
	mov.broadphase(bodies, mov.overlapped)
	if len(mov.overlapped) != 10 {
		t.Errorf("Should be 10 unique comparisons for a list of 5. Got %d", len(mov.overlapped))
	}
}

// Basic test to check that a sphere will end up above a slab.
// The test uses no restitution (bounciness).
func TestSphereAt(t *testing.T) {
	mov := newMover()
	slab := newBody(NewBox(100, 25, 100)).SetMaterial(0, 0)
	slab.World().Loc.SetS(0, -25, 0)                // slab below ball at world y==0.
	ball := newBody(NewSphere(1)).SetMaterial(1, 0) //
	ball.World().Loc.SetS(-5, 15, -3)               // ball above slab.
	bodies := []Body{slab, ball}
	for cnt := 0; cnt < 100; cnt++ {
		mov.Step(bodies, 0.02)
	}
	ballAt, want := dumpV3(ball.World().Loc), dumpV3(&lin.V3{-5, 1, -3})
	if ballAt != want {
		t.Errorf("Ball should be at %s, but its at %s", want, ballAt)
	}
}

// Testing
// ============================================================================
// Utility functions for all package testcases.

func dumpT(t *lin.T) string   { return dumpV3(t.Loc) + dumpQ(t.Rot) }
func dumpQ(q *lin.Q) string   { return fmt.Sprintf("%2.1f", *q) }
func dumpV3(v *lin.V3) string { return fmt.Sprintf("%2.1f", *v) }
func dumpM3(m *lin.M3) string {
	format := "[%+2.1f, %+2.1f, %+2.1f]\n"
	str := fmt.Sprintf(format, m.X0, m.Y0, m.Z0)
	str += fmt.Sprintf(format, m.X1, m.Y1, m.Z1)
	str += fmt.Sprintf(format, m.X2, m.Y2, m.Z2)
	return str
}

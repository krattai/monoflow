// Copyright © 2013 Galvanized Logic Inc.
// Use is governed by a FreeBSD license found in the LICENSE file.

package move

import (
	"math"
	"vu/math/lin"
)

// Shapes are physics collision primitives used to enclose a 3D model.
// A Shape is always in local space centered at the origin. Combine a
// shape with a transform to position the shape anywhere in world space.
// Shapes do not allocate memory so calculations, like filling
// in bounding boxes, are expected to be given the necessary structures.
type Shape interface {
	Type() int       // Type returns the shape type.
	Volume() float64 // Volume is useful for mass = density*volume.

	// Aabb updates ab to be the axis aligned bounding box for this shape.
	// The updated Abox ab will be in the space defined by the transform.
	//    ab     : Output structure. Providing a nil Abox will cause a panic.
	//    margin : Optional small positive value that increases the size
	//             of the surrounding box. Use 0 for no margin.
	// The updated Abox ab is returned.
	Aabb(transfrom *lin.T, ab *Abox, margin float64) *Abox

	// Inertia is needed by collision resolution.
	//    mass   : can be set directly or as density*Volume()
	// The input vector, inertia, is updated and returned.
	Inertia(mass float64, inertia *lin.V3) *lin.V3
}

// Enumerate the shapes handled by physics and returned
// by Shape.Type()
const (
	SphereShape = iota // Considered convex (curving outwards).
	BoxShape           // Polyhedral (flat faces, straight edges). Considered convex.
	NumShapes          // Keep as last constant.
)

// Currently the shapes are so simple they are all kept in this one file.
// Shapes get crazy complex. For example:
//    FUTURE Capsule
//    FUTURE Cylinder
//    FUTURE Cone
//    FUTURE Multi sphere
//    FUTURE Compound shape of multiple primitives.
// 	  FUTURE Convex hull shapes created from triangle meshes.
//    FUTURE and so on to soft bodies.

// Shape interface
// ============================================================================
// box shape

// box is a collision shape primitive. It is an axis aligned bounding box that
// is centered at the origin and defined by half-lengths along each axis.
// A box has 6 faces, 8 vertices, and 12 edges.
type box struct {
	Hx, Hy, Hz float64
}

// NewBox creates a Box shape. Negative input values are turned positive.
// Input values of zero are ignored, but not recommended.
func NewBox(hx, hy, hz float64) Shape { return &box{math.Abs(hx), math.Abs(hy), math.Abs(hz)} }

// Implements Shape.Type
func (b *box) Type() int { return BoxShape }

// Implements Shape.Aabb
// The axis aligned bounding box must be big enough to surround a box
// that has been transformed.
func (b *box) Aabb(t *lin.T, ab *Abox, margin float64) *Abox {

	// transform the basis vectors, keeping them positive for extents.
	xx, xy, xz := lin.MultSQ(1, 0, 0, t.Rot)
	yx, yy, yz := lin.MultSQ(0, 1, 0, t.Rot)
	zx, zy, zz := lin.MultSQ(0, 0, 1, t.Rot)
	xx, xy, xz = math.Abs(xx), math.Abs(xy), math.Abs(xz)
	yx, yy, yz = math.Abs(yx), math.Abs(yy), math.Abs(yz)
	zx, zy, zz = math.Abs(zx), math.Abs(zy), math.Abs(zz)

	// Dot the half-extents, plus margin, with the transformed basis vectors.
	// to get the furthest extent in each direction.
	hmx, hmy, hmz := b.Hx+margin, b.Hy+margin, b.Hz+margin
	ex := hmx*xx + hmy*xy + hmz*xz
	ey := hmx*yx + hmy*yy + hmz*yz
	ez := hmx*zx + hmy*zy + hmz*zz

	ab.Sx, ab.Sy, ab.Sz = t.Loc.X-ex, t.Loc.Y-ey, t.Loc.Z-ez
	ab.Lx, ab.Ly, ab.Lz = t.Loc.X+ex, t.Loc.Y+ey, t.Loc.Z+ez
	return ab
}

// Implements Shape.Volume
func (b *box) Volume() float64 { return b.Hx * 2 * b.Hy * 2 * b.Hz * 2 }

// Implements Shape.Inertia
func (b *box) Inertia(mass float64, inertia *lin.V3) *lin.V3 {
	lx2, ly2, lz2 := 4.0*b.Hx*b.Hx, 4.0*b.Hy*b.Hy, 4.0*b.Hz*b.Hz
	inertia.SetS(mass/12.0*(ly2+lz2), mass/12.0*(lx2+lz2), mass/12.0*(lx2+ly2))
	return inertia
}

// box
// ============================================================================
// sphere shape

// sphere is a collision shape primitive that is defined by a radius around
// the origin.
type sphere struct {
	R float64
}

// NewSphere creates a Sphere shape. Negative radius values are turned positive.
// Input values of zero are ignored, but not recommended.
func NewSphere(radius float64) Shape { return &sphere{math.Abs(radius)} }

// Implements Shape.Type
func (s *sphere) Type() int { return SphereShape }

// Implements Shape.Aabb
func (s *sphere) Aabb(t *lin.T, ab *Abox, margin float64) *Abox {
	sides := s.R + margin
	ab.Sx, ab.Sy, ab.Sz = t.Loc.X-sides, t.Loc.Y-sides, t.Loc.Z-sides
	ab.Lx, ab.Ly, ab.Lz = t.Loc.X+sides, t.Loc.Y+sides, t.Loc.Z+sides
	return ab
}

// Implements Shape.Volume
func (s *sphere) Volume() float64 { return 4 / 3 * s.R * s.R * s.R * math.Pi }

// Implements Shape.Inertia
func (s *sphere) Inertia(mass float64, inertia *lin.V3) *lin.V3 {
	elem := 0.4 * mass * s.R * s.R
	inertia.SetS(elem, elem, elem)
	return inertia
}

// sphere
// ============================================================================
// Abox

// Abox is an axis aligned bounding box used with the Shape interface.
// Its primary purpose is to surround arbitrary shapes during broad phase
// collision detection. Abox is not a primitive shape for collision - use Box
// instead.  The vertices for the full axis aligned box are:
//    Sx, Sy, Sz -- smallest vertex (left, bottom, back = minimum point)
//    Sx, Sy, Lz |
//    Sx, Ly, Sz |
//    Sx, Ly, Lz |- generate if necessary.
//    Lx, Sy, Sz |
//    Lx, Sy, Lz |
//    Lx, Ly, Sz |
//    Lx, Ly, Lz -- largest vertex (right, top, front = maximum point)
type Abox struct {
	Sx, Sy, Sz float64 // Smallest point.
	Lx, Ly, Lz float64 // Largest point.
}

// Overlaps returns true if Abox a and b are intersecting. Returns false
// if Abox a and b are not intersecting or are just touching along one or
// more points, edges, or faces.
func (a *Abox) Overlaps(b *Abox) bool {
	return a.Lx > b.Sx && a.Sx < b.Lx && a.Ly > b.Sy && a.Sy < b.Ly && a.Lz > b.Sz && a.Sz < b.Lz
}

#include "Vertex.h"



Vertex::Vertex(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
	u = 0;
	v = 0;
	r = g = b = 0;
	nx = ny = nz = 0;
}

Vertex::Vertex()
{
	x = y = z = u = v = r = g = b = nx = ny = nz = 0;
}

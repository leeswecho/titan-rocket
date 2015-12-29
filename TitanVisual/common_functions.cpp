#include "common_classes.h"
//***********************************************//
//***********************************************//
vertex compute_normal(vertex p1, vertex p2, vertex p3)
{
	vertex pn;

	vertex rel1;

	rel1.x = p2.x - p3.x;
	rel1.y = p2.y - p3.y;
	rel1.z = p2.z - p3.z;

	vertex rel2;

	rel2.x = p1.x - p2.x;
	rel2.y = p1.y - p2.y;
	rel2.z = p1.z - p2.z;

	//compute cross product
	pn.x = (rel1.y * rel2.z) - (rel2.y * rel1.z);
	pn.y = (-1 * rel1.x * rel2.z) + (rel1.z * rel2.x);
	pn.z = (rel1.x * rel2.y) - (rel2.x * rel1.y);

	//normalize vector
	double mag = sqrt( pn.x * pn.x + pn.y * pn.y + pn.z * pn.z);
	pn.x /= mag;
	pn.y /= mag;
	pn.z /= mag;

	pn.x *= -1;
	pn.y *= -1;
	pn.z *= -1;

	pn.nx = pn.x;
	pn.ny = pn.y;
	pn.nz = pn.z;


	return pn;

}
//***********************************************//
//***********************************************//
int copy_normal(vertex* p1, vertex* p2)
{
	p1->nx = p2->nx;
	p1->ny = p2->ny;
	p1->nz = p2->nz;

	return 0;
}
//***********************************************//
//***********************************************//
MATRIX create_identity_matrix()
{
	MATRIX ret;

	ret._11 = 1.0;
	ret._22 = 1.0;
	ret._33 = 1.0;
	ret._44 = 1.0;

	ret._12 = 0.0;
	ret._13 = 0.0;
	ret._14 = 0.0;

	ret._21 = 0.0;
	ret._23 = 0.0;
	ret._24 = 0.0;

	ret._31 = 0.0;
	ret._32 = 0.0;
	ret._34 = 0.0;

	ret._41 = 0.0;
	ret._42 = 0.0;
	ret._43 = 0.0;

	return ret;
}
//***********************************************//
//***********************************************//
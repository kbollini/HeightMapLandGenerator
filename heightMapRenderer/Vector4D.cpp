#include "stdafx.h"

void initVector4D(float x, float y, float z, float d, Vector4D *v)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->d = d;
}


void add(Vector4D *v1, Vector4D *v2, Vector4D *v)
{
	v->x = v1->x + v2->x;
	v->y = v1->y + v2->y;
	v->z = v1->z + v2->z;
	v->d = 0.0f;
}

void addf(Vector4D *v1, float c, Vector4D *v)
{
	v->x = v1->x + c;
	v->y = v1->y + c;
	v->z = v1->z + c;
	v->d = 0.0f;
}

void minus(Vector4D *v1, Vector4D *v2, Vector4D *v)
{
	v->x = v1->x - v2->x;
	v->y = v1->y - v2->y;
	v->z = v1->z - v2->z;
	v->d = 0.0f;
}

void minusf(Vector4D *v1, float c,Vector4D *res)
{
	res->x = v1->x - c;
	res->y = v1->y - c;
	res->z = v1->z - c;
	res->d = 0.0f;
}


void mult(Vector4D *v1, Vector4D *v2, Vector4D *v)
{
	v->x = v1->x * v2->x;
	v->y = v1->y * v2->y;
	v->z = v1->z * v2->z;
	v->d = v1->d * v2->d;
}
void multf(Vector4D *v1, float c, Vector4D *v)
{
	v->x = v1->x * c;
	v->y = v1->y * c;
	v->z = v1->z * c;
	v->d = 0.0f;
}


bool inf(Vector4D *v1, Vector4D *v2)
{
	if (v1->x < v2->x && v1->y < v2->y && v1->z < v2->z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool supp(Vector4D *v1, Vector4D *v2)
{
	if (v1->x > v2->x && v1->y > v2->y && v1->z > v2->z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void interpolate(Vector4D *v)
{
	;
}
float getDotProduct(Vector4D *v1, Vector4D *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z)/(getNorme(v1)*getNorme(v2));
}
void CrossProduct(Vector4D *v1, Vector4D *v2, Vector4D *res)
{
	res->x = v1->y*v2->z - v2->y*v1->z;
	res->y = v1->z*v2->x - v2->z*v1->x;
	res->z = v1->x*v2->y - v2->x*v1->y;
	res->d = v1->d;
}

void normalize(Vector4D *v)
{
	v->x /= getNorme(v);
	v->y /= getNorme(v);
	v->z /= getNorme(v);
}

float* getArray(Vector4D *v){
	float *vect = new float[4];
	vect[0] = v->x;
	vect[1] = v->y;
	vect[2] = v->z;
	vect[3] = v->d;
	return vect;
}
/*
Vector4D operator+(Vector4D &a, Vector4D &b)
{
	Vector4D *v;
	v = add(&a,&b);
	return *v;
}
Vector4D operator-(Vector4D &a, Vector4D &b)
{
	Vector4D *v;
	v = minus(&a,&b);
	return *v;
}

Vector4D operator*(Vector4D &a, Vector4D &b)
{
	Vector4D *v;
	v = mult(&a,&b);
	return *v;
}

Vector4D operator*(Vector4D &a, float c)
{
	Vector4D *v;
	v = multf(&a,c);
	return *v;
}
Vector4D operator*(float c, Vector4D &a)
{
	Vector4D *v;
	v = multf(&a,c);
	return *v;
}*/

float getNorme(Vector4D *v)
{
	return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}
#pragma once

typedef struct Vector4Dstr
{
	float x;
	float y;
	float z;
	float d;		
}Vector4D;

void initVector4D(float x, float y, float z, float d, Vector4D *v);

void add(Vector4D *v1, Vector4D *v2, Vector4D *res);
void addf(Vector4D *v1, float c, Vector4D *res);
void minus(Vector4D *v1, Vector4D *v2,Vector4D *res);
void minusf(Vector4D *v1, float c,Vector4D *res);
void mult(Vector4D *v1, Vector4D *v2, Vector4D *res);
void multf(Vector4D *v1, float c,Vector4D *res);

bool inf(Vector4D *v1, Vector4D *v2);
bool supp(Vector4D *v1, Vector4D *v2);

void interpolate(Vector4D *v);
float getNorme(Vector4D *v);
float getDotProduct(Vector4D *v1, Vector4D *v2);
void CrossProduct(Vector4D *v1, Vector4D *v2, Vector4D *res);
void normalize(Vector4D *v);
float* getArray(Vector4D *v);

/*Vector4D operator+(Vector4D &a, Vector4D &b);
Vector4D operator-(Vector4D &a, Vector4D &b);
Vector4D operator*(Vector4D &a, Vector4D &b);
Vector4D operator*(Vector4D &a, float c);
Vector4D operator*(float c, Vector4D &a);*/
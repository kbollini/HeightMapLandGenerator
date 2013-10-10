#include "stdafx.h"

int getLandFromHM(const char *filename, int height, int width, float scale, LAND *pLand)
{
	if(!filename) {
		perror("Uncorrect filename");
		return -1;
	}
	
	pLand->u32LandHeight = height;
	pLand->u32LandWidth = width;
	pLand->u32LandAlt = scale*256;
	unsigned char *imageMatrice = new unsigned char[height*width];

	FILE *f = fopen(filename, "rb");
	if(!f){
		perror("Unable to open file");
		return -1 ;
	}
	int countVertex = 0;
	while (countVertex < width*height) {
		unsigned char c = fgetc(f);
		imageMatrice[countVertex] = c;
		countVertex++;
	}

	//Init number of vertex and faces
	pLand->u32VertexCount = width*height;
	pLand->pVertex = new VERTEX[pLand->u32VertexCount];
	pLand->pNormals = new VERTEX[pLand->u32VertexCount];
	pLand->u32FaceCount = ((width-1) * (height-1)) * 2;

	int currIndex;
	//MAKE VERTICES
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++) {
			currIndex =GetIndex(i,j,width);
			VERTEX Vertice;
			Vertice.fX = (float)i;
			Vertice.fY = imageMatrice[currIndex]*scale;
			Vertice.fZ = (float)j;
			pLand->pVertex[currIndex] = Vertice;
		}

	//MAKE VERTICES NORMALS
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++) {
			currIndex =GetIndex(i,j,width);
			VERTEX normal;
			computeNormal(i,j, &normal, pLand);
			pLand->pNormals[currIndex] = normal;
		}

	return 0;
}


unsigned int GetIndex(int x, int y, int offset)
{
	return x + (y*offset);
}

int GetFace(unsigned int index, FACE *pFace, LAND *pLand)
{
	if(index < 0 || index > pLand->u32FaceCount)
	{
		return -1;
	}
	int width = pLand->u32LandWidth;
	int cubeIndex = (index/2);
	int y = cubeIndex/(width-1);
	int x = cubeIndex - (y*(width-1));
	//Init Number of faces
	if (index%2){
		pFace->pu32Vertices[0] = GetIndex(x+1,y,width);
		pFace->pu32Vertices[1] = GetIndex(x+1,y+1,width);
		pFace->pu32Vertices[2] = GetIndex(x,y+1,width);
	} else {
		pFace->pu32Vertices[0] = GetIndex(x,y,width);
		pFace->pu32Vertices[1] = GetIndex(x+1,y,width);
		pFace->pu32Vertices[2] = GetIndex(x,y+1,width);
	}

	return 0;		
}

void computeNormal(int indexX, int indexY, VERTEX *pNormal, LAND *pLand)
{
	//Sum
	Vector4D sum;
	initVector4D(0.0f,0.0f,0.0f,0.0f, &sum);
	int width = pLand->u32LandWidth;

	//compute triangles contributions
	Vector4D v, v0, v1, side1, side2, contribution;
	VertexToVector4D(&pLand->pVertex[GetIndex(indexX,indexY,width)], &v);
	//t1
	if(indexX-1 > 0 && indexY+1 < pLand->u32LandWidth)
	{
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX-1,indexY,width)], &v0);
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX-1,indexY+1,width)], &v1);
		minus(&v0, &v, &side1);
		minus(&v1, &v0, &side2);
		CrossProduct(&side1, &side2, &contribution);
		add(&sum,&contribution, &sum);
	}
	//t2
	if(indexX-1 > 0 && indexY+1 < pLand->u32LandWidth)
	{
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX-1,indexY+1,width)], &v0);
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX,indexY+1,width)], &v1);
		minus(&v0, &v, &side1);
		minus(&v1, &v0, &side2);
		CrossProduct(&side1, &side2, &contribution);
		add(&sum,&contribution, &sum);
	}

	//t3
	if(indexX+1 > pLand->u32LandWidth && indexY+1 < pLand->u32LandWidth)
	{
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX,indexY+1,width)], &v0);
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX+1,indexY+1,width)], &v1);
		minus(&v0, &v, &side1);
		minus(&v1, &v0, &side2);
		CrossProduct(&side1, &side2, &contribution);
		add(&sum,&contribution, &sum);
	}

	//t4
	if(indexX+1 > pLand->u32LandWidth && indexY+1 < pLand->u32LandWidth && indexY-1 > 0)
	{
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX+1,indexY,width)], &v0);
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX+1,indexY-1,width)], &v1);
		minus(&v0, &v, &side1);
		minus(&v1, &v0, &side2);
		CrossProduct(&side1, &side2, &contribution);
		add(&sum,&contribution, &sum);
	}

	//t5
	if(indexX+1 > pLand->u32LandWidth && indexY-1 > 0)
	{
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX+1,indexY-1,width)], &v0);
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX,indexY-1,width)], &v1);
		minus(&v0, &v, &side1);
		minus(&v1, &v0, &side2);
		CrossProduct(&side1, &side2, &contribution);
		add(&sum,&contribution, &sum);
	}

	//t6
	if(indexX-1 > 0 && indexY-1 > 0)
	{
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX,indexY-1,width)], &v0);
		VertexToVector4D(&pLand->pVertex[GetIndex(indexX-1,indexY,width)], &v1);
		minus(&v0, &v, &side1);
		minus(&v1, &v0, &side2);
		CrossProduct(&side1, &side2, &contribution);
		add(&sum,&contribution, &sum);
	}

	normalize(&sum);
	Vector4DToVertex(&sum, pNormal);
  }

void VertexToVector4D(VERTEX *v0, Vector4D *v1)
{
	initVector4D(v0->fX,v0->fY,v0->fZ,0.0f, v1);
}

void Vector4DToVertex(Vector4D *v0, VERTEX *v1)
{
	v1->fX = v0->x;
	v1->fY = v0->y;
	v1->fZ = v0->z;
}
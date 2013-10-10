#pragma once
#define SIZE 8

typedef struct BOUNDSstr
{
	Vector4D v4DCenter;
	float fRadius;
} BOUNDS;

typedef struct OCTREEstr
{
	struct OCTREEstr *pChild;
	int *pFace;
	unsigned int u32FaceCount;
	BOUNDS bounds;
} OCTREE;

OCTREE *createOctree(LAND *pLand, unsigned int u32Threshold, const unsigned int u32MaximumDepth);

void buildChilds(const unsigned int u32Threshold, const unsigned int u32MaximumDepth, 
	const unsigned int u32CurrentDepth, OCTREE *pFather, LAND *pLand);

void calcChildFaces(OCTREE *pFather, OCTREE *pChild, LAND *pLand);


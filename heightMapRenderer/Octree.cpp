#include "stdafx.h"

OCTREE *createOctree(LAND *pLand, unsigned int u32Threshold, const unsigned int u32MaximumDepth)
{
	BOUNDS bounds;
	bounds.fRadius = max(pLand->u32LandHeight * 0.5, pLand->u32LandAlt * 0.5);
	Vector4D pCenter;
	initVector4D(bounds.fRadius, bounds.fRadius, bounds.fRadius, 0.0,
		&pCenter);
	bounds.v4DCenter = pCenter;
	
	int *pFace = new int[pLand->u32FaceCount];
	for (unsigned int i = 0; i<pLand->u32FaceCount; i++)
		pFace[i] = i;

	OCTREE father;
	father.bounds = bounds;
	father.pFace = pFace;
	father.u32FaceCount = pLand->u32FaceCount;

	buildChilds(u32Threshold, u32MaximumDepth, 0, &father, pLand);
	return &father;
}

void buildChilds(const unsigned int u32Threshold, const unsigned int u32MaximumDepth, 
	const unsigned int u32CurrentDepth, OCTREE *pFather, LAND *pLand)
{
	if (pFather->u32FaceCount <= u32Threshold || u32CurrentDepth >= u32MaximumDepth)
	{
		pFather->pChild = NULL;
	}
	else
	{
		pFather->pChild = new OCTREE[SIZE];
		for (int i = 0; i < SIZE; i++)
		{	
			Vector4D boundsOffsetTable[8] = 
			{
                    {-0.5, -0.5, -0.5},
                    {+0.5, -0.5, -0.5},
                    {-0.5, +0.5, -0.5},
                    {+0.5, +0.5, -0.5},
                    {-0.5, -0.5, +0.5},
                    {+0.5, -0.5, +0.5},
                    {-0.5, +0.5, +0.5},
                    {+0.5, +0.5, +0.5}
            };
			
            Vector4D offset;
			float fRadius = pFather->bounds.fRadius;
			multf(&boundsOffsetTable[i], fRadius, &offset);
			
            BOUNDS newBounds;
			newBounds.fRadius = fRadius * 0.5f;
			add(&pFather->bounds.v4DCenter, &offset, &newBounds.v4DCenter);
			pFather->pChild[i].bounds = newBounds;
			calcChildFaces(pFather, &pFather->pChild[i], pLand);

			buildChilds(u32Threshold, u32MaximumDepth, u32CurrentDepth + 1,
				&pFather->pChild[i], pLand);
		}
	}
}


void calcChildFaces(OCTREE *pFather, OCTREE *pChild, LAND *pLand)
{
	Vector4D pCenter = pChild->bounds.v4DCenter;
	float fRadius = pChild->bounds.fRadius;
	Vector4D v4DMin;
	minusf(&pCenter, fRadius, &v4DMin);
	Vector4D v4DMax; 
	addf(&pCenter, fRadius, &v4DMax);
	unsigned int u32NewCmpt = 0;
	int *pNewFace = new int[pFather->u32FaceCount];
	for (unsigned int i = 0; i < pFather->u32FaceCount; i++)
	{
		FACE face;
		GetFace(i, &face, pLand);
		for (unsigned int j = 0; j < 3; j++)
		{
			Vector4D pCurrPoint;
			VertexToVector4D(&pLand->pVertex[face.pu32Vertices[j]], &pCurrPoint);
			if (inf(&pCurrPoint, &v4DMax) && supp(&pCurrPoint, &v4DMin))
			{
				pNewFace[u32NewCmpt] = i;
				u32NewCmpt++;
				break;
			}
		}
	}
	pChild->pFace = pNewFace;
	pChild->u32FaceCount = u32NewCmpt;
}
#pragma once

typedef struct LANDstr {

	unsigned int u32FaceCount;

	VERTEX *pVertex;
	VERTEX *pNormals;
	unsigned int u32VertexCount;

	unsigned int u32LandAlt;
	unsigned int u32LandWidth;
	unsigned int u32LandHeight;
}LAND;
		
int getLandFromHM(const char *filename, int height, int width, float scale, LAND *pTer);
unsigned int GetIndex(int x, int y, int offset);
int GetFace(unsigned int index, FACE *pFace, LAND *pLand);

void computeNormal(int indexX, int indexY, VERTEX *pNormal, LAND *pLand);
void VertexToVector4D(VERTEX *v0, Vector4D *v1);
void Vector4DToVertex(Vector4D *v0, VERTEX *v1);

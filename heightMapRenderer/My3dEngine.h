#pragma once
#include "3DEngine.h"
#include "OBJReader.h"
#include "GL/GL.h"
#include "GL/GLU.h"
#include "RAWLoader.h"

class My3dEngine : public C3DEngine
{
public:

	OCTREE pOctree;
	LAND *pLand;
	POINT oldPos;
	float zoom;
	float elevation;
	float tourner;
	bool move;
	bool rotate;
	float x, z;
	int currOctree;

	virtual void Setup(HWND hWnd);
	virtual void Render(unsigned int u32Width, unsigned int u32Height);

	//--- Events notifications


	void My3dEngine::DrawCubeOctree(BOUNDS bound);
	virtual void MouseWheel(float fIncrement);
	virtual void MouseMove(POINT Pos);
	virtual void LButtonDown(POINT Pos);
	virtual void LButtonUp(POINT Pos);
	virtual void RButtonDown(POINT Pos);
	virtual void RButtonUp(POINT Pos);
	virtual void KeyDown(int s32VirtualKey);
	virtual void updateCamera();
};


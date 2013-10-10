#include "stdafx.h"

My3dEngine E;

void My3dEngine::Setup(HWND h)
{

	int octaves = 3;
	int frequence = 3;
	float persistence = 0.5;
    int taille = 256;
    int lissage = 3;
	float scale = 0.4;

    // création de l'heightmap
    Calque calque (taille, 1);
	calque.generer_calque(frequence,octaves,persistence,lissage,"resultat.raw");

	//Parse de l'heightmap
	pLand = new LAND;
	if(getLandFromHM("resultat.raw",taille,taille,scale,pLand) < 0)
	{
		perror("cannot build LAND Struct");	
	}

	//Initialisations Octree
	int profondeur = 2;
	pOctree = *createOctree(pLand, 0, profondeur);

	//Gl Options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);


	//init camera
	zoom = -500;
	oldPos.x = 0;
	oldPos.y = 0;
	x = -110;
	z = 0;
	move= false;
	rotate = false;

	elevation = 35;
	currOctree = 9;
}

void My3dEngine::Render(unsigned int u32Width, unsigned int u32Height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear frame/depth buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	updateCamera();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (double) 640/480, 0.5, 1000);
	
	GLfloat ambient[] = {0.01,0.01,0.01,1};
	GLfloat specular[] = {1,1,1,1};
	GLfloat diffuse[] = {1,1,1,1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	//TODO founds octrees
	if (currOctree <8) {
		DrawCubeOctree(pOctree.pChild[currOctree].bounds);
		
		
		for (unsigned int i = 0; i < 8; i++)
		{
			if (pOctree.pChild[currOctree].pChild != NULL)
			{
				DrawCubeOctree(pOctree.pChild[currOctree].pChild[i].bounds);
			}
		}
		for (unsigned int j=0; j < pOctree.pChild[currOctree].u32FaceCount; j++) {
			FACE pFace;
			GetFace(pOctree.pChild[currOctree].pFace[j], &pFace, pLand);
			/* init normals */
			glBegin(GL_TRIANGLES);
			for (int k =0; k<3; k++)
			{
				//glTexCoord2fv(&scn->pUV[face->pu32UV[k]].fU);
				glNormal3fv(&pLand->pNormals[pFace.pu32Vertices[k]].fX);
				glVertex3fv(&pLand->pVertex[pFace.pu32Vertices[k]].fX);
			}
			glEnd();
		}
	}
	else
	{
		for (unsigned int j=0; j < pOctree.u32FaceCount; j++) {
			FACE pFace;
			GetFace(pOctree.pFace[j], &pFace, pLand);
			/* init normals */
			glBegin(GL_TRIANGLES);
			for (int k =0; k<3; k++)
			{
				//glTexCoord2fv(&scn->pUV[face->pu32UV[k]].fU);
				glNormal3fv(&pLand->pNormals[pFace.pu32Vertices[k]].fX);
				glVertex3fv(&pLand->pVertex[pFace.pu32Vertices[k]].fX);
			}
			glEnd();
		}
	} 

	for (int i = 0; i < 8; i++)
		DrawCubeOctree(pOctree.bounds);

}

void My3dEngine::DrawCubeOctree(BOUNDS bound)
{
	glBegin(GL_LINE_STRIP);
    glColor3f(1.0,0.0,0.0);
	glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
	glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
    glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
    glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
    glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
  glEnd();

   glBegin(GL_LINE_STRIP);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
	glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
	glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
	glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
	glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
	glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
  glEnd();

  glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
	glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
	glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
    glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
    glVertex3f(bound.v4DCenter.x - bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
    glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
    glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y - bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
    glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z + bound.fRadius);
    glVertex3f(bound.v4DCenter.x + bound.fRadius, bound.v4DCenter.y + bound.fRadius,
		bound.v4DCenter.z - bound.fRadius);
  glEnd();
}

void My3dEngine::MouseWheel(float fIncrement)
{
	zoom += fIncrement;
}
void My3dEngine::MouseMove(POINT Pos)
{
	if(rotate) {
	if(oldPos.x < Pos.x)
		tourner = tourner +0.6;
	else if(oldPos.x > Pos.x)
		tourner = tourner - 0.6;

	if(oldPos.y < Pos.y)
		elevation = elevation + 0.6;
	else if(oldPos.y > Pos.y)
		elevation = elevation- 0.6;
	}

	if(move) {
	if(oldPos.x < Pos.x)
		x = x+0.3;
	else if(oldPos.x > Pos.x)
		x = x-0.3;

	if(oldPos.y < Pos.y)
		z = z +0.3;
	else if(oldPos.y > Pos.y)
		z = z- 0.3;
	}

	oldPos = Pos;

}
void My3dEngine::LButtonDown(POINT Pos)
{
	move = true;
}
void My3dEngine::LButtonUp(POINT Pos)
{
	move = false;
}
void My3dEngine::RButtonDown(POINT Pos)
{
	rotate = true;
}
void My3dEngine::RButtonUp(POINT Pos)
{
	rotate = false;
}
void My3dEngine::KeyDown(int s32VirtualKey)
{
	if(s32VirtualKey > 48 && s32VirtualKey <58)
		currOctree = s32VirtualKey - 49;
}

void My3dEngine::updateCamera()
{
	/*dezoomer*/
	glTranslatef(0,0,zoom);
	/*Angle elevation*/
	glRotatef(elevation,1,0,0);
	/*tourner autour*/
	glRotatef(tourner,0,1,0);
	/*reculer sur x,z*/
	glTranslatef(x,0,z);
}
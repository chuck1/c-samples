
//////////////////////////////////////////////////////////////////////////////////////////
//	Scene.cpp
//	Draw the scene for shadow mapping
//	Downloaded from: www.paulsprojects.net
//	Created:	16th September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math/math.h>

#include "scene.h"

void DrawScene(float angle)
{
	//Display lists for objects
	static GLuint spheresList=0, torusList=0, baseList=0;

	//Create spheres list if necessary
	if(!spheresList)
	{
		spheresList=glGenLists(1);
		glNewList(spheresList, GL_COMPILE);
		{


			//glColor3f(0.0f, 1.0f, 0.0f);
			GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
			GLfloat diffuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
			GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat shininess[] = {128.0f};
			
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			//glMaterialfv(GL_FRONT, GL_EMISSION, color);
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);



			glPushMatrix();

			glTranslatef(0.45f, 1.0f, 0.45f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(-0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.0f, 0.0f,-0.9f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glPopMatrix();
		}
		glEndList();
	}

	//Create torus if necessary
	if(!torusList)
	{
		torusList=glGenLists(1);
		glNewList(torusList, GL_COMPILE);
		{

			GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
			GLfloat diffuse[] = {0.0f, 1.0f, 0.0f, 1.0f};
			GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat shininess[] = {128.0f};
			
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			//glMaterialfv(GL_FRONT, GL_EMISSION, color);
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

			//glColor3f(1.0f, 0.0f, 0.0f);


			glPushMatrix();

			glTranslatef(0.0f, 0.5f, 0.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glutSolidTorus(0.2, 0.5, 24, 48);

			glPopMatrix();
		}
		glEndList();
	}

	//Create base if necessary
	if(!baseList)
	{
		baseList=glGenLists(1);
		glNewList(baseList, GL_COMPILE);
		{

			//glColor3f(0.0f, 0.0f, 1.0f);

			GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
			GLfloat diffuse[] = {0.0f, 0.0f, 1.0f, 1.0f};
			GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat shininess[] = {128.0f};
			
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			//glMaterialfv(GL_FRONT, GL_EMISSION, color);
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);




			glPushMatrix();

			glScalef(1.0f, 0.05f, 1.0f);
			glutSolidCube(3.0f);

			glPopMatrix();
		}
		glEndList();
	}


	//Draw objects
	glCallList(baseList);
	glCallList(torusList);

	glPushMatrix();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glCallList(spheresList);
	glPopMatrix();
}




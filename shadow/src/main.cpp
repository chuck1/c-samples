//////////////////////////////////////////////////////////////////////////////////////////
//	Shadow Mapping Tutorial
//	Accompanies a tutorial found on my site
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
//#include <GLee.h>	//GL header file, including extensions
#include <GL/glut.h>

#include <math/vec3.h>
#include <math/vec4.h>
#include <math/mat44.h>
#include <math/color.h>

#include <glutpp/texture.h>

#include "TIMER.h"
#include "FPS_COUNTER.h"
#include "scene.h"
#include "main.h"
#include <string.h>

//Timer used for frame rate independent movement
TIMER timer;

//Frames per second counter
FPS_COUNTER fpsCounter;

//Camera & light positions
math::vec3 cameraPosition(-5.0f, 5.0f,-5.0f);
math::vec3 lightPosition(-5.0f, 5.0f,5.0f);

//Size of shadow map
const int shadowMapSize = 512;

//Textures
//GLuint shadowMapTexture;

//window size
int windowWidth, windowHeight;

//Matrices
math::mat44 lightProjectionMatrix, lightViewMatrix;
math::mat44 cameraProjectionMatrix, cameraViewMatrix;

glutpp::texture tex_shadow_map;

// Settings
bool ortho = true;
bool shadow = true;

enum cm
{
	COLOR_MATERIAL,
	MATERIAL
};

int color_mode = MATERIAL;

float angle = 0;

//Called for initiation
bool Init(void)
{
	//Check for necessary extensions
	if(!GL_ARB_depth_texture || !GL_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
		return false;
	}

	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);
	
	/*
	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_DEPTH_COMPONENT,
			shadowMapSize,
			shadowMapSize,
			0,
			GL_DEPTH_COMPONENT,
			GL_UNSIGNED_BYTE,
			NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	*/
	tex_shadow_map.init(shadowMapSize,shadowMapSize);
	
	if(color_mode==COLOR_MATERIAL)
	{
		//Use the color as the ambient and diffuse material
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}
	//White specular material color, shininess 16
	glMaterialfv(GL_FRONT, GL_SPECULAR, math::white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);


	//Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth/windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 1.0f, 50.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(	lightPosition.x, lightPosition.y, lightPosition.z,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();

	//Reset timer
	timer.Reset();

	return true;
}
void RenderLightPOV()
{
	//First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(lightViewMatrix);

	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0, 0, 0, 0);

	//Draw the scene
	DrawScene(angle);

	//Read the depth buffer into the shadow map texture
	//glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	tex_shadow_map.bind();
	
	glCopyTexSubImage2D(
		GL_TEXTURE_2D,
		0, 0, 0, 0, 0,
		shadowMapSize, shadowMapSize);
	
	//restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

}
void	RenderShadow()
{
	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	static math::mat44 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]

	math::mat44 textureMatrix=biasMatrix*lightProjectionMatrix*lightViewMatrix;

	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	//Bind & enable shadow map texture
	//glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	tex_shadow_map.bind();
	glEnable(GL_TEXTURE_2D);

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	//Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

}
//Called to draw scene
void SMT_Display()
{
	//angle of spheres in scene. Calculate from time
	//float angle=timer.GetTime()/10;	
	angle += 0.1;

	if(shadow) RenderLightPOV();

	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);//me



	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraViewMatrix);

	glViewport(0, 0, windowWidth, windowHeight);

	//Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT1, GL_POSITION,	math::vec4(lightPosition));
	glLightfv(GL_LIGHT1, GL_AMBIENT,	math::white*0.2f);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,	math::white*0.2f);
	glLightfv(GL_LIGHT1, GL_SPECULAR,	math::black);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	DrawScene(angle);

	//3rd pass
	//Draw with bright light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, math::white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, math::white);

	if(shadow) RenderShadow();

	DrawScene(angle);

	if(shadow) RenderShadowPost();
	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	
	glDisable(GL_ALPHA_TEST);
	
	
	//Restore other states
	glDisable(GL_LIGHTING);

	//Update frames per second counter
	fpsCounter.Update();

	//Print fps
	static char fpsString[32];
	sprintf(fpsString, "%.2f", fpsCounter.GetFps());


	if(ortho)
	{
		//Set matrices for ortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//Print text
		glRasterPos2f(-1.0f, 0.9f);
		for(unsigned int i=0; i<strlen(fpsString); ++i)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsString[i]);

		//reset matrices
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}


	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
}

//Called on window resize
void Reshape(int w, int h)
{
	//Save new window size
	windowWidth=w, windowHeight=h;

	//Update the camera's projection matrix
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth/windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	glPopMatrix();
}

//Called when a key is pressed
void Keyboard(unsigned char key, int x, int y)
{
	//If escape is pressed, exit
	if(key==27)
		exit(0);

	//Use P to pause the animation and U to unpause
	if(key=='P' || key=='p')
		timer.Pause();

	if(key=='U' || key=='u')
		timer.Unpause();

	if(key=='O' || key=='o')
		ortho = !ortho;

	if(key=='S' || key=='s')
		shadow = !shadow;

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 512);
	glutCreateWindow("Shadow Mapping");

	if(!Init())
		return 0;

	glutDisplayFunc(SMT_Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
	return 0;
}

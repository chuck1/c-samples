#ifndef __LIZ_H__
#define __LIZ_H__


/* Copyright (c) Mark J. Kilgard, 1994, 1997.  */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* Example for PC game developers to show how to *combine* texturing,
   reflections, and projected shadows all in real-time with OpenGL.
   Robust reflections use stenciling.  Robust projected shadows
   use both stenciling and polygon offset.  PC game programmers
   should realize that neither stenciling nor polygon offset are 
   supported by Direct3D, so these real-time rendering algorithms
   are only really viable with OpenGL. 

   The program has modes for disabling the stenciling and polygon
   offset uses.  It is worth running this example with these features
   toggled off so you can see the sort of artifacts that result.

   Notice that the floor texturing, reflections, and shadowing
   all co-exist properly. */

/* When you run this program:  Left mouse button controls the
   view.  Middle mouse button controls light position (left &
   right rotates light around dino; up & down moves light
   position up and down).  Right mouse button pops up menu. */

/* Check out the comments in the "redraw" routine to see how the
   reflection blending and surface stenciling is done.  You can
   also see in "redraw" how the projected shadows are rendered,
   including the use of stenciling and polygon offset. */

/* This program is derived from glutdino.c */

/* Compile: cc -o dinoshade dinoshade.c -lglut -lGLU -lGL -lXmu -lXext -lX11 -lm */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       /* for cos(), sin(), and sqrt() */
#include <GL/glut.h>    /* OpenGL Utility Toolkit header */
#include <GL/glext.h>

#include <Maths/Maths.h>

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif

/* Variable controlling various rendering modes. */

class liz
{
	public:
		liz():
			stencilReflection(1),
			stencilShadow(1),
			offsetShadow(1),
			renderShadow(1),
			renderDinosaur(1),
			renderReflection(1),
			linearFiltering(0),
			useMipmaps(0),
			useTexture(1),
			reportSpeed(0),
			animation(1),
			lightSwitch(GL_TRUE),
			directionalLight(1),
			forceExtension(0),
			jump(0.0),
			lightAngle(0.0),
			lightHeight(20),
			angle(-150),   /* in degrees */
			angle2(30),   /* in degrees */
			lightMoving(0),
			bodyWidth(3.0),
			lightColor({0.8, 1.0, 0.8, 1.0}), /* green-tinted */
			skinColor({0.1, 1.0, 0.1, 1.0}),
			eyeColor({1.0, 0.2, 0.2, 1.0}),
			{
				
				floorVertices({
						{ -20.0, 0.0, 20.0 },
						{ 20.0, 0.0, 20.0 },
						{ 20.0, 0.0, -20.0 },
						{ -20.0, 0.0, -20.0 }});
				




			}

		int stencilReflection;
		int stencilShadow;
		int offsetShadow;
		int renderShadow;
		int renderDinosaur;
		int renderReflection;
		int linearFiltering;
		int useMipmaps;
		int useTexture;
		int reportSpeed;
		int animation;
		GLboolean lightSwitch;
		int directionalLight;
		int forceExtension;

		/* Time varying or user-controled variables. */
		float jump;
		float lightAngle;
		float lightHeight;
		GLfloat angle;   /* in degrees */
		GLfloat angle2;   /* in degrees */

		int moving, startx, starty;
		int lightMoving, lightStartX, lightStartY;

		int polygonOffsetVersion;

		GLdouble bodyWidth;

		/* *INDENT-OFF* */

		GLfloat lightPosition[4];
		GLfloat lightColor[]; /* green-tinted */
		GLfloat skinColor[], eyeColor[];
		/* *INDENT-ON* */

		/* Nice floor texture tiling pattern. */

		GLfloat floorVertices[4][3];



		VECTOR4D floorPlane;
		MATRIX4X4 floorShadow;

};


enum
{
	MISSING, EXTENSION, ONE_DOT_ONE
};

/* Enumerants for refering to display lists. */
typedef enum
{
	RESERVED,
	BODY_SIDE,
	BODY_EDGE,
	BODY_WHOLE,
	ARM_SIDE,
	ARM_EDGE,
	ARM_WHOLE,
	LEG_SIDE,
	LEG_EDGE,
	LEG_WHOLE,
	EYE_SIDE,
	EYE_EDGE,
	EYE_WHOLE
} displayLists;




enum
{
	M_NONE, M_MOTION, M_LIGHT, M_TEXTURE, M_SHADOWS, M_REFLECTION, M_DINOSAUR,
	M_STENCIL_REFLECTION, M_STENCIL_SHADOW, M_OFFSET_SHADOW,
	M_POSITIONAL, M_DIRECTIONAL, M_PERFORMANCE
};

void makeFloorTexture();
void makeDinosaur();
void drawDinosaur();
void extrudeSolidFromPolygon(GLfloat data[][2], unsigned int dataSize, GLdouble thickness, GLuint side, GLuint edge, GLuint whole);
void drawFloor();


#endif

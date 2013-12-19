
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

#include <math/math.h>

#include <liz.h>

GLfloat body[][2] =
{
	{0, 3},
	{1, 1},
	{5, 1},
	{8, 4},
	{10, 4},
	{11, 5},
	{11, 11.5},
	{13, 12},
	{13, 13},
	{10, 13.5},
	{13, 14},
	{13, 15},
	{11, 16},
	{8, 16},
	{7, 15},
	{7, 13},
	{8, 12},
	{7, 11},
	{6, 6},
	{4, 3},
	{3, 2},
	{1, 2}
};

GLfloat arm[][2] = { {8, 10}, {9, 9}, {10, 9}, {13, 8}, {14, 9}, {16, 9},
	{15, 9.5}, {16, 10}, {15, 10}, {15.5, 11}, {14.5, 10}, {14, 11}, {14, 10},
	{13, 9}, {11, 11}, {9, 11} };

GLfloat leg[][2] = { {8, 6}, {8, 4}, {9, 3}, {9, 2}, {8, 1}, {8, 0.5}, {9, 0},
	{12, 0}, {10, 1}, {10, 2}, {12, 4}, {11, 6}, {10, 7}, {9, 7} };

GLfloat eye[][2] = { {8.75, 15}, {9, 14.7}, {9.6, 14.7}, {10.1, 15},
	{9.6, 15.25}, {9, 15.25} };

const char *circles[] =
{
	"....xxxx........",
	"..xxxxxxxx......",
	".xxxxxxxxxx.....",
	".xxx....xxx.....",
	"xxx......xxx....",
	"xxx......xxx....",
	"xxx......xxx....",
	"xxx......xxx....",
	".xxx....xxx.....",
	".xxxxxxxxxx.....",
	"..xxxxxxxx......",
	"....xxxx........",
	"................",
	"................",
	"................",
	"................",
};

void makeFloorTexture()
{
	GLubyte floorTexture[16][16][3];
	GLubyte *loc;
	int s, t;

	/* Setup RGB image for the texture. */
	loc = (GLubyte*) floorTexture;
	for (t = 0; t < 16; t++)
	{
		for (s = 0; s < 16; s++)
		{
			if (circles[t][s] == 'x')
			{
				/* Nice green. */
				loc[0] = 0x1f;
				loc[1] = 0x8f;
				loc[2] = 0x1f;
			}
			else
			{
				/* Light gray. */
				loc[0] = 0xaa;
				loc[1] = 0xaa;
				loc[2] = 0xaa;
			}
			loc += 3;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if(gliz.useMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 16, 16,
				GL_RGB, GL_UNSIGNED_BYTE, floorTexture);
	}
	else
	{
		if(gliz.linearFiltering)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0,
				GL_RGB, GL_UNSIGNED_BYTE, floorTexture);
	}
}
void extrudeSolidFromPolygon(GLfloat data[][2],
		unsigned int dataSize,
		GLdouble thickness,
		GLuint side,
		GLuint edge,
		GLuint whole)
{
	static GLUtriangulatorObj *tobj = NULL;
	GLdouble vertex[3], dx, dy, len;
	int i;
	int count = dataSize / (2 * sizeof(GLfloat));

	if (tobj == NULL)
	{
		tobj = gluNewTess();  /* create and initialize a GLU polygon tesselation object */

		//gluTessCallback(tobj, GLU_BEGIN, glBegin);
		//gluTessCallback(tobj, GLU_VERTEX, glVertex2fv);  /* semi-tricky */
		//gluTessCallback(tobj, GLU_END, glEnd);
	}
	glNewList(side, GL_COMPILE);
	glShadeModel(GL_SMOOTH);  /* smooth minimizes seeing
				     tessellation */
	gluBeginPolygon(tobj);
	for (i = 0; i < count; i++) {
		vertex[0] = data[i][0];
		vertex[1] = data[i][1];
		vertex[2] = 0;
		gluTessVertex(tobj, vertex, data[i]);
	}
	gluEndPolygon(tobj);
	glEndList();
	glNewList(edge, GL_COMPILE);
	glShadeModel(GL_FLAT);  /* flat shade keeps angular hands
				   from being "smoothed" */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= count; i++)
	{
		/* mod function handles closing the edge */
		glVertex3f(data[i % count][0], data[i % count][1], 0.0);
		glVertex3f(data[i % count][0], data[i % count][1], thickness);
		/* Calculate a unit normal by dividing by Euclidean
		   distance. We could be lazy and use
		   glEnable(GL_NORMALIZE) so we could pass in arbitrary
		   normals for a very slight performance hit. */
		dx = data[(i + 1) % count][1] - data[i % count][1];
		dy = data[i % count][0] - data[(i + 1) % count][0];
		len = sqrt(dx * dx + dy * dy);
		glNormal3f(dx / len, dy / len, 0.0);
	}
	glEnd();
	glEndList();

	glNewList(whole, GL_COMPILE);
	glFrontFace(GL_CW);
	glCallList(edge);
	glNormal3f(0.0, 0.0, -1.0);  /* constant normal for side */
	glCallList(side);
	glPushMatrix();

	glTranslatef(0.0, 0.0, thickness);
	glFrontFace(GL_CCW);
	glNormal3f(0.0, 0.0, 1.0);  /* opposite normal for other side */
	glCallList(side);

	glPopMatrix();
	glEndList();
}
void liz::makeDinosaur()
{
	extrudeSolidFromPolygon(body, sizeof(body), bodyWidth,
			BODY_SIDE, BODY_EDGE, BODY_WHOLE);
	extrudeSolidFromPolygon(arm, sizeof(arm), bodyWidth / 4,
			ARM_SIDE, ARM_EDGE, ARM_WHOLE);
	extrudeSolidFromPolygon(leg, sizeof(leg), bodyWidth / 2,
			LEG_SIDE, LEG_EDGE, LEG_WHOLE);
	extrudeSolidFromPolygon(eye, sizeof(eye), bodyWidth + 0.2,
			EYE_SIDE, EYE_EDGE, EYE_WHOLE);
}
void liz::drawDinosaur()
{
	glPushMatrix();
	/* Translate the dinosaur to be at (0,8,0). */
	glTranslatef(-8, 0, -bodyWidth / 2);
	glTranslatef(0.0, jump, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, skinColor);
	glCallList(BODY_WHOLE);
	glTranslatef(0.0, 0.0, bodyWidth);
	glCallList(ARM_WHOLE);
	glCallList(LEG_WHOLE);
	glTranslatef(0.0, 0.0, -bodyWidth - bodyWidth / 4);
	glCallList(ARM_WHOLE);
	glTranslatef(0.0, 0.0, -bodyWidth / 4);
	glCallList(LEG_WHOLE);
	glTranslatef(0.0, 0.0, bodyWidth / 2 - 0.1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, eyeColor);
	glCallList(EYE_WHOLE);
	glPopMatrix();
}





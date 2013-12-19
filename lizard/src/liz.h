#ifndef __LIZ_H__
#define __LIZ_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       /* for cos(), sin(), and sqrt() */
#include <GL/glut.h>    /* OpenGL Utility Toolkit header */
#include <GL/glext.h>

#include <math/vec4.h>
#include <math/mat44.h>
#include <math/color.h>

#include <glutpp/light.h>
#include <glutpp/plane.h>

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
			skinColor({0.1, 1.0, 0.1, 1.0}),
			eyeColor({1.0, 0.2, 0.2, 1.0})
		{



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

		int moving;
		int startx;
		int starty;
		int lightMoving;
		int lightStartX;
		int lightStartY;

		int polygonOffsetVersion;

		GLdouble bodyWidth;

		/* *INDENT-OFF* */
		
		//math::vec4 lightPosition;
		//math::color lightColor; /* green-tinted */
		glutpp::light		light_;
		
		math::color		skinColor;
		math::color		eyeColor;
		
		/* Nice floor texture tiling pattern. */
		math::mat44		floorShadow;	
		glutpp::plane		floor_;

		void redraw();
		void mouse(int, int, int, int);
		void motion(int, int);
		void idle();
		void controlLights(int);
		void visible(int);

		void makeDinosaur();
		void drawDinosaur();
};

extern liz gliz;

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
void extrudeSolidFromPolygon(GLfloat data[][2], unsigned int dataSize, GLdouble thickness, GLuint side, GLuint edge, GLuint whole);


#endif

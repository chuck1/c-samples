
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

#include <math/vec3.h>
#include <math/vec4.h>
#include <math/mat44.h>

#include <liz.h>

/* Create a matrix that will project the desired shadow. */
void shadowMatrix(math::mat44 &shadowMat, math::vec3 groundplane, math::vec4 lightpos)
{
	math::vec4 ground(groundplane, 0.0f);
	
	GLfloat dot = ground.DotProduct(lightpos);
	
	math::mat44 diag;

	diag *= dot;

	shadowMat = diag - ( lightpos * ground );
}
void idlefunc()
{
	gliz.idle();
}
/* Press any key to redraw; good when motion stopped and
   performance reporting on. */
/* ARGSUSED */
static void key(unsigned char c, int x, int y)
{
	if (c == 27) exit(0);  /* IRIS GLism, Escape quits. */
	
	glutPostRedisplay();
}

/* Press any key to redraw; good when motion stopped and
   performance reporting on. */
/* ARGSUSED */
static void special(int k, int x, int y)
{
	glutPostRedisplay();
}
static int supportsOneDotOne(void)
{
	const char *version;
	int major, minor;

	version = (char *) glGetString(GL_VERSION);
	if (sscanf(version, "%d.%d", &major, &minor) == 2)
		return major >= 1 && minor >= 1;
	return 0;            /* OpenGL version string malformed! */
}
void redraw()
{
	gliz.redraw();
}
void mouse(int a, int b, int c, int d)
{
	gliz.mouse(a,b,c,d);
}
void motion(int a, int b)
{
	gliz.motion(a,b);
}
void visible(int a)
{
	gliz.visible(a);
}
void key()
{

}
void menu(int a)
{
	gliz.controlLights(a);
}

int main(int argc, char **argv)
{
	int i;

	glutInit(&argc, argv);

	for (i=1; i<argc; i++)
	{
		if (!strcmp("-linear", argv[i]))
		{
			gliz.linearFiltering = 1;
		}
		else if (!strcmp("-mipmap", argv[i]))
		{
			gliz.useMipmaps = 1;
		}
		else if (!strcmp("-ext", argv[i]))
		{
			gliz.forceExtension = 1;
		}
	}

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);

#if 0
	/* In GLUT 4.0, you'll be able to do this an be sure to
	   get 2 bits of stencil if the machine has it for you. */
	glutInitDisplayString("samples stencil>=2 rgb double depth");
#endif

	glutCreateWindow("Shadowy Leapin' Lizards");

	if (glutGet(GLUT_WINDOW_STENCIL_SIZE) <= 1) {
		printf("dinoshade: Sorry, I need at least 2 bits of stencil.\n");
		exit(1);
	}

	/* Register GLUT callbacks. */
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutVisibilityFunc(visible);
	glutKeyboardFunc(key);
	glutSpecialFunc(special);

	glutCreateMenu(menu);

	glutAddMenuEntry("Toggle motion", M_MOTION);
	glutAddMenuEntry("-----------------------", M_NONE);
	glutAddMenuEntry("Toggle light", M_LIGHT);
	glutAddMenuEntry("Toggle texture", M_TEXTURE);
	glutAddMenuEntry("Toggle shadows", M_SHADOWS);
	glutAddMenuEntry("Toggle reflection", M_REFLECTION);
	glutAddMenuEntry("Toggle dinosaur", M_DINOSAUR);
	glutAddMenuEntry("-----------------------", M_NONE);
	glutAddMenuEntry("Toggle reflection stenciling", M_STENCIL_REFLECTION);
	glutAddMenuEntry("Toggle shadow stenciling", M_STENCIL_SHADOW);
	glutAddMenuEntry("Toggle shadow offset", M_OFFSET_SHADOW);
	glutAddMenuEntry("----------------------", M_NONE);
	glutAddMenuEntry("Positional light", M_POSITIONAL);
	glutAddMenuEntry("Directional light", M_DIRECTIONAL);
	glutAddMenuEntry("-----------------------", M_NONE);
	glutAddMenuEntry("Toggle performance", M_PERFORMANCE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	gliz.makeDinosaur();

#ifdef GL_VERSION_1_1
	if(supportsOneDotOne() && !gliz.forceExtension)
	{
		gliz.polygonOffsetVersion = ONE_DOT_ONE;
		glPolygonOffset(-2.0, -1.0);
	}
	else
#endif
	{
#ifdef GL_EXT_polygon_offset
		/* check for the polygon offset extension */
		if (glutExtensionSupported("GL_EXT_polygon_offset"))
		{
			gliz.polygonOffsetVersion = EXTENSION;
			//glPolygonOffsetEXT(-0.1, -0.002);
		}
		else 
#endif
		{
			gliz.polygonOffsetVersion = MISSING;
			printf("\ndinoshine: Missing polygon offset.\n");
			printf("           Expect shadow depth aliasing artifacts.\n\n");
		}
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(3.0);

	glMatrixMode(GL_PROJECTION);
	gluPerspective( /* field of view in degree */ 40.0,
			/* aspect ratio */ 1.0,
			/* Z near */ 20.0, /* Z far */ 100.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(
			0.0, 8.0, 60.0,		/* eye is at (0,0,30) */
			0.0, 8.0, 0.0,		/* center is at (0,0,0) */
			0.0, 1.0, 0.);		/* up is in postivie Y direction */

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gliz.light_.color_);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	makeFloorTexture();

	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}




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

#include <lizard/liz.h>

/* Create a matrix that will project the desired shadow. */
void shadowMatrix(MATRIX4X4 &shadowMat,
		VECTOR4D groundplane,
		VECTOR4D lightpos)
{
	/* Find dot product between light position vector and ground plane normal. */
	GLfloat dot = groundplane.DotProduct(lightpos);

	MATRIX4X4 diag(
			dot, 0.0f,0.0f,0.0f,
			0.0f,dot, 0.0f,0.0f,
			0.0f,0.0f,dot, 0.0f,
			0.0f,0.0f,0.0f,dot);

	shadowMat = diag - ( lightpos * groundplane );

}

/* Find the plane equation given 3 points. */
void findPlane(VECTOR4D &plane, VECTOR3D v0, VECTOR3D v1, VECTOR3D v2)
{
	VECTOR3D vec0, vec1;

	/* Need 2 vectors to find cross product. */
	vec0 = v1 - v0;
	vec1 = v2 - v0;

	/* find cross product to get A, B, and C of plane equation */
	VECTOR3D cross = vec0.CrossProduct(vec1);

	plane.x = cross.x;
	plane.y = cross.y;
	plane.z = cross.z;

	plane.w = -cross.DotProduct(v0);
}

void redraw()
{
	int start, end;

	if (reportSpeed)
	{
		start = glutGet(GLUT_ELAPSED_TIME);
	}

	/* Clear; default stencil clears to zero. */
	if ((stencilReflection && renderReflection) || (stencilShadow && renderShadow))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	else
	{
		/* Avoid clearing stencil when not using it. */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/* Reposition the light source. */
	lightPosition[0] = 12*cos(lightAngle);
	lightPosition[1] = lightHeight;
	lightPosition[2] = 12*sin(lightAngle);
	if (directionalLight)
	{
		lightPosition[3] = 0.0;
	}
	else
	{
		lightPosition[3] = 1.0;
	}

	shadowMatrix(floorShadow, floorPlane, lightPosition);

	glPushMatrix();
	/* Perform scene rotations based on user mouse input. */
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	/* Tell GL new light source position. */
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	if (renderReflection)
	{
		if (stencilReflection)
		{
			/* We can eliminate the visual "artifact" of seeing the "flipped"
			   dinosaur underneath the floor by using stencil.  The idea is
			   draw the floor without color or depth update but so that 
			   a stencil value of one is where the floor will be.  Later when
			   rendering the dinosaur reflection, we will only update pixels
			   with a stencil value of 1 to make sure the reflection only
			   lives on the floor, not below the floor. */

			/* Don't update color or depth. */
			glDisable(GL_DEPTH_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			/* Draw 1 into the stencil buffer. */
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

			/* Now render floor; floor pixels just get their stencil set to 1. */
			drawFloor();

			/* Re-enable update of color and depth. */ 
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glEnable(GL_DEPTH_TEST);

			/* Now, only render where stencil is set to 1. */
			glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}

		glPushMatrix();

		/* The critical reflection step: Reflect dinosaur through the floor
		   (the Y=0 plane) to make a relection. */
		glScalef(1.0, -1.0, 1.0);

		/* Reflect the light position. */
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		/* To avoid our normals getting reversed and hence botched lighting
		   on the reflection, turn on normalize.  */
		glEnable(GL_NORMALIZE);
		glCullFace(GL_FRONT);

		/* Draw the reflected dinosaur. */
		drawDinosaur();

		/* Disable noramlize again and re-enable back face culling. */
		glDisable(GL_NORMALIZE);
		glCullFace(GL_BACK);

		glPopMatrix();

		/* Switch back to the unreflected light position. */
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		if (stencilReflection)
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	/* Back face culling will get used to only draw either the top or the
	   bottom floor.  This let's us get a floor with two distinct
	   appearances.  The top floor surface is reflective and kind of red.
	   The bottom floor surface is not reflective and blue. */

	/* Draw "bottom" of floor in blue. */
	glFrontFace(GL_CW);  /* Switch face orientation. */
	glColor4f(0.1, 0.1, 0.7, 1.0);
	drawFloor();
	glFrontFace(GL_CCW);

	if (renderShadow)
	{
		if (stencilShadow)
		{
			/* Draw the floor with stencil value 3.  This helps us only 
			   draw the shadow once per floor pixel (and only on the
			   floor pixels). */
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}
	}

	/* Draw "top" of floor.  Use blending to blend in reflection. */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.7, 0.0, 0.0, 0.3);
	glColor4f(1.0, 1.0, 1.0, 0.3);
	drawFloor();
	glDisable(GL_BLEND);

	if (renderDinosaur)
	{
		/* Draw "actual" dinosaur, not its reflection. */
		drawDinosaur();
	}

	if (renderShadow)
	{

		/* Render the projected shadow. */

		if (stencilShadow)
		{

			/* Now, only render where stencil is set above 2 (ie, 3 where
			   the top floor is).  Update stencil with 2 where the shadow
			   gets drawn so we don't redraw (and accidently reblend) the
			   shadow). */
			glStencilFunc(GL_LESS, 2, 0xffffffff);  /* draw if ==1 */
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		}

		/* To eliminate depth buffer artifacts, we use polygon offset
		   to raise the depth of the projected shadow slightly so
		   that it does not depth buffer alias with the floor. */
		if (offsetShadow)
		{
			switch (polygonOffsetVersion)
			{
				case EXTENSION:
#ifdef GL_EXT_polygon_offset
					glEnable(GL_POLYGON_OFFSET_EXT);
					break;
#endif
#ifdef GL_VERSION_1_1
				case ONE_DOT_ONE:
					glEnable(GL_POLYGON_OFFSET_FILL);
					break;
#endif
				case MISSING:
					/* Oh well. */
					break;
			}
		}

		/* Render 50% black shadow color on top of whatever the
		   floor appareance is. */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);  /* Force the 50% black. */
		glColor4f(0.0, 0.0, 0.0, 0.5);

		glPushMatrix();
		/* Project the shadow. */
		glMultMatrixf((GLfloat *) floorShadow);
		drawDinosaur();
		glPopMatrix();

		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);

		if (offsetShadow)
		{
			switch (polygonOffsetVersion)
			{
#ifdef GL_EXT_polygon_offset
				case EXTENSION:
					glDisable(GL_POLYGON_OFFSET_EXT);
					break;
#endif
#ifdef GL_VERSION_1_1
				case ONE_DOT_ONE:
					glDisable(GL_POLYGON_OFFSET_FILL);
					break;
#endif
				case MISSING:
					/* Oh well. */
					break;
			}
		}
		if (stencilShadow)
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	if (directionalLight)
	{
		/* Draw an arrowhead. */
		glDisable(GL_CULL_FACE);
		glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
		glRotatef(lightAngle * -180.0 / M_PI, 0, 1, 0);
		glRotatef(atan(lightHeight/12) * 180.0 / M_PI, 0, 0, 1);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
		glVertex3f(2, 1, 1);
		glVertex3f(2, -1, 1);
		glVertex3f(2, -1, -1);
		glVertex3f(2, 1, -1);
		glVertex3f(2, 1, 1);
		glEnd();
		/* Draw a white line from light direction. */
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(5, 0, 0);
		glEnd();
		glEnable(GL_CULL_FACE);
	}
	else
	{
		/* Draw a yellow ball at the light source. */
		glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
		glutSolidSphere(1.0, 5, 5);
	}
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glPopMatrix();

	if (reportSpeed) {
		glFinish();
		end = glutGet(GLUT_ELAPSED_TIME);
		printf("Speed %.3g frames/sec (%d ms)\n", 1000.0/(end-start), end-start);
	}

	glutSwapBuffers();
}

/* ARGSUSED2 */
static void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) {
			lightMoving = 1;
			lightStartX = x;
			lightStartY = y;
		}
		if (state == GLUT_UP) {
			lightMoving = 0;
		}
	}
}

/* ARGSUSED1 */
static void motion(int x, int y)
{
	if(moving)
	{
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}

	if (lightMoving)
	{
		lightAngle += (x - lightStartX)/40.0;
		lightHeight += (lightStartY - y)/20.0;
		lightStartX = x;
		lightStartY = y;
		glutPostRedisplay();
	}
}

/* Advance time varying state when idle callback registered. */
static void idle(void)
{
	static float time = 0.0;

	time = glutGet(GLUT_ELAPSED_TIME) / 500.0;

	jump = 4.0 * fabs(sin(time)*0.5);
	if (!lightMoving) {
		lightAngle += 0.03;
	}
	glutPostRedisplay();
}

static void controlLights(int value)
{
	switch (value) {
		case M_NONE:
			return;
		case M_MOTION:
			animation = 1 - animation;
			if (animation) {
				glutIdleFunc(idle);
			} else {
				glutIdleFunc(NULL);
			}
			break;
		case M_LIGHT:
			lightSwitch = !lightSwitch;
			if (lightSwitch) {
				glEnable(GL_LIGHT0);
			} else {
				glDisable(GL_LIGHT0);
			}
			break;
		case M_TEXTURE:
			useTexture = !useTexture;
			break;
		case M_SHADOWS:
			renderShadow = 1 - renderShadow;
			break;
		case M_REFLECTION:
			renderReflection = 1 - renderReflection;
			break;
		case M_DINOSAUR:
			renderDinosaur = 1 - renderDinosaur;
			break;
		case M_STENCIL_REFLECTION:
			stencilReflection = 1 - stencilReflection;
			break;
		case M_STENCIL_SHADOW:
			stencilShadow = 1 - stencilShadow;
			break;
		case M_OFFSET_SHADOW:
			offsetShadow = 1 - offsetShadow;
			break;
		case M_POSITIONAL:
			directionalLight = 0;
			break;
		case M_DIRECTIONAL:
			directionalLight = 1;
			break;
		case M_PERFORMANCE:
			reportSpeed = 1 - reportSpeed;
			break;
	}
	glutPostRedisplay();
}

/* When not visible, stop animating.  Restart when visible again. */
static void visible(int vis)
{
	if (vis == GLUT_VISIBLE) {
		if (animation)
			glutIdleFunc(idle);
	} else {
		if (!animation)
			glutIdleFunc(NULL);
	}
}

/* Press any key to redraw; good when motion stopped and
   performance reporting on. */
/* ARGSUSED */
static void key(unsigned char c, int x, int y)
{
	if (c == 27) {
		exit(0);  /* IRIS GLism, Escape quits. */
	}
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

	int
main(int argc, char **argv)
{
	int i;

	glutInit(&argc, argv);

	for (i=1; i<argc; i++)
	{
		if (!strcmp("-linear", argv[i]))
		{
			linearFiltering = 1;
		}
		else if (!strcmp("-mipmap", argv[i]))
		{
			useMipmaps = 1;
		}
		else if (!strcmp("-ext", argv[i]))
		{
			forceExtension = 1;
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

	glutCreateMenu(controlLights);

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
	makeDinosaur();

#ifdef GL_VERSION_1_1
	if (supportsOneDotOne() && !forceExtension) {
		polygonOffsetVersion = ONE_DOT_ONE;
		glPolygonOffset(-2.0, -1.0);
	}
	else
#endif
	{
#ifdef GL_EXT_polygon_offset
		/* check for the polygon offset extension */
		if (glutExtensionSupported("GL_EXT_polygon_offset"))
		{
			polygonOffsetVersion = EXTENSION;
			//glPolygonOffsetEXT(-0.1, -0.002);
		}
		else 
#endif
		{
			polygonOffsetVersion = MISSING;
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
	gluLookAt(0.0, 8.0, 60.0,  /* eye is at (0,0,30) */
			0.0, 8.0, 0.0,      /* center is at (0,0,0) */
			0.0, 1.0, 0.);      /* up is in postivie Y direction */

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	makeFloorTexture();

	/* Setup floor plane for projected shadow calculations. */
	findPlane(floorPlane, floorVertices[1], floorVertices[2], floorVertices[3]);

	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}



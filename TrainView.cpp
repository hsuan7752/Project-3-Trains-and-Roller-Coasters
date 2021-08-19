/************************************************************************
     File:        TrainView.cpp

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu

     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						The TrainView is the window that actually shows the 
						train. Its a
						GL display canvas (Fl_Gl_Window).  It is held within 
						a TrainWindow
						that is the outer window with all the widgets. 
						The TrainView needs 
						to be aware of the window - since it might need to 
						check the widgets to see how to draw

	  Note:        we need to have pointers to this, but maybe not know 
						about it (beware circular references)

     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/

#include <iostream>
#include <Fl/fl.h>

// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
//#include "GL/gl.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "GL/glu.h"
#include <math.h>

#include "TrainView.H"
#include "TrainWindow.H"
#include "Utilities/3DUtils.H"


#ifdef EXAMPLE_SOLUTION
#	include "TrainExample/TrainExample.H"
#endif


//************************************************************************
//
// * Constructor to set up the GL window
//========================================================================
TrainView::
TrainView(int x, int y, int w, int h, const char* l) 
	: Fl_Gl_Window(x,y,w,h,l)
//========================================================================
{
	mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );

	resetArcball();
}

//************************************************************************
//
// * Reset the camera to look at the world
//========================================================================
void TrainView::
resetArcball()
//========================================================================
{
	// Set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this, 40, 250, .2f, .4f, 0);
}

//************************************************************************
//
// * FlTk Event handler for the window
//########################################################################
// TODO: 
//       if you want to make the train respond to other events 
//       (like key presses), you might want to hack this.
//########################################################################
//========================================================================
int TrainView::handle(int event)
{
	// see if the ArcBall will handle the event - if it does, 
	// then we're done
	// note: the arcball only gets the event if we're in world view
	if (tw->worldCam->value())
		if (arcball.handle(event)) 
			return 1;

	// remember what button was used
	static int last_push;

	switch(event) {
		// Mouse button being pushed event
		case FL_PUSH:
			last_push = Fl::event_button();
			// if the left button be pushed is left mouse button
			if (last_push == FL_LEFT_MOUSE  ) {
				doPick();
				damage(1);
				return 1;
			};
			break;

	   // Mouse button release event
		case FL_RELEASE: // button release
			damage(1);
			last_push = 0;
			return 1;

		// Mouse button drag event
		case FL_DRAG:

			// Compute the new control point position
			if ((last_push == FL_LEFT_MOUSE) && (selectedCube >= 0)) {
				ControlPoint* cp = &m_pTrack->points[selectedCube];

				double r1x, r1y, r1z, r2x, r2y, r2z;
				getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

				double rx, ry, rz;
				mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z, 
								static_cast<double>(cp->pos.x), 
								static_cast<double>(cp->pos.y),
								static_cast<double>(cp->pos.z),
								rx, ry, rz,
								(Fl::event_state() & FL_CTRL) != 0);

				cp->pos.x = (float) rx;
				cp->pos.y = (float) ry;
				cp->pos.z = (float) rz;
				damage(1);
			}
			break;

		// in order to get keyboard events, we need to accept focus
		case FL_FOCUS:
			return 1;

		// every time the mouse enters this window, aggressively take focus
		case FL_ENTER:	
			focus(this);
			break;

		case FL_KEYBOARD:
		 		int k = Fl::event_key();
				int ks = Fl::event_state();
				if (k == 'p') {
					// Print out the selected control point information
					if (selectedCube >= 0) 
						printf("Selected(%d) (%g %g %g) (%g %g %g)\n",
								 selectedCube,
								 m_pTrack->points[selectedCube].pos.x,
								 m_pTrack->points[selectedCube].pos.y,
								 m_pTrack->points[selectedCube].pos.z,
								 m_pTrack->points[selectedCube].orient.x,
								 m_pTrack->points[selectedCube].orient.y,
								 m_pTrack->points[selectedCube].orient.z);
					else
						printf("Nothing Selected\n");

					return 1;
				};
				break;
	}

	return Fl_Gl_Window::handle(event);
}

//************************************************************************
//
// * this is the code that actually draws the window
//   it puts a lot of the work into other routines to simplify things
//========================================================================
void TrainView::draw()
{

	//*********************************************************************
	//
	// * Set up basic opengl informaiton
	//
	//**********************************************************************
	//initialized glad
	if (gladLoadGL())
	{
		//initiailize VAO, VBO, Shader...
	}
	else
		throw std::runtime_error("Could not initialize GLAD!");

	// Set up the view port
	glViewport(0,0,w(),h());

	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0,0,.3f,0);		// background should be blue

	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here

	//######################################################################
	// TODO: 
	// you might want to set the lighting up differently. if you do, 
	// we need to set up the lights AFTER setting up the projection
	//######################################################################
	// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// top view only needs one light
	if (tw->topCam->value()) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	} else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}
	

	//*********************************************************************
	//
	// * set the light parameters
	//
	//**********************************************************************
	GLfloat lightPosition1[]	= {0,1,1,0}; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[]	= {1, 0, 0, 0};
	GLfloat lightPosition3[]	= {0, -1, 0, 0};
	GLfloat yellowLight[]		= {0.5f, 0.5f, .1f, 1.0};
	GLfloat whiteLight[]			= {1.0f, 1.0f, 1.0f, 1.0};
	GLfloat blueLight[]			= {.1f,.1f,.3f,1.0};
	GLfloat grayLight[]			= {.3f, .3f, .3f, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);



	//*********************************************************************
	// now draw the ground plane
	//*********************************************************************
	// set to opengl fixed pipeline(use opengl 1.x draw function)
	glUseProgram(0);

	setupFloor();
	glDisable(GL_LIGHTING);
	drawFloor(200,10);


	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	glEnable(GL_LIGHTING);
	setupObjects();

	drawStuff();

	// this time drawing is for shadows (except for top view)
	if (!tw->topCam->value()) {
		setupShadows();
		drawStuff(true);
		unsetupShadows();
	}

	if (tw->spotLT->value())
	{

	}
	else
		glDisable(GL_LIGHT1);

	if (tw->pointLT->value())
	{
		float yellowAmbientDiffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		float position[] = { 0.0f, 100.0f, 0.0f, 1.0f };

		glLightfv(GL_LIGHT1, GL_AMBIENT, yellowAmbientDiffuse);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowAmbientDiffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, position);
	}
	else
		glDisable(GL_LIGHT1);
}

//************************************************************************
//
// * This sets up both the Projection and the ModelView matrices
//   HOWEVER: it doesn't clear the projection first (the caller handles
//   that) - its important for picking
//========================================================================
void TrainView::
setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(w()) / static_cast<float>(h());

	// Check whether we use the world camp
	if (tw->worldCam->value())
		arcball.setProjection(false);
	// Or we use the top cam
	else if (tw->topCam->value()) {
		float wi, he;
		if (aspect >= 1) {
			wi = 110;
			he = wi / aspect;
		} 
		else {
			he = 110;
			wi = he * aspect;
		}

		// Set up the top camera drop mode to be orthogonal and set
		// up proper projection matrix
		glMatrixMode(GL_PROJECTION);
		glOrtho(-wi, wi, -he, he, 200, -200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90,1,0,0);
	} 
	// Or do the train view or other view here
	//####################################################################
	// TODO: 
	// put code for train view projection here!	
	//####################################################################
	else {
#ifdef EXAMPLE_SOLUTION
		trainCamView(this,aspect);
#endif
	}
}

//************************************************************************
//
// * this draws all of the stuff in the world
//
//	NOTE: if you're drawing shadows, DO NOT set colors (otherwise, you get 
//       colored shadows). this gets called twice per draw 
//       -- once for the objects, once for the shadows
//########################################################################
// TODO: 
// if you have other objects in the world, make sure to draw them
//########################################################################
//========================================================================
void TrainView::drawStuff(bool doingShadows)
{
	// Draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	if (!tw->trainCam->value()) {
		for(size_t i=0; i<m_pTrack->points.size(); ++i) {
			if (!doingShadows) {
				if ( ((int) i) != selectedCube)
					glColor3ub(240, 60, 60);
				else
					glColor3ub(240, 240, 30);
			}
			m_pTrack->points[i].draw();
		}
	}
	// draw the track
	//####################################################################
	// TODO: 
	// call your own track drawing code
	//####################################################################

	drawTrack(doingShadows);

#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif

	// draw the train
	//####################################################################
	// TODO: 
	//	call your own train drawing code
	//####################################################################
#ifdef EXAMPLE_SOLUTION
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
		drawTrain(this, doingShadows);
#endif

	if (!tw->trainCam->value())
		drawTrain(doingShadows);
}

// 
//************************************************************************
//
// * this tries to see which control point is under the mouse
//	  (for when the mouse is clicked)
//		it uses OpenGL picking - which is always a trick
//########################################################################
// TODO: 
//		if you want to pick things other than control points, or you
//		changed how control points are drawn, you might need to change this
//########################################################################
//========================================================================
void TrainView::
doPick()
//========================================================================
{
	// since we'll need to do some GL stuff so we make this window as 
	// active window
	make_current();		

	// where is the mouse?
	int mx = Fl::event_x(); 
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluPickMatrix((double)mx, (double)(viewport[3]-my), 
						5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100,buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	// draw the cubes, loading the names as we go
	for(size_t i=0; i<m_pTrack->points.size(); ++i) {
		glLoadName((GLuint) (i+1));
		m_pTrack->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3]-1;
	} else // nothing hit, nothing selected
		selectedCube = -1;

	printf("Selected Cube %d\n",selectedCube);
}

enum Type
{
	LINEAR = 1, CARDINAL, B_SPLINE
};
#define PI 3.14159265
void TrainView::
drawTrack(bool doingShadow)
{
	unsigned int DIVIDE_LINE = 500;

	int splineType = -1;

	splineType = tw->splineBrowser->value();

	float distance = 0;

	for (size_t i = 0; i < m_pTrack->points.size(); ++i)
	{
		//pos
		Pnt3f cp_pos_p1 = m_pTrack->points[i].pos;
		Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
		Pnt3f cp_pos_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
		Pnt3f cp_pos_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].pos;

		//orient
		Pnt3f cp_orient_p1 = m_pTrack->points[i].orient;
		Pnt3f cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
		Pnt3f cp_orient_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].orient;
		Pnt3f cp_orient_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].orient;

		float percent = 1.0f / DIVIDE_LINE;
		float t = 0;
		Pnt3f qt, orient_t;

		switch (splineType)
		{
		case Type::LINEAR:
			qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
			break;
		case Type::CARDINAL:
			qt = cp_pos_p2;
			break;
		case Type::B_SPLINE:
			qt = cp_pos_p1 * (1.0f / 6.0f) + cp_pos_p2 * (4.0f / 6.0f) + cp_pos_p3 * (1.0f / 6.0f);
			break;
		}

		for (size_t j = 0; j < DIVIDE_LINE; ++j)
		{
			Pnt3f qt0 = qt;
			t += percent;

			float G[3][4]{ { cp_pos_p1.x, cp_pos_p2.x, cp_pos_p3.x, cp_pos_p4.x },
						   { cp_pos_p1.y, cp_pos_p2.y, cp_pos_p3.y, cp_pos_p4.y }, 
						   { cp_pos_p1.z, cp_pos_p2.z, cp_pos_p3.z, cp_pos_p4.z } };

			float M_cardinal[4][4]{ { -0.5,  1.5, -1.5,  0.5 },
									{    1, -2.5,    2, -0.5 },
									{ -0.5,    0,  0.5,    0 },
									{    0,    1,    0,    0 } };

			float M_b_spline[4][4]{ { -0.1667,    0.5,   -0.5, 0.1667 },
									{     0.5,     -1,    0.5,      0 },
									{    -0.5,      0,    0.5,      0 },
									{  0.1667, 0.6667, 0.1667,      0 } };

			float T[4] { pow(t, 3), pow(t, 2), t, 1 };

			float C[4]{ 0 };

			switch (splineType)
			{
			case Type::LINEAR:
				qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
				orient_t = (1 - t) * cp_orient_p1 + t * cp_orient_p2;
				break;
			case Type::CARDINAL:
				Mult_Q(C, M_cardinal, T);
				qt = cp_pos_p1 * C[0] + cp_pos_p2 * C[1] + cp_pos_p3 * C[2] + cp_pos_p4 * C[3];
				orient_t = cp_orient_p1 * C[0] + cp_orient_p2 * C[1] + cp_orient_p3 * C[2] + cp_orient_p4 * C[3];
				break;
			case Type::B_SPLINE:
				Mult_Q(C, M_b_spline, T);
				qt = cp_pos_p1 * C[0] + cp_pos_p2 * C[1] + cp_pos_p3 * C[2] + cp_pos_p4 * C[3];
				orient_t = cp_orient_p1 * C[0] + cp_orient_p2 * C[1] + cp_orient_p3 * C[2] + cp_orient_p4 * C[3];
				break;
			}

			Pnt3f qt1 = qt;

			orient_t.normalize();

			Pnt3f sub;
			sub.x = qt1.x - qt0.x;
			sub.y = qt1.y - qt0.y;
			sub.z = qt1.z - qt0.z;

			Pnt3f cross_t = sub * orient_t;
			cross_t.normalize();
			cross_t = cross_t * 2.5f;

			
			glBegin(GL_LINES);
			if (!doingShadow)
				glColor3ub(32, 32, 64);
			glVertex3f(qt0.x + cross_t.x, qt0.y + cross_t.y, qt0.z + cross_t.z);
			glVertex3f(qt1.x + cross_t.x, qt1.y + cross_t.y, qt1.z + cross_t.z);

			glVertex3f(qt0.x - cross_t.x, qt0.y - cross_t.y, qt0.z - cross_t.z);
			glVertex3f(qt1.x - cross_t.x, qt1.y - cross_t.y, qt1.z - cross_t.z);

			/*glVertex3f(qt0.x, qt0.y, qt0.z);
			glVertex3f(qt1.x, qt1.y, qt1.z);*/

			glEnd();
			glLineWidth(3);

			//draw Sleeper
			distance += sqrtf(pow(sub.x, 2) + pow(sub.y, 2) + pow(sub.z, 2));

			Pnt3f AC(qt1.x - qt0.x, qt1.y - qt0.y, qt1.z - qt0.z);
			Pnt3f AB(1.0f, 0.0f, 0.0f);

			float cos_y = (float)(AC.x * AB.x + AC.y * AB.y + AC.z * AB.z) 
									/ abs(sqrt(pow(AC.x, 2) + pow(AC.y, 2) + pow(AC.z, 2))) * abs(sqrt(pow(AB.x, 2) + pow(AB.y, 2) + pow(AB.z, 2)));
			float sin_y = (float)sqrt(1 - pow(cos_y, 2));
			if ((AC.x < 0 && AC.z > 0) || (AC.x > 0 && AC.z > 0))
				sin_y = -sin_y;

			float rotationMatrix[3][3] = { { cos_y, 0, sin_y },
										  { 0, 1, 0 },
										  { -sin_y, 0, cos_y } };
			double* v0 = new double[3]{ - 1.5, 0, + 5 };
			double* v1 = new double[3]{ + 1.5, 0, + 5 };
			double* v2 = new double[3]{ + 1.5, 0, - 5 };
			double* v3 = new double[3]{ - 1.5, 0, - 5 };

			v0 = rotate(rotationMatrix, v0);
			v1 = rotate(rotationMatrix, v1);
			v2 = rotate(rotationMatrix, v2);
			v3 = rotate(rotationMatrix, v3);

			if (distance > 8)
			{
				distance = 0;
				/*glBegin(GL_QUADS);
				if (!doingShadow)
					glColor3ub(255, 255, 255);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3dv(v0);
				glTexCoord2f(0.1f, 1.0f);
				glVertex3dv(v1);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3dv(v2);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3dv(v3);
				glEnd();*/

				//down
				glBegin(GL_QUADS);
				if (!doingShadow)
					glColor3ub(100, 80, 100);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(qt1.x + v0[0], qt1.y + v0[1], qt1.z + v0[2]);
				glTexCoord2f(0.1f, 1.0f);
				glVertex3f(qt1.x + v1[0], qt1.y + v1[1], qt1.z + v1[2]);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(qt1.x + v2[0], qt1.y + v2[1], qt1.z + v2[2]);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(qt1.x + v3[0], qt1.y + v3[1], qt1.z + v3[2]);
				glEnd();

				//up
				glBegin(GL_QUADS);
				if (!doingShadow)
					glColor3ub(100, 80, 100);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(qt1.x + v0[0], qt1.y + v0[1] + 0.5, qt1.z + v0[2]);
				glTexCoord2f(0.1f, 1.0f);
				glVertex3f(qt1.x + v1[0], qt1.y + v1[1] + 0.5, qt1.z + v1[2]);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(qt1.x + v2[0], qt1.y + v2[1] + 0.5, qt1.z + v2[2]);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(qt1.x + v3[0], qt1.y + v3[1] + 0.5, qt1.z + v3[2]);
				glEnd();

				//front
				


				/*glBegin(GL_QUADS);
				if (!doingShadow)
					glColor3ub(0, 255, 255);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(qt0.x - 1.5, qt0.y, qt0.z + 5);
				glTexCoord2f(0.1f, 1.0f);
				glVertex3f(qt0.x + 1.5, qt0.y, qt0.z + 5);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(qt0.x + 1.5, qt0.y, qt0.z - 5);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(qt0.x - 1.5, qt0.y, qt0.z - 5);
				glEnd();*/
			}
		}
	}
}

void TrainView::
Mult_Q(float* C, float M[][4], float* T)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			C[i] += M[j][i] * T[j];
}

void TrainView::
drawTrain(bool doingShadow)
{
	Pnt3f qt, orient_t;

	int splineType = -1;

	splineType = tw->splineBrowser->value();

	for (size_t i = 0; i < m_pTrack->points.size(); ++i)
	{
		//pos
		Pnt3f cp_pos_p1 = m_pTrack->points[i].pos;
		Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
		Pnt3f cp_pos_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
		Pnt3f cp_pos_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].pos;
		//orient
		Pnt3f cp_orient_p1 = m_pTrack->points[i].orient;
		Pnt3f cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
		Pnt3f cp_orient_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].orient;
		Pnt3f cp_orient_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].orient;

		float t = 0;

		switch (splineType)
		{
		case Type::LINEAR:
			qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
			break;
		case Type::CARDINAL:
			qt = cp_pos_p2;
			break;
		case Type::B_SPLINE:
			qt = cp_pos_p1 * (1.0f / 6.0f) + cp_pos_p2 * (4.0f / 6.0f) + cp_pos_p3 * (1.0f / 6.0f);
			break;
		}
	}

	//front
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 1, qt.z - 5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 1, qt.z - 5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 11, qt.z - 5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 11, qt.z - 5);
	glEnd();

	//back
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 1, qt.z + 5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 1, qt.z + 5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 11, qt.z + 5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 11, qt.z + 5);
	glEnd();

	//left
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 1, qt.z - 5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(qt.x - 5, qt.y + 1, qt.z + 5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(qt.x - 5, qt.y + 11, qt.z + 5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 11, qt.z - 5);
	glEnd();

	//right
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(qt.x + 5, qt.y + 1, qt.z - 5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 1, qt.z + 5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 11, qt.z + 5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(qt.x + 5, qt.y + 11, qt.z - 5);
	glEnd();

	//up
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 11, qt.z + 5);
	glTexCoord2f(0.1f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 11, qt.z + 5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 11, qt.z - 5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 11, qt.z - 5);
	glEnd();

	//down
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 1, qt.z + 5);
	glTexCoord2f(0.1f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 1, qt.z + 5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(qt.x + 5, qt.y + 1, qt.z - 5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(qt.x - 5, qt.y + 1, qt.z - 5);
	glEnd();
}

double* TrainView::
rotate(float m[][3], double* p)
{
	double* n = new double[]{0, 0, 0};
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			n[i] += m[i][j] * p[j];
	return n;
}

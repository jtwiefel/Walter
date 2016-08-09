/*
 * BotViewController.cpp
 *
 *  Created on: 07.08.2016
 *      Author: SuperJochenAlt
 */


#include <windows.h>  // openGL windows
#include <thread>

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <GL/Glui.h>

#include "spatial.h"
#include "Util.h"
#include <ui/BotView.h>
#include <ui/BotWindowCtrl.h>

using namespace std;

static GLfloat glBotArmColor[] 			= { 1.0f, 0.3f, 0.2f };
static GLfloat glBotJointColor[] 		= { 0.5f, 0.6f, 0.6f };
static GLfloat glCoordSystemColor4v[] 	= { 0.03f, 0.27f, 0.32f,0.5f };
static GLfloat glRasterColor3v[] 		= { 0.73f, 0.77f, 0.82f };
static GLfloat glSubWindowColor[] 		= {0.97,0.97,0.97};
static GLfloat glWindowTitleColor[] 	= { 1.0f, 1.0f, 1.0f };

// 3d moving window eye position
/*
float currEyeAngle= -45;					// current eye position of moveable subwindow
float currEyeHeightAngle= 0;				// current eye position of moveable subwindow
float currEyeDistance = ViewEyeDistance;	// current eye distance of moveable subwindow
*/

/*
static int leftButtonMouseX,leftButtonMouseY;
static int lastMouseScroll;
static bool leftMouseButton;

void SubWindow3dMotionCallback(int x, int y) {
	if (leftMouseButton) {
		float viewAngle = (float) (x-leftButtonMouseX);
		float heightAngle = (float) (y-leftButtonMouseY);
		currEyeAngle -= viewAngle;
		currEyeHeightAngle -= heightAngle;
	}

	currEyeDistance -= 20*lastMouseScroll;
	lastMouseScroll = 0;
	currEyeDistance = constrain(currEyeDistance,ViewEyeDistance/3,ViewEyeDistance*3);
	currEyeHeightAngle = constrain(currEyeHeightAngle,-90.0f,45.0f);

	float eyePosition[3] = {
			currEyeDistance*( sinf(radians(currEyeAngle)) * cosf(radians(currEyeHeightAngle))),
			ViewBotHeight - currEyeDistance*sinf(radians(currEyeHeightAngle)),
			currEyeDistance*(cosf(radians(currEyeAngle)) * cosf(radians(currEyeHeightAngle)))};
	botWindowCtrl.bottomRight.setEyePosition(eyePosition);
	if (leftMouseButton) {
		leftButtonMouseX = x;
		leftButtonMouseY = y;
	}

  glutPostRedisplay();
}


void SubWindows3DMouseCallback(int button, int button_state, int x, int y )
{
    leftMouseButton = false;

	if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {
	    leftButtonMouseX = x;
	    leftButtonMouseY = y;
	    leftMouseButton = true;
	}

	// Wheel reports as button 3(scroll up) and button 4(scroll down)
	if ((button == 3) || (button == 4)) // It's a wheel event
	{
		// Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
		if (button_state != GLUT_UP) { // Disregard redundant GLUT_UP events
			if (button == 3)
				lastMouseScroll++;
			else
				lastMouseScroll--;
			SubWindow3dMotionCallback(x,y);
		}
	}
}

*/
// compute a value floating from start to target during startup time
// (used for eye position to get a neat animation)
float BotView::startupFactor(float start, float target) {
	if (startupAnimationRatio < 1.0) {
		float myStartupRatio = 0.01;
		if (startupAnimationRatio >= 0.3)
			myStartupRatio = (startupAnimationRatio-0.3)/0.7;
		float distortedFactor = (1.0-(1.0-myStartupRatio)*(1.0-myStartupRatio));
		float startupFactorAngle = distortedFactor*PI/2.0;
		if (start == 0.0)
			return target*sin(startupFactorAngle);

		return target + (start-target)*cos(startupFactorAngle);
	}
	return target;
}

void BotView::setStartupAnimationRatio(float ratio) {
	startupAnimationRatio = ratio;
}

void BotView::setLights()
{
	const float lightDistance = 1500.0f;
  GLfloat light_ambient[] =  {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] =  {0.4, 0.4, 0.4, 1.0};
  GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position0[] = {lightDistance, 3*lightDistance, lightDistance, 0.0};		// ceiling left
  GLfloat light_position1[] = {-lightDistance, 3*lightDistance, lightDistance, 0.0};	// ceiling right
  GLfloat light_position2[] = {0, 3*lightDistance, -lightDistance, 0.0};				// far away from the back

  GLfloat mat_ambient[] =  {0.6, 0.6, 0.6, 1.0};
  GLfloat mat_diffuse[] =  {0.4, 0.8, 0.4, 1.0};
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shinynes[] = {50.0};

  glMaterialfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_LIGHT0, GL_SPECULAR, mat_shinynes);
  glMaterialfv(GL_LIGHT1, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_LIGHT1, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_LIGHT1, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_LIGHT1, GL_SPECULAR, mat_shinynes);
  glMaterialfv(GL_LIGHT2, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_LIGHT2, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_LIGHT2, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_LIGHT2, GL_SPECULAR, mat_shinynes);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);

  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
}


void BotView::printSubWindowTitle(std::string text ) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                 // Reset the model-view matrix
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glWindowTitleColor);
	glRasterPos2f(-0.9,0.8);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12,(const unsigned char*) text.c_str());
}


void BotView::drawCoordSystem(bool withRaster) {
	// draw coordinate system
	const float axisLength = 500.0f;
	const float arrowLength = 20.0f;
	const float unitLength = 100.0f;
	const float rasterLineLength = axisLength*2;
	if (withRaster) {
		glPushAttrib(GL_LIGHTING_BIT);
		glBegin(GL_LINES);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glRasterColor3v);
			glColor3fv(glRasterColor3v);
			for (float i = -rasterLineLength;i<=rasterLineLength;i = i + unitLength ) {
				glVertex3f(i, 0.0, -rasterLineLength);glVertex3f(i,0.0f, rasterLineLength);
			}
			for (float i = -rasterLineLength;i<=rasterLineLength;i = i + unitLength ) {
				glVertex3f(-rasterLineLength, 0.0f, i);glVertex3f(rasterLineLength, 0.0f, i);
			}
		glEnd();
		glPopAttrib();
	}

	glBegin(GL_LINES);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glCoordSystemColor4v);
		glColor4fv(glCoordSystemColor4v);

		// robot's x-axis
		glVertex3f(0.0f, 0.0f, -arrowLength);glVertex3f(0.0f, 0.0f, axisLength);
		glVertex3f(0.0f, 0.0f, axisLength);glVertex3f(0.0f,+arrowLength/2, axisLength-arrowLength);
		glVertex3f(0.0f, 0.0f, axisLength);glVertex3f(0.0f,-arrowLength/2, axisLength-arrowLength);
		for (float i = 0;i<axisLength;i = i + unitLength ) {
			glVertex3f(0.0f, -arrowLength/2, i);glVertex3f(0.0f,+arrowLength/2, i);
		}

		// robot's y-axis
		glVertex3f(-arrowLength, 0.0f, 0.0f);glVertex3f(axisLength, 0.0f, 0.0f);
		glVertex3f(axisLength, 0.0f, 0.0f);glVertex3f(axisLength-arrowLength, -arrowLength/2, 0.0f);
		glVertex3f(axisLength, 0.0f, 0.0f);glVertex3f(axisLength-arrowLength, arrowLength/2, 0.0f);
		for (float i = 0;i<axisLength;i = i + unitLength ) {
			glVertex3f(i, -arrowLength/2, 0.0f);glVertex3f(i,+arrowLength/2, 0.0f);
		}

		// robot's z-axis
		glVertex3f(0.0f, -arrowLength, 0.0f);glVertex3f(0.0f, axisLength,0.0f);
		glVertex3f(0.0f, axisLength,0.0f);glVertex3f(+arrowLength/2,axisLength-arrowLength, 0.0f);
		glVertex3f(0.0f, axisLength,0.0f);glVertex3f(-arrowLength/2, axisLength-arrowLength,0.0f);
		for (float i = 0;i<axisLength;i = i + unitLength ) {
			glVertex3f(-arrowLength/2, i,0.0f);glVertex3f(+arrowLength/2, i,0.0f);
		}
	glEnd();

	glRasterPos3f(axisLength+arrowLength, 0.0f, 0.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12,(const unsigned char*) "y");
	glRasterPos3f(0.0f, 0.0f, axisLength+arrowLength);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12,(const unsigned char*) "x");
	glRasterPos3f(0.0f, axisLength+arrowLength,0.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12,(const unsigned char*) "z");
}


void BotView::paintBot(const JointAngleType& angles) {
	const float baseplateRadius= 140;
	const float baseplateHeight= 20;

	const float baseLength = HipHeight;
	const float baseRadius = 60;
	const float baseJointRadius = 60;

	const float upperarmLength = UpperArmLength;
	const float upperarmJointRadius= 45;
	const float upperarmRadius = 45;

	const float forearmLength = ForearmLength;
	const float forearmJointRadius= 35;
	const float forearmRadius = 35;

	const float handLength= HandLength/3.0;
	const float handJointRadius= 23;
	const float handRadius= 23;

	const float gripperLength= HandLength/3.0;
	const float gripperRadius=10;

	const float gripperLeverLength= HandLength/3.0;
	const float gripperLeverRadius=5;

	glMatrixMode(GL_MODELVIEW);
	glClearColor(glSubWindowColor[0], glSubWindowColor[1],glSubWindowColor[2],0.0f);

	// coord system
	drawCoordSystem(true);

	// base plate
	glPushMatrix();
	glRotatef(-90.0,1.0,0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotJointColor);
	glutSolidCylinder(baseplateRadius, baseplateHeight, 36, 1);

	// shoulder
	glTranslatef(0.0, 0.0,baseplateHeight);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotArmColor);
	glutSolidCylinder(baseRadius, baseLength, 36, 1);

	// shoulder joint
	glTranslatef(0.0,0.0,baseLength);  // Move right and into the screen
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotJointColor);
	glutSolidSphere(baseJointRadius, 36, 36);

	// upperarm
	glRotatef(degrees(angles[0]),0.0,0.0, 1.0); // turn along angle
	glRotatef(degrees(angles[1]),1.0,0.0, 0.0); // rotate along base angle
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotArmColor);
	glutSolidCylinder(upperarmRadius, upperarmLength, 36, 1);

	// upperarm joint
	glTranslatef(0.0,0.0,upperarmLength);  // move to its start height
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotJointColor);
	glutSolidSphere(upperarmJointRadius, 36, 36);

	// forearm
	glRotatef(90+degrees(angles[2]),1.0,0.0, 0.0); // rotate along base angle
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotArmColor);
	glutSolidCylinder(forearmRadius, forearmLength, 36, 1);

	// forearm joint
	glRotatef(degrees(angles[3]),0.0,0.0, 1.0); // rotate along base angle
	glTranslatef(0.0,0.0,forearmLength);  // move to its start height
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotJointColor);
	glPushMatrix(),
		glTranslatef(forearmJointRadius/2,0.0,0);  // move to its start height
		glutSolidSphere(forearmJointRadius, 36, 36);
	glPopMatrix();
	glPushMatrix(),
		glTranslatef(-forearmJointRadius/2,0.0,0);  // move to its start height
		glutSolidSphere(forearmJointRadius, 36, 36);
	glPopMatrix();
	glPushMatrix(),
		glTranslatef(-forearmJointRadius/2,0.0,0);  // move to its start height
		glRotatef(90,0.0,1.0, 0.0); // rotate along base angle
		glutSolidCylinder(forearmJointRadius, forearmJointRadius, 36, 1);
	glPopMatrix();

	// hand
	glRotatef(degrees(angles[4]),1.0,0.0, 0.0); // rotate along base angle
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotArmColor);
	glutSolidCylinder(handRadius, handLength, 36, 1);

	// hand joint
	glTranslatef(0.0,0.0,handLength);  // move to its start height
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotJointColor);
	glutSolidSphere(handJointRadius, 36, 36);

	// hand
	glRotatef(degrees(angles[5]),0.0,0.0, 1.0); // rotate along base angle
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotArmColor);

	float gripperAngle = degrees(angles[6]);
	glPushMatrix();
		glRotatef(gripperAngle+12,0.0,1.0, 0.0); // rotate along base angle
		glutSolidCylinder(gripperLeverRadius, gripperLeverLength, 36, 1);
		glTranslatef(0,0.0,gripperLeverLength);
		glRotatef(-gripperAngle-12,0.0,1.0, 0.0); // rotate along base angle
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotJointColor);
		glutSolidSphere(gripperRadius, 36, 36);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotArmColor);
		glutSolidCylinder(gripperRadius, gripperLength, 36, 1);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-gripperAngle-12,0.0,1.0, 0.0); // rotate along base angle
		glutSolidCylinder(gripperLeverRadius, gripperLeverLength, 36, 1);
		glTranslatef(0,0.0,gripperLeverLength);
		glRotatef(gripperAngle+12,0.0,1.0, 0.0); // rotate along base angle
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotJointColor);
		glutSolidSphere(gripperRadius, 36, 36);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBotArmColor);
		glutSolidCylinder(gripperRadius, gripperLength, 36, 1);
	glPopMatrix();

	glPopMatrix();
}

int BotView::create(int mainWindow, string pTitle, View pView) {
	// initially start with zero size, will be resized in reshape
	title = pTitle;
	view = pView;
	windowHandle = glutCreateSubWindow(mainWindow, 1,1,1,1);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);   							// Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    							// Set the type of depth-test
	glShadeModel(GL_SMOOTH);   							// Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 	// Nice perspective corrections

	setLights();
	switch (view) {
	case TOP_VIEW: {
		float pos[] = {0,ViewEyeDistance, 0};
		setEyePosition(pos);
		break;
	}
	case FRONT_VIEW: {
		float pos[] = {0,ViewBotHeight/2, ViewEyeDistance};
		setEyePosition(pos);
		break;
	}
	case RIGHT_VIEW: {
		float pos[] = {-ViewEyeDistance,ViewBotHeight/2, 0};
		setEyePosition(pos);
		break;
	}
	case _3D_VIEW: {
		float pos[] = {ViewEyeDistance*sinf(radians(-45)),ViewBotHeight, ViewEyeDistance*cosf(radians(-45))};
		setEyePosition(pos);
		break;
	}
	default:
		break;
	}

	return windowHandle;
}

void BotView::display() {
	glutSetWindow(windowHandle);
	glClearColor(glSubWindowColor[0], glSubWindowColor[1], glSubWindowColor[2], 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printSubWindowTitle(title);

	setWindowPerspective();
	paintBot(angles);
}

void BotView::reshape(int x,int y, int w, int h) {
	glutSetWindow(windowHandle);
	glutShowWindow();
	glutPositionWindow(x, y);
	glutReshapeWindow(w, h);
	glViewport(0, 0, w, h);

}


void BotView::setWindowPerspective() {
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the model-view matrix

	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / (GLfloat)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 5000.0f);
	float startView[] = {-ViewEyeDistance,ViewEyeDistance, 0 };

	switch (view) {
	case TOP_VIEW:
		gluLookAt(startupFactor(startView[0],0), startupFactor(startView[1],ViewEyeDistance) ,startupFactor(startView[2],0),
				  0.0, 0.0, 0.0,
				  1.0, 0.0,	0.0);
		break;
	case FRONT_VIEW:
		gluLookAt(startupFactor(startView[0],0.0),startupFactor(startView[1],ViewBotHeight/2), startupFactor(startView[2],ViewEyeDistance) ,
				  0.0,startupFactor(0,ViewBotHeight/2), 0.0,
				  0.0, 1.0,	0.0);

		break;
	case RIGHT_VIEW:
		gluLookAt(startupFactor(startView[0],-ViewEyeDistance), startupFactor(startView[1],ViewBotHeight/2) ,startupFactor(startView[2],0.0),
				  0.0,startupFactor(0,ViewBotHeight/2), 0.0,
				  0.0, 1.0,0.0);
		break;
	case _3D_VIEW:
		gluLookAt(startupFactor(startView[0], eyePosition[0]),startupFactor(startView[1],eyePosition[1]),startupFactor(startView[2], eyePosition[2]),
				0.0, startupFactor(0,ViewBotHeight/2), 0.0,
				0.0, 1.0, 0.0);
		break;
	default:
		break;
	}
}

void BotView::setEyePosition(float* pEyePosition) {
	eyePosition[0] = pEyePosition[0];
	eyePosition[1] = pEyePosition[1];
	eyePosition[2] = pEyePosition[2];
}

void BotView::setEyePosition(float pCurrEyeDistance, float pBaseAngle, float pHeightAngle) {

	currEyeDistance = constrain(pCurrEyeDistance,ViewEyeDistance/3.0f,ViewEyeDistance*3.0f);
	baseAngle = pBaseAngle;
	heightAngle = constrain(pHeightAngle,-90.0f,45.0f);

	eyePosition[0] = currEyeDistance*( sinf(radians(baseAngle)) * cosf(radians(heightAngle)));
	eyePosition[1] = ViewBotHeight - currEyeDistance*sinf(radians(heightAngle));
	eyePosition[2] = currEyeDistance*(cosf(radians(baseAngle)) * cosf(radians(heightAngle)));
}

void BotView::changeEyePosition(float pCurrEyeDistance, float pBaseAngle, float pHeightAngle) {

	currEyeDistance += pCurrEyeDistance;
	baseAngle 		+= pBaseAngle;
	heightAngle 	+= pHeightAngle;

	setEyePosition(currEyeDistance, baseAngle, heightAngle);
}

void BotView::setAngles(JointAngleType pAngles) {
	angles = pAngles;
}

void BotView::hide() {
	glutSetWindow(windowHandle);
	glutHideWindow();
}

void BotView::show() {
	glutSetWindow(windowHandle);
	glutShowWindow();

}

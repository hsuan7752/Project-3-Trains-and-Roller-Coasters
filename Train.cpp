#include "Train.H"
#include "glad/glad.h"

Train::
Train(Pnt3f pos, Pnt3f orient, bool doingShadow)
{
	this->pos = pos;

	this->orient = orient;

	firstCar(doingShadow);

	vec.x = -9.0f;
	vec.y = 0.0f;
	vec.z = 0.0f;
}

Train::
Train(Pnt3f pos)
{
	this->pos = pos;
}

void Train::
add(bool doingShadow)
{
	next_car = new Train(Pnt3f(pos.x + 15, pos.y, pos.z));
	next_car->draw(doingShadow);
}

void Train::
del()
{
	
}

void Train::
draw(bool doingShadow)
{
	//left
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z - 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z - 3);
	glEnd();

	//right
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z + 3);
	glEnd();

	//front
	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z - 3);
	glEnd();

	//back
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z - 3);
	glEnd();

	//up
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(0.1f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z - 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z - 3);
	glEnd();

	//down
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(0.1f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z - 3);
	glEnd();

	//draw wheels
	/*glBegin();*/
}

void Train::
firstCar(bool doingShadow)
{
	//left
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z - 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z - 3);
	glEnd();

	//right
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z + 3);
	glEnd();

	//front
	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z - 3);
	glEnd();

	//back
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z - 3);
	glEnd();

	//up
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(0.1f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 7, pos.z - 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 7, pos.z - 3);
	glEnd();

	//down
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(0.1f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z + 3);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 1, pos.z - 3);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x - 9, pos.y + 1, pos.z - 3);
	glEnd();


	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + 3, pos.y + 6, pos.z + 4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 6, pos.z + 4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 12, pos.z + 4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + 3, pos.y + 12, pos.z + 4);
	glEnd();

	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + 3, pos.y + 6, pos.z - 4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 6, pos.z - 4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 12, pos.z - 4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + 3, pos.y + 12, pos.z - 4);
	glEnd();

	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + 3, pos.y + 6, pos.z + 4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 3, pos.y + 6, pos.z - 4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 3, pos.y + 12, pos.z - 4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + 3, pos.y + 12, pos.z + 4);
	glEnd();

	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + 9, pos.y + 6, pos.z + 4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 6, pos.z - 4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 12, pos.z - 4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + 9, pos.y + 12, pos.z + 4);
	glEnd();

	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + 9, pos.y + 12, pos.z + 4);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + 9, pos.y + 12, pos.z - 4);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + 3, pos.y + 12, pos.z - 4);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + 3, pos.y + 12, pos.z + 4);
	glEnd();
}

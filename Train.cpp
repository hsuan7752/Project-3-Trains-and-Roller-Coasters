#include "Train.H"
#include "glad/glad.h"
#include <math.h>
#include <iostream>
#include "TrainView.H"
#include "TrainWindow.H"

Train::
Train(Pnt3f pos, Pnt3f orient, float* vec, bool doingShadow)
{
	this->pos = pos;

	this->orient = orient;

	Pnt3f AC(vec[0], vec[1], vec[2]);
	Pnt3f AB(-9.0f, 0.0f, 0.0f);

	float cos_y = (float)(AC.x * AB.x + AC.y * AB.y + AC.z * AB.z)
		/ (abs(sqrt(pow(AC.x, 2) + pow(AC.y, 2) + pow(AC.z, 2))) * abs(sqrt(pow(AB.x, 2) + pow(AB.y, 2) + pow(AB.z, 2))));
	float sin_y = (float)sqrt(1 - pow(cos_y, 2));

	if ((AC.x < 0 && AC.z > 0) || (AC.x > 0 && AC.z > 0))
				sin_y = -sin_y;

	float rotationMatrix[3][3] = { { cos_y, 0, -sin_y },
										  { 0, 1, 0 },
										  { sin_y, 0, cos_y } };
	float* v0 = new float[3]{ -5, 7, +3 };
	float* v1 = new float[3]{ 5, 7, +3 };
	float* v2 = new float[3]{ 5, 7, -3 };
	float* v3 = new float[3]{ -5, 7, -3 };
	
	float* v4 = new float[3]{ -5, 1, +3 };
	float* v5 = new float[3]{ 5, 1, +3 };
	float* v6 = new float[3]{ 5, 1, -3 };
	float* v7 = new float[3]{ -5, 1, -3 };

	v0 = rotatef(rotationMatrix, v0);
	v1 = rotatef(rotationMatrix, v1);
	v2 = rotatef(rotationMatrix, v2);
	v3 = rotatef(rotationMatrix, v3);

	v4 = rotatef(rotationMatrix, v4);
	v5 = rotatef(rotationMatrix, v5);
	v6 = rotatef(rotationMatrix, v6);
	v7 = rotatef(rotationMatrix, v7);

	//left
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v7[0], pos.y + v7[1], pos.z + v7[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v6[0], pos.y + v6[1], pos.z + v6[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v2[0], pos.y + v2[1], pos.z + v2[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v3[0], pos.y + v3[1], pos.z + v3[2]);
	glEnd();

	//right
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v4[0], pos.y + v4[1], pos.z + v4[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v5[0], pos.y + v5[1], pos.z + v5[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v1[0], pos.y + v1[1], pos.z + v1[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v0[0], pos.y + v0[1], pos.z + v0[2]);
	glEnd();

	//front
	glBegin(GL_QUADS);
	if (!doingShadow)
		//glColor3ub(120, 100, 200);
		glColor3ub(255, 255, 255);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v7[0], pos.y + v7[1], pos.z + v7[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v4[0], pos.y + v4[1], pos.z + v4[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v0[0], pos.y + v0[1], pos.z + v0[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v3[0], pos.y + v3[1], pos.z + v3[2]);
	glEnd();

	//back
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v6[0], pos.y + v6[1], pos.z + v6[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v5[0], pos.y + v5[1], pos.z + v5[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v1[0], pos.y + v1[1], pos.z + v1[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v2[0], pos.y + v2[1], pos.z + v2[2]);
	glEnd();

	//up
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v0[0], pos.y + v0[1], pos.z + v0[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v1[0], pos.y + v1[1], pos.z + v1[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v2[0], pos.y + v2[1], pos.z + v2[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v3[0], pos.y + v3[1], pos.z + v3[2]);
	glEnd();

	//down
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v4[0], pos.y + v4[1], pos.z + v4[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v5[0], pos.y + v5[1], pos.z + v5[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v6[0], pos.y + v6[1], pos.z + v6[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v7[0], pos.y + v7[1], pos.z + v7[2]);
	glEnd();
}

Train::
Train(Pnt3f pos)
{
	this->pos = pos;
}

void Train::
add(Pnt3f pos, Pnt3f orient, float* vec, bool doingShadow)
{
	next_car = new Train(pos);
	next_car->draw(doingShadow, vec);
}

void Train::
del()
{
	
}

void Train::
draw(bool doingShadow, float* vec)
{
	Pnt3f AC(vec[0], vec[1], vec[2]);
	Pnt3f AB(-9.0f, 0.0f, 0.0f);

	float cos_y = (float)(AC.x * AB.x + AC.y * AB.y + AC.z * AB.z)
		/ (abs(sqrt(pow(AC.x, 2) + pow(AC.y, 2) + pow(AC.z, 2))) * abs(sqrt(pow(AB.x, 2) + pow(AB.y, 2) + pow(AB.z, 2))));
	float sin_y = (float)sqrt(1 - pow(cos_y, 2));

	if ((AC.x < 0 && AC.z > 0) || (AC.x > 0 && AC.z > 0))
		sin_y = -sin_y;

	float rotationMatrix[3][3] = { { cos_y, 0, -sin_y },
										  { 0, 1, 0 },
										  { sin_y, 0, cos_y } };
	float* v0 = new float[3]{ -5, 7, +3 };
	float* v1 = new float[3]{ 5, 7, +3 };
	float* v2 = new float[3]{ 5, 7, -3 };
	float* v3 = new float[3]{ -5, 7, -3 };

	float* v4 = new float[3]{ -5, 1, +3 };
	float* v5 = new float[3]{ 5, 1, +3 };
	float* v6 = new float[3]{ 5, 1, -3 };
	float* v7 = new float[3]{ -5, 1, -3 };

	v0 = rotatef(rotationMatrix, v0);
	v1 = rotatef(rotationMatrix, v1);
	v2 = rotatef(rotationMatrix, v2);
	v3 = rotatef(rotationMatrix, v3);

	v4 = rotatef(rotationMatrix, v4);
	v5 = rotatef(rotationMatrix, v5);
	v6 = rotatef(rotationMatrix, v6);
	v7 = rotatef(rotationMatrix, v7);

	//left
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v7[0], pos.y + v7[1], pos.z + v7[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v6[0], pos.y + v6[1], pos.z + v6[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v2[0], pos.y + v2[1], pos.z + v2[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v3[0], pos.y + v3[1], pos.z + v3[2]);
	glEnd();

	//right
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v4[0], pos.y + v4[1], pos.z + v4[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v5[0], pos.y + v5[1], pos.z + v5[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v1[0], pos.y + v1[1], pos.z + v1[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v0[0], pos.y + v0[1], pos.z + v0[2]);
	glEnd();

	//front
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v7[0], pos.y + v7[1], pos.z + v7[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v4[0], pos.y + v4[1], pos.z + v4[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v0[0], pos.y + v0[1], pos.z + v0[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v3[0], pos.y + v3[1], pos.z + v3[2]);
	glEnd();

	//back
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v6[0], pos.y + v6[1], pos.z + v6[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v5[0], pos.y + v5[1], pos.z + v5[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v1[0], pos.y + v1[1], pos.z + v1[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v2[0], pos.y + v2[1], pos.z + v2[2]);
	glEnd();

	//up
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v0[0], pos.y + v0[1], pos.z + v0[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v1[0], pos.y + v1[1], pos.z + v1[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v2[0], pos.y + v2[1], pos.z + v2[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v3[0], pos.y + v3[1], pos.z + v3[2]);
	glEnd();

	//down
	glBegin(GL_QUADS);
	if (!doingShadow)
		glColor3ub(120, 100, 200);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(pos.x + v4[0], pos.y + v4[1], pos.z + v4[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pos.x + v5[0], pos.y + v5[1], pos.z + v5[2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(pos.x + v6[0], pos.y + v6[1], pos.z + v6[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(pos.x + v7[0], pos.y + v7[1], pos.z + v7[2]);
	glEnd();
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

float* Train::
rotatef(float m[][3], float* p)
{
	float* n = new float[] {0, 0, 0};
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			n[i] += m[i][j] * p[j];

	return n;
}
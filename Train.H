#pragma once
#include "Utilities/Pnt3f.H"
class Train
{
public:
	Train(Pnt3f pos, Pnt3f orient, float* vec, bool doingShadow);

	Train(Pnt3f pos);

	void	add(bool doingShadow);

	void	del();

	void	draw(bool doingShadow);

	Pnt3f	pos;

	void	firstCar(bool doingShadow);

	float*	rotatef(float m[][3], float* p);

private:
	Train*	next_car = nullptr;	

	Pnt3f	orient;
};
#pragma once
#include "../../Common/Vector4.h"

using namespace NCL::Maths;
class Particles
{
	
public:
	Particles(Vector4 positions, Vector4 velocities);
	~Particles();

private:
	Vector4 positions;
	Vector4 velocities;


};


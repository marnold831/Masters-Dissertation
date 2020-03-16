#include "Particles.h"

Particles::Particles(int numParticles) : numParticles(numParticles)
{
	positions.resize(numParticles);

	for (int i = 0; i < numParticles; ++i) {
		positions[i] = Vector4();
	}

	velocities.resize(numParticles);
	for (int i = 0; i < numParticles; ++i) {
		velocities[i] = Vector4();
	}
}

Particles::~Particles()
{

}
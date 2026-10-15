#ifndef WHITTEDINTEGRATOR
#define WHITTEDINTEGRATOR

#include "shader.h"

// Implement a new class, called WhittedIntegrator, which inherits from the parent class Shader and
// computes the direct illumination given a set of Point Lights.

class Whittedintegrator : public Shader
{
public:
	// constructor 
	Whittedintegrator();

	Vector3D computeColor(const Ray& r,
		const std::vector<Shape*>& objList,
		const std::vector<LightSource*>& lsList) const;

private:
};

#endif // WHITTEDINTEGRATOR#pragma once

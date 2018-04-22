#pragma once


#include "Core/Basic/BasicTypes.h"

#include <vector>


class PerlinNoise {

	// The permutation vector
	std::vector<int> p;
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise();
	// Generate a new permutation vector based on the value of seed
	PerlinNoise(uint32 seed);
	// Get a noise value, for 2D images z can have any value
	float noise(float x, float y, float z);
private:
	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
};




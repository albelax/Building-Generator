#ifndef ROOF_H
#define ROOF_H

#include <vector>
#include <gtest/gtest.h>

#include "Object.h"
#include "Corner.h"
#include "Walls.h"

#define TESTING 0

#if TESTING
#include <gtest/gtest.h>
#endif // TESTING

/// \file Roof.h
/// \brief this is the class that makes The roof, it uses the matrices of the walls to fill the gap and make a roof
/// \author Alberto La Scala

class Roof : public Object
{
public:

	/// \brief Roof creates an empty roof
	Roof();

	/// \brief Roof creates a roof using the transformation matrices of the walls to load the edge as a separate mesh
	/// then it interpolates the matrices to fill the roof with simpler geometry
	Roof(Object &_walls, Object &_corners);

private:

	/// \brief round I know that comparing flaots is dodgy, but I really needed it, this function rounds up the floats to multiples of 0.5f
	static float round(float const _in);

	/// \brief sortEdges it's an utility function that sorts the edges by Z and X,
	///  it makes my job easier when it comes down to filling the gap between the walls
	void sortEdges();

	/// \brief fill fills the gap between the walls
	void fill();

	/// \brief isNearlyEqual was a good alternative to the Round function, this could have avoided the floating comparison
	/// but in the end comparing floats although is less correct resulted slightly easier and readable
	bool isNearlyEqual(float const a, float const b, float const error_margin = 0.0003f) const;

	/// \brief removeMatrices cleans up the extra matrices that may be in place,
	/// the ones beyond the walls or the ones on top of the corners
	void removeMatrices(Object &_object);

#if TESTING
	FRIEND_TEST(Round, first);
#endif // TESTING

};
#endif // ROOF_H

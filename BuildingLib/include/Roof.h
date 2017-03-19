#ifndef ROOF_H
#define ROOF_H

#include <vector>
#include <gtest/gtest.h>

#include "Object.h"
#include "Corner.h"
#include "Walls.h"

#define TESTING 1

class Roof : public Object
{
public:
	Roof();
	Roof(Object &_walls, Object &_corners);
private:
	void makeTranslationTable() override;
	static float round(float const _in);
	void sortEdges();
	void fill();
	bool isNearlyEqual(float const a, float const b, float const error_margin = 0.0003f) const;
	void removeMatrices(Object &_object);

#if TESTING
	FRIEND_TEST(Round, first);
#endif // TESTING

};
#endif // ROOF_H

#ifndef ROOF_H
#define ROOF_H

#include "Object.h"
#include "Corner.h"
#include "Walls.h"
#include <vector>

class Roof : public Object
{
public:
	Roof();
	Roof(Object &_walls, Object &_corners);
private:
	void makeTranslationTable() override;
	float round(float const _in) const;
	void sortEdges();
	void fill();
	bool isNearlyEqual(float const a, float const b, float const error_margin = 0.0003f) const;
	void removeMatrices(Object &_object);
};
#endif // ROOF_H

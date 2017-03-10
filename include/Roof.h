#ifndef ROOF_H
#define ROOF_H

#include "Object.h"
#include "Walls.h"
#include <vector>

class Roof : public Object
{
public:
	Roof();
	Roof(Object _walls, Object _corners);
private:
	void makeTranslationTable() override;
	float round(float _in);
	void sortEdges();
	void fill();
};
#endif // ROOF_H

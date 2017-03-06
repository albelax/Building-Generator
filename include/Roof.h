#ifndef ROOF_H
#define ROOF_H

#include "Object.h"
#include "Walls.h"
#include <vector>

class Roof : public Object
{
public:
	Roof();
	Roof(Walls _base);
private:
	void makeTranslationTable() override;
};
#endif // ROOF_H

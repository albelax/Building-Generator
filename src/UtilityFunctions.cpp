#include "UtilityFunctions.h"

std::vector<std::string> utilityFunctions::split(std::string _stringIn, char _splitChar)
{
	int count = 0;
	std::string tempString;
	std::vector<std::string> retVector;
	for (int i = 0; i < static_cast<int>(_stringIn.length()+1); i++)
	{
		if(tempString.length() < 1 && _stringIn[i] == ' ') continue;
		else if(_stringIn[i] != _splitChar)
		{
			tempString += _stringIn[i];
		}
		else
		{
			retVector.resize(retVector.size() + 1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}

		if(i == static_cast<int>(_stringIn.length()))
		{
			retVector.resize(retVector.size()+1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}
	}
	return retVector;
}
//--------------------------------------------------------------------------------------------------------------

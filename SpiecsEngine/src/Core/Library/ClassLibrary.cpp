/**
* @file ClassLibrary.cpp.
* @brief The ClassLibrary Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ClassLibrary.h"
#include "StringLibrary.h"

namespace Spiecs {

	std::string ClassLibrary::GetClassString(ClassType t)
	{
		/**
		* @brief Split Class Name to Namespcae and Class.
		*/
		const std::vector<std::string>& outSplit = StringLibrary::SplitString(t.get().name(), ':');

		/**
		* @brief Return Class Name.
		*/
		return outSplit[outSplit.size() - 1];
	}
	
}
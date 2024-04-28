#include "Pchheader.h"
#include "ClassLibrary.h"
#include "StringLibrary.h"

namespace Spiecs {

	std::string ClassLibrary::GetClassString(ClassType t)
	{
		const std::vector<std::string>& outSplit = StringLibrary::SplitString(t.get().name(), ':');
		return outSplit[outSplit.size() - 1];
	}
}
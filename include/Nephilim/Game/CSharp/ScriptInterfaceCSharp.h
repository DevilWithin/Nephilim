#ifndef NephilimScriptInterfaceCSharp_h__
#define NephilimScriptInterfaceCSharp_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Game/BaseScriptInterface.h>

NEPHILIM_NS_BEGIN

/**
	\class ScriptInterfaceCSharp
	\brief Interface between C# and a BaseGame instance

	This class acts as the bridge that connects a BaseGame with all its
	functionality to a ready scripting solution.

	The main duties of this class are:
	1) Implement a methodology for the scripts organization and workflow
	2) Expose to these scripts access to manipulate the BaseGame and its structures
	3) Call entry points on the scripts when the BaseGame requests it
*/
class NEPHILIM_API ScriptInterfaceCSharp
{
public:

};

NEPHILIM_NS_END
#endif // NephilimScriptInterfaceCSharp_h__
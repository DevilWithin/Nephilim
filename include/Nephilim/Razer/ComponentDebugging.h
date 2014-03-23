#ifndef NephilimRazerComponentDebugging_h__
#define NephilimRazerComponentDebugging_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Razer/Component.h>

NEPHILIM_NS_BEGIN
namespace rzr
{

/**
	\class ComponentTransform
	\brief 3D transform for a entity, scale + rotation + translation
*/
class NEPHILIM_API ComponentDebugging : public Component
{
public:
	float x,y,z; /// Where to draw the box
};

};
NEPHILIM_NS_END
#endif // NephilimRazerComponentDebugging_h__

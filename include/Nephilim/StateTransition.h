#ifndef NephilimStateTransition_h__
#define NephilimStateTransition_h__

#include "Platform.h"

#include <vector>

NEPHILIM_NS_BEGIN

class StateStack;
class Time;
class Renderer;
class State;
class GameCore;

/**
	\ingroup Core
	\class StateTransition
	\brief Performs a transition animation between two states

	Used together with StateStack and State classes
*/
class NEPHILIM_API StateTransition
{
public:
	/// Base construction
	StateTransition();

	/// Virtual destructor
	virtual ~StateTransition();

	/// Set the duration of the transition effect
	void setDuration(float duration);

	void drawPreviousFrame(Renderer* renderer);
	void drawNextFrame(Renderer* renderer);
	void finish();

	virtual void activate()
	{

	}

	/// Virtual method for updating the transition
	virtual void update(const Time& time);

	virtual void draw(Renderer* renderer)
	{

	}

	bool allowUpdatesA;
	bool allowUpdatesB;

private:
	friend class StateStack;
	StateStack* m_stack;

	std::vector<State*> m_futureList;
	bool m_finished;

	float m_duration; ///< The total duration of the effect
};

NEPHILIM_NS_END
#endif // NephilimStateTransition_h__
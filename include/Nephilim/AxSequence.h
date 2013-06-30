#ifndef NephilimAxSequence_h__
#define NephilimAxSequence_h__

#include "Platform.h"
#include "AxBase.h"

#include <list>

NEPHILIM_NS_BEGIN

class NEPHILIM_API AxSequence : public AxBase
{
public:
	/// Sequence mode types
	enum SequenceModes
	{
		Ordered,
		Parallel
	};

	/// Initializes in ordered mode by default
	AxSequence();

	/// Initialize with a sequence mode
	AxSequence(SequenceModes mode);

	/// Initialize with a sequence mode and 0 to 3 initial animations
	AxSequence(SequenceModes mode, AxBase *anim1 = NULL, AxBase *anim2 = NULL, AxBase* anim3 = NULL);

	/// Destroy all animations
	~AxSequence();

	/// Attempts to deduce initial animation parameters by looking at the targets
	virtual void deduceInitialParameters();

	/// Add a new target for animation
	virtual void addTarget(AxTarget* target);

	/// Returns true when the sequence is finished
	virtual bool isOver();

	/// Add an animation to the sequence
	void add(AxBase* animation);

	/// Updates the sequence appropriately
	virtual float update(float delta);

	/// Get the duration of the animation
	virtual float getDuration();

	typedef std::list<AxBase*>::iterator AnimationIterator;
	std::list<AxBase*> m_animations;
	SequenceModes m_mode;
	size_t m_current;
};

NEPHILIM_NS_END
#endif // NephilimAxSequence_h__
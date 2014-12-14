#ifndef NephilimStateStack_h__
#define NephilimStateStack_h__

#include <Nephilim/Platform.h>

#include <vector>
#include <map>

NEPHILIM_NS_BEGIN

class String;
class Time;
class GameState;
class StateStackTransition;
class GraphicsDevice;
class Event;
class GameCore;

/**
	\class GameStateBatch
	\brief Configuration of the stack state, for transitioning

	By configuring a state batch structure and pushing it to the stack,
	it will activate the exact states ordered and do this with an animated transition if needed.
*/
class GameStateBatch
{
public:

	struct StateUnit
	{
		bool       nameBased; ///< otherwise its object based
		GameState* object;    ///<
		String     name;      ///<
	};

	std::vector<StateUnit> orderedGameStates;

public:

	/// Push a state by name instead of object
	void push(const String& state)
	{
		StateUnit unit;
		unit.nameBased = true;
		unit.name = state;
		orderedGameStates.push_back(unit);
	}
};

/**
	\ingroup Core
	\class StateStack
	\brief Mechanism to control game states like the pause screen, main menu, etc

	This tool uses a delayed action system. In other words, it does not necessarily apply changes immediately.

	Whenever you manipulate a StateStack object from a regular place, ie, not inside a State object execution,
	you must always call process() to push the pending changes and refresh the state. In the other hand,
	when the stack is being manipulated internally by State*, process() is completely irrelevant and other rules apply.

	State - bool active - when a state is about to be added to execution, it becomes active, and goes inactive once it finishes
*/
class NEPHILIM_API StateStack
{
public:
	/// Reference to the GameCore instance that owns this stack
	GameCore* mGame = nullptr;


public:
	/// Creates an empty state machine
	StateStack();

	/// Safely destroys all states by sending them a destroy callback
	~StateStack();

	/// Processes all the state changes that are pending
	/// Can never be called while the stack is working(rendering,updating or fetching events)
	void process();

	/// Lock the stack to prevent further changes
	/// It will be automatically unlocked when the next update() ends which has no transition active and all pending operations were
	/// commited. This means the stack is ready for being altered again
	/// When you call lock, that means you are done with your stack operations, and you can only do more when the stack is all ready
	void lock();

	/// Check if the stack is currently locked
	bool isLocked();

	/// Push a state batch down the stack
	void triggerBatch(const GameStateBatch& batch);

	/// Clear all active states
	void clear();

	/// Triggers a transition animation at any time
	/// If there are no pending changes in the state stack, the transition happens but is useless.
	/// Note: All transitions are deallocated when they finish. You must allocate it dynamically using new.
	/// Note: Passing NULL will mean a immediate transition
	/// Note: If a transition is passed while another is in effect, the older is deallocated and the new one is used.
	void performTransition(StateStackTransition* transition);

	/// Finishes the transition animation, deletes the transition
	/// and then commits all needed changes for regular functioning
	void endTransition();

	/// Check if a given state is currently being executed
	bool isActive(const String& name);

	/// Check if a given state is currently being executed
	bool isActive(GameState* state);
	
	void drawList(std::vector<GameState*>& list, GraphicsDevice* renderer);

	void drawCurrentList(GraphicsDevice* renderer);


	/// Push a state for execution directly
	void add(GameState* state);

	/// Push a binded state for execution by its name
	void add(const String& name);

	/// Remove the top state from the stack
	void pop();

	/// Updates the right states 
	void update(Time &time);

	/// Perform a update on the list
	void updateList(std::vector<GameState*>& list, const Time& time);

	/// Delivers the event to the right events
	void pushEvent(Event &event);

	/// Draws the right states
	void drawStates(GraphicsDevice *renderer);

	int getActiveStateCount();

	/// Bind a new state to the list
	/// Returns false if the name was already taken
	bool bind(const String& name, GameState* state);

	/// This function will apply the pending operations to the state stack
	void applyChangesTo(std::vector<GameState*>& list);

	/// Erase a state
	void erase(GameState* state);

	/// Get the parent game, if any
	GameCore* getParentGame();

	/// Get a state or NULL
	GameState* getBinding(const String& name);

	bool isTransitionActive(); 


	struct StateStackOperation
	{
		enum Type
		{
			Erase = 0, ///< Erases a given state instance from the list
			Add,       ///< Adds a given state instance to the list
			Pop,       ///< Remove the top state instance from the list
		};

		Type type;
		GameState* obj;
	};

//private:

	/// This container represents an actual stack of states on top of each other
	typedef std::vector<GameState*> GameStateList;

	GameStateList mCurrentList; ///< The currently active stack
	GameStateList mFutureList;  ///< The stack that will be active after the transition

	friend class StateStackTransition;

	std::map<String, GameState*> m_bindList; ///< The list of binded states, that "live" under the stack

	std::vector<StateStackOperation> m_pendingOperations; ///< The list of pending operations

	StateStackTransition* m_transition; ///< The active transition

	bool batchPending;
	GameStateBatch nextBatch;

	bool m_stackLock;

	/// If the stack is locked, all operations fail automatically
	/// Normally, the stack is unlocked automatically once the transitions end
	bool mLocked;
};

NEPHILIM_NS_END
#endif // NephilimStateStack_h__
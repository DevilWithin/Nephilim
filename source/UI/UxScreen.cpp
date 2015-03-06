#include <Nephilim/UI/UxScreen.h>
#include <Nephilim/UI/UxEvent.h>
#include <Nephilim/UI/UIView.h>
#include <Nephilim/UI/UICanvas.h>

#include <Nephilim/Graphics/Window.h>

#include <Nephilim/Foundation/Logging.h>

NEPHILIM_NS_BEGIN

/// Get the object name of this node
const char* UxScreen::getName()
{
	return "ScreenRoot";
}

/// Get the class name of this node
const char* UxScreen::getClassName()
{
	return "UxScreen";
}

/// Dispatch an event down the hierarchy
void UxScreen::dispatch(const UxEvent& event)
{
	for (auto node : _children)
	{
		// For now all children are expected to be nodes
		if (dynamic_cast<UICanvas*>(node))
		{
			UICanvas* view = static_cast<UICanvas*>(node);
			view->pushEvent(event._event);
			//	Log("Canvas being drawn");
		}
	}
}

/// Take a lower level event directly and translate it into a UxEvent for dispatch()
void UxScreen::translateAndDispatch(const Event& windowEvent)
{

}

/// Render the entire screen contents
void UxScreen::render(UxRenderState renderState)
{
	GDI->setClearColor(clearColor);
	//GDI->clearColorBuffer();
	GDI->setDefaultViewport();
	GDI->setProjectionMatrix(View(0, 0, window->width(), window->height()).getMatrix());


	for (auto node : _children)
	{
		// For now all children are expected to be nodes
		if (dynamic_cast<UICanvas*>(node))
		{
			UICanvas* view = static_cast<UICanvas*>(node);
			view->draw(GDI);
		//	Log("Canvas being drawn");
		}
		// For now all children are expected to be nodes
		if (dynamic_cast<UIView*>(node))
		{
			UIView* view = static_cast<UIView*>(node);
			view->drawItself(GDI, mat4::identity);
		}
	}
}

/// This is the primary call, that makes the entire hierarchy draw
void UxScreen::renderScreen()
{
	UxRenderState renderState;
	render(renderState);
}

NEPHILIM_NS_END
#ifndef GenericMainAndroid_h__
#define GenericMainAndroid_h__

#include <Nephilim/Platform.h>

// -- Assumes a _engine global variable declared previously to communicate with

#if defined NEPHILIM_ANDROID

#include <Nephilim/Foundation/AndroidWrapper.h>

#if !defined ANDROID_PACKAGE_NAME || !defined ANDROID_ACTIVITY_NAME
#error Cannot build for Android platform without specifying the ANDROID_PACKAGE_NAME and ANDROID_ACTIVITY_CLASS
#endif

#include <Nephilim/Helpers/AndroidGlue.h>
#include <android/keycodes.h>

Engine sdk;

void android_init(){

	static bool alreadyStarted = false;

	if(!alreadyStarted){
		_engine = &sdk;
#if defined NEPHILIM_GLES1
		_engine->glesHint = 1;
#else
		_engine->glesHint = 2;
#endif

		init();

		alreadyStarted = true;
	}
	else{

	}


}

void android_gl_reload()
{
	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::LostGraphicsContext;
	_engine->injectEvent(ev);
}

void android_render(){
	update();
}

void android_resize(int w, int h){
	_engine = &sdk; // ensure
	_engine->m_window->setSize(w, h);

	/*
	pE::Event ev;
	ev.type = pE::Event::Resized;
	ev.size.width = w;
	ev.size.height = h;
	myApp.pendingEvents.push_back(ev);*/
}

void android_keydown(int key){
//	NEPHILIM_NS::String sd = "Keypressed: " + pE::String::number(key);

	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::KeyPressed;

	switch(key){
	case AKEYCODE_A: ev.key.code = NEPHILIM_NS::Keyboard::A;break;
	case AKEYCODE_S: ev.key.code = NEPHILIM_NS::Keyboard::S;break;
	case AKEYCODE_D: ev.key.code = NEPHILIM_NS::Keyboard::D;break;
	case AKEYCODE_W: ev.key.code = NEPHILIM_NS::Keyboard::W;break;
	case AKEYCODE_BACK: ev.key.code = NEPHILIM_NS::Keyboard::AndroidBack;break;
	}

	_engine->injectEvent(ev);
}


// -- Multi touch down
void android_multitouchdown(float x, float y, int index, int id)
{
	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::TouchPressed;
	ev.touch.x = x;
	ev.touch.y = y;
	ev.touch.index = index;
	ev.touch.id = id;
	_engine->injectEvent(ev);
}
// -- Multi touch up
void android_multitouchup(float x, float y, int index, int id)
{
	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::TouchReleased;
	ev.touch.x = x;
	ev.touch.y = y;
	ev.touch.index = index;
	ev.touch.id = id;
	_engine->injectEvent(ev);
}
// -- Multi touch move
void android_multitouchmove(float x, float y, int index, int id)
{
	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::TouchMoved;
	ev.touch.x = x;
	ev.touch.y = y;
	ev.touch.index = index;
	ev.touch.id = id;
	_engine->injectEvent(ev);
}



void android_touchdown(float x, float y){
	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::TouchPressed;
	ev.touch.x = x;
	ev.touch.y = y;
	_engine->injectEvent(ev);
}


void android_touchup(float x, float y){
	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::TouchReleased;
	ev.touch.x = x;
	ev.touch.y = y;
	_engine->injectEvent(ev);
}

void android_touchmove(float x, float y){
	NEPHILIM_NS::Event ev;
	ev.type = NEPHILIM_NS::Event::TouchMoved;
	ev.touch.x = x;
	ev.touch.y = y;
	_engine->injectEvent(ev);
}



#endif
#endif // GenericMainAndroid_h__

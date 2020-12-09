#pragma once

#include "hps.h"
#include "dprintf.h"
#include <cassert>

#define APP_ACTION

// MobileApp is a plaform-independent class which users can modify to
//  store application data or actions. This class (along with UserMobileSurface)
//  allow Android/iOS gui code to communicate with C++ mobile independent code.
//
// For Android users: refer to UserMobileSurface.h for declaring APP_ACTION methods
//

// Class to handle errors
class MyErrorHandler: public HPS::EventHandler
{
public:
	MyErrorHandler() : HPS::EventHandler() {}
	virtual ~MyErrorHandler() {}

	// Override to provide behavior for an error event
	virtual HandleResult Handle(HPS::Event const * in_event)
	{
		assert(in_event!=NULL);
		HPS::ErrorEvent const * error = static_cast<HPS::ErrorEvent const *>(in_event);
		const char* msg = error->message.GetBytes();
		if(msg)
			eprintf("%s\n", msg);
		else
			eprintf("An error occurred but there was no specific message regarding this event");
		return HandleResult::Handled;
	}
};

// Class to handle warnings
class MyWarningHandler: public HPS::EventHandler
{
public:
	MyWarningHandler() : HPS::EventHandler() {}
	virtual ~MyWarningHandler() {}

	// Override to provide behavior for a warning event
	virtual HandleResult Handle(HPS::Event const * in_event)
	{
		assert(in_event!=NULL);
		HPS::WarningEvent const * warning = static_cast<HPS::WarningEvent const *>(in_event);
		const char* msg = warning->message.GetBytes();
		if(msg)
			wprintf("%s\n", msg);
		else
			wprintf("A warning occurred but there was no specific message regarding this event");
		return HandleResult::Handled;
	}

};

class MobileApp
{
public:
	static MobileApp & inst() {
		static MobileApp instance;
		return instance;
	}

	APP_ACTION void		shutdown(); // any calls made to hps after this point are invalid
	APP_ACTION void		setLibraryDirectory(const char *libraryDir);
	APP_ACTION void		setFontDirectory(const char *fontDir);
	APP_ACTION void		setMaterialsDirectory(const char *materialsDir);

private:
	MobileApp();
	MobileApp(MobileApp const &);		// Singleton - do not implement
	void operator=(MobileApp const &);	// Singleton - do not implement

	// Single HPS::World instance
	HPS::World *			_world;
	MyErrorHandler			_errorHandler;
	MyWarningHandler		_warningHandler;

};



#include "MobileApp.h"
#include "dprintf.h"

#include "hoops_license.h"

MobileApp::MobileApp()
	: _world(0)
{
	//TODO - CREATE HPS WORLD
	_world = new HPS::World(HOOPS_LICENSE);

	// Subscribe _errorHandler to handle errors
	HPS::Database::GetEventDispatcher().Subscribe(_errorHandler, HPS::Object::ClassID<HPS::ErrorEvent>());

	// Subscribe _warningHandler to handle warnings
	HPS::Database::GetEventDispatcher().Subscribe(_warningHandler, HPS::Object::ClassID<HPS::WarningEvent>());
}

void MobileApp::shutdown()
{
    delete _world;
}

void MobileApp::setFontDirectory(const char* fontDir)
{
	_world->SetFontDirectory(fontDir);
}

void MobileApp::setLibraryDirectory(const char *libraryDir)
{
	_world->SetExchangeLibraryDirectory(libraryDir);
}

void MobileApp::setMaterialsDirectory(const char* materialsDir)
{
	_world->SetMaterialLibraryDirectory(materialsDir);
}

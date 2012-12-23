#include "firstlight.h"

namespace flt
{

namespace core
{
	const matrix4 IdentityMatrix;
}

namespace renderer
{
	const Color ColorWhite(255,255,255,255);
	const Color ColorBlack(255,0,0,0);
	const Color ColorRed(255,255,0,0);
	const Color ColorGreen(255,0,255,0);
	const Color ColorBlue(255,0,0,255);
}

void initEngine()
{
	utils::Logger::newInstance();
	utils::Logger::getInstance().enableGroup("engine",true);
	LOG("engine","Logger created.");
	
	EventManager::newInstance();
	LOG("engine","EventManager created.");
	
	ArchiveManager::newInstance();
	LOG("engine","ArchiveManager created.");

	renderer::TextureManager::newInstance();
	LOG("engine","TextureManager created.");

	renderer::MaterialManager::newInstance();
	LOG("engine","MaterialManager created.");

	renderer::MaterialFactory::newInstance();
	LOG("engine","MaterialFactory created.");

	XSpriteManager::newInstance();
	LOG("engine","XSpriteManager created.");

	XFontManager::newInstance();
	LOG("engine","XFontManager created.");

	anim::AnimSetManager::newInstance();
	LOG("engine","AnimSetManager created.");

	ui::UIManager::newInstance();
	LOG("engine","UIManager created.");

	sound::initSoundSystem();
	LOG("engine","Sound System inited.");
}

void uninitEngine()
{
	sound::shutDownSoundSystem();
	LOG("engine","Sound System shut down.");

	ui::UIManager::deleteInstance();
	LOG("engine","UIManager shut down.");

	anim::AnimSetManager::deleteInstance();
	LOG("engine","AnimSetManager shut down.");

	XFontManager::deleteInstance();
	LOG("engine","XFontManager shut down.");

	XSpriteManager::deleteInstance();
	LOG("engine","XSpriteManager shut down.");

	renderer::MaterialFactory::deleteInstance();
	LOG("engine","MaterialFactory shut down.");

	renderer::MaterialManager::deleteInstance();
	LOG("engine","MaterialManager shut down.");

	renderer::TextureManager::deleteInstance();
	LOG("engine","TextureManager shut down.");

	ArchiveManager::deleteInstance();
	LOG("engine","ArchiveManager shut down.");

	EventManager::deleteInstance();
	LOG("engine","EventManager shut down.");

	LOG("engine","Logger shut down.");
	utils::Logger::deleteInstance();
}

Engine::Engine()
{
	initEngine();
}

Engine::~Engine()
{
	uninitEngine();
}

}// end namespace flt
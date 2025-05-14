#include "cMenu.h"

#include "cApp.h"

/**
 * @file cMenu.cpp
 *
 * @brief Contains menu class implementation
 *
 * This file implements menu class for menu window management.
**/

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CONSTRUCTORS & DESTRUCTOR ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief Default constructor
cMenu::cMenu()
{
	InitMenu();
}

/// @brief Destructor
cMenu::~cMenu()
{
	ExitMenu();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// MENU MANAGEMENT //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/// @brief Initialize properties of option for menu management
/// @return Always return true by default
bool cMenu::InitMenu()
{
	using namespace app_const;
	nOption = INIT_MENU_OPTION;
	nOptionLimit = static_cast<int>(std::size(MENU_OPTIONS));
	sOptionLabels = { MENU_OPTIONS, MENU_OPTIONS + nOptionLimit };
	return true;
}
/// @brief Free memory of option when exit
/// @return Always return true by default
bool cMenu::ExitMenu()
{
	sOptionLabels.clear();
	return true;
}
/// @brief Load option and draw it on screen
/// @param App Pointer to application
/// @return Always return true by default
bool cMenu::LoadOption(cApp* App)
{
	nOption = (nOption % nOptionLimit + nOptionLimit) % nOptionLimit;
	CloseMenu(App);

	switch (nOption) {
		case 0:
			eAppOption = cMenu::Option::NEW_GAME;
			App->GameReset();
			break;
		case 1:
			eAppOption = cMenu::Option::CONTINUE;
			App->GameReset();
			App->OnGameLoad();
			break;
		case 2:
			eAppOption = cMenu::Option::SETTINGS;
			break;
		case 3:
			eAppOption = cMenu::Option::ABOUT_US;
			break;
		case 4:
			eAppOption = cMenu::Option::EXIT_APP;
			break;
		default:
			break;
	}

	OpenMenu(App);
	return true;
}

/// @brief Display menu on screen
/// @param App Pointer to application
/// @return Always return true by default
bool cMenu::DisplayMenu(cApp* App)
{
	nOption = (nOption % nOptionLimit + nOptionLimit) % nOptionLimit;

	App->Clear(app::BLACK);
	App->DrawSprite(0, 0, cAssetManager::GetInstance().GetSprite("menu_background"));
	for (int id = 0; id < nOptionLimit; id++) {
		const std::string optionName = std::string(sOptionLabels[id]) + (id == nOption ? "_chosen" : "");
		const app::Sprite* optionSprite = cAssetManager::GetInstance().GetSprite(optionName);
		App->SetPixelMode(app::Pixel::MASK);
		App->DrawSprite(146, 65 + id * 10, optionSprite);
		if (id == nOption) {
			App->SetPixelMode(app::Pixel::NORMAL);
		}
	}
	return true;
}

/// @brief Open and display menu on screen
/// @param App Pointer to application
/// @return Always return true by default
bool cMenu::OpenMenu(cApp* App)
{
	DisplayMenu(App);
	return true;
}

/// @brief Update menu on screen when user press key
/// @param App Pointer to application
/// @return Always return true by default
bool cMenu::UpdateMenu(cApp* App)
{
	if (App->GetKey(app::Key::UP) == Button::RELEASED) {
		--nOption;
		DisplayMenu(App);
	}
	if (App->GetKey(app::Key::DOWN) == Button::RELEASED) {
		++nOption;
		DisplayMenu(App);
	}
	if (App->GetKey(app::Key::ENTER) == Button::RELEASED) {
		LoadOption(App);
	}
	return true;
}

/// @brief Close menu on screen
/// @param App Pointer to application
/// @return Always return true by default
bool cMenu::CloseMenu(cApp* App)
{
	return true;
}
/// @brief Display settings on screen (sound on/off)
/// @param App Pointer to application
/// @return Always return true by default
bool cMenu::DisplaySettings(cApp* App) const
{
	App->Clear(app::BLACK);
	App->DrawSprite(0, 0, cAssetManager::GetInstance().GetSprite("menu_background"));

	if (isMusicPlaying) {
		App->DrawSprite(146, 65, cAssetManager::GetInstance().GetSprite("sound_on"));
	}
	else {
		App->DrawSprite(146, 65, cAssetManager::GetInstance().GetSprite("sound_off"));
	}
	return true;
}

/// @brief Check if music is playing or not, then turn on/off music (until now)
/// @param App Pointer to application
/// @return Always return true by default
bool cMenu::UpdateSettings(cApp* App)
{
	if (isMusicPlaying) {
		app_sound::StopMusic();
		isMusicPlaying = false;
	}
	else {
		app_sound::PlayMusic("ncs0");
		isMusicPlaying = true;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// END OF FILE ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
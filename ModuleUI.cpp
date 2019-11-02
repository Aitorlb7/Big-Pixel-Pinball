#include "ModuleUI.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	printFont = { 0,0, 300 ,22 };
	Fontrect = { 0,0, 16 ,22 };
}
ModuleUI::~ModuleUI()
{

}
bool ModuleUI::Start()
{
	LOG("Loading UI textures");
	graphics = App->textures->Load("Textures/Assets_pinball.png");
	font_start = App->fonts->Load("Textures/Fonts.png", "0123456789", Fontrect, 1);

	return true;
}
update_status ModuleUI::Update()
{
	App->fonts->BlitText(0, 0, font_start, "09876", printFont);	
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	if (graphics != nullptr) { 
		SDL_DestroyTexture(graphics);
		graphics = nullptr;
	}


	return true;
}
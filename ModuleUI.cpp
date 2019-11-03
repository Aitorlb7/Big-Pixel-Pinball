#include "ModuleUI.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"
#include <string>

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}
ModuleUI::~ModuleUI()
{

}
bool ModuleUI::Start()
{
	LOG("Loading UI textures");
	graphics = App->textures->Load("Textures/Assets_pinball.png");
	font_start = App->fonts->Load("Textures/Font.png", "0123456789", 1);

	return true;
}
update_status ModuleUI::Update()
{
	score = 1234;
	App->renderer->DrawQuad({ 0, 0, 630, 40 }, 0, 0, 0, 255, true, true);
	App->fonts->BlitText(0, 0, 0, std::to_string(score).c_str());
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
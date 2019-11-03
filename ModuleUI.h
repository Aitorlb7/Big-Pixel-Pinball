#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "ModuleTextures.h"
#include "Globals.h"
#include "ModuleRender.h"

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();
	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Rect printFont;
	SDL_Rect Fontrect;

	int font_start = -1;

	int score = 0;
	uint score_text = 0000000;

	SDL_Texture* graphics = nullptr;

};
#endif

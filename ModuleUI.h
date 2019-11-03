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
	SDL_Rect Banner;
	SDL_Rect Scorerect1;
	SDL_Rect Scorerect2;
	SDL_Rect Inforect;

	SDL_Rect _100Rect;

	int font_start = -1;

	int score = 0;
	int high_score = 0;
	int prev_score = 0;
	int num_balls = 3;

	int y;

	SDL_Texture* graphics = nullptr;

};
#endif

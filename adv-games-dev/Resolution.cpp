#include "../include/SDL.h"
#include "../include/SDL_ttf.h"
#include "../include/SDL_image.h"
#include <iostream>

const int RESO_MENU = 4;

class Resolution
{

	void updateSelection(bool selected[], SDL_Texture* menu[], SDL_Surface* temp[], SDL_Renderer* gRenderer, TTF_Font* font, const char* labels[], SDL_Color TextColour[], int opt_index)
	{
		for (int j = 1; j < RESO_MENU; j++)
		{
			selected[j] = false;
		}

		selected[opt_index] = true;

		for (int i = 1; i < RESO_MENU; i++)
		{
			if (!selected[i])
			{
				SDL_DestroyTexture(menu[i]);
				temp[i] = TTF_RenderText_Solid(font, labels[i], TextColour[0]);
				menu[i] = SDL_CreateTextureFromSurface(gRenderer, temp[i]);
			}

			if (selected[i])
			{
				SDL_DestroyTexture(menu[i]);
				temp[i] = TTF_RenderText_Solid(font, labels[i], TextColour[1]);
				menu[i] = SDL_CreateTextureFromSurface(gRenderer, temp[i]);
			}
		}
	}

public:

	int showresolution(SDL_Surface *screen, SDL_Event e, SDL_Window *win)
	{
		bool keyboard = true;
		bool resRun = true;

		Uint32 time;
		int x, y;
		const char *labels[RESO_MENU] = { "RESOLUTION","800X600","1200X800","Back" };
		bool selected[RESO_MENU] = { 0,0,0,0 };
		SDL_Texture* background;
		SDL_Surface* background_surface;
		SDL_Texture * menu[RESO_MENU];
		SDL_Rect POS[RESO_MENU];
		SDL_Surface * temp[RESO_MENU];
		SDL_Color TextColour[2] = { { 150, 0, 0 },{ 200, 200, 200 } };

		SDL_Renderer * gRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

		TTF_Font *font = TTF_OpenFont("impact.ttf", 80);
		if (font == NULL)
		{
			printf("Font not found");
		}


		background_surface = IMG_Load("background.png");
		background = SDL_CreateTextureFromSurface(gRenderer, background_surface);


		for (int i = 0; i < RESO_MENU; i++)
		{
			temp[i] = TTF_RenderText_Solid(font, labels[i], TextColour[0]);
		}
		int w;
		int h;

		SDL_GetWindowSize(win, &w, &h);


		POS[0] = { w / 2, h / 5 , 100, 60 };
		POS[1] = { w / 2, (h / 5) * 2 , 110, 60 };
		POS[2] = { w / 2, (h / 5) * 3 , 110, 60 };
		POS[3] = { w / 2, (h / 5) * 4 , 60, 60 };


		for (int i = 0; i < RESO_MENU; i++)
		{
			menu[i] = SDL_CreateTextureFromSurface(gRenderer, temp[i]);
		}

		int opt_index = 1;

		updateSelection(selected, menu, temp, gRenderer, font, labels, TextColour, opt_index);

		while (true)
		{
			SDL_RenderClear(gRenderer);
			//do render stuff here

			SDL_RenderCopy(gRenderer, background, NULL, NULL);

			for (int i = 0; i < RESO_MENU; i++)
			{
				SDL_RenderCopy(gRenderer, menu[i], NULL, &POS[i]);
			}

			//update the screen to the current render
			SDL_RenderPresent(gRenderer);


			time = SDL_GetTicks();
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_QUIT:
					for (int i = 0; i < RESO_MENU; i++)
					{
						SDL_DestroyTexture(menu[i]);
					}
					return 0;

				case SDL_MOUSEMOTION:
					
					if (keyboard) break;

					x = e.motion.x;
					y = e.motion.y;
					for (int i = 1; i < RESO_MENU; i++)
					{
						if (x >= POS[i].x && x <= POS[i].x + POS[i].w && y >= POS[i].y && y <= POS[i].y + POS[i].h)
						{
							if (!selected[i])
							{
								selected[i] = 1;
								SDL_DestroyTexture(menu[i]);
								temp[i] = TTF_RenderText_Solid(font, labels[i], TextColour[1]);
								menu[i] = SDL_CreateTextureFromSurface(gRenderer, temp[i]);
							}
						}

						else
						{
							if (selected[i])
							{
								selected[i] = 0;
								SDL_DestroyTexture(menu[i]);
								temp[i] = TTF_RenderText_Solid(font, labels[i], TextColour[0]);
								menu[i] = SDL_CreateTextureFromSurface(gRenderer, temp[i]);
							}

						}
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					keyboard = false;
					x = e.button.x;
					y = e.button.y;

					for (int i = 1; i < RESO_MENU; i++)
					{
						if (x >= POS[i].x && x <= POS[i].x + POS[i].w && y >= POS[i].y && y <= POS[i].y + POS[i].h)
						{
							if (i == 1)
							{
								SDL_SetWindowSize(win, 800, 600);
							}

							if (i == 2)
							{
								SDL_SetWindowSize(win, 1200, 800);
							}

							for (int i = 0; i < RESO_MENU; i++)
							{
								SDL_DestroyTexture(menu[i]);
							}
							SDL_RenderClear(gRenderer);
							SDL_DestroyRenderer(gRenderer);
							return i;
						}

					}
					break;

				//case SDL_JOYBUTTONDOWN:
				//	switch (e.jbutton.button)
				//	{
				//		//case
				//	}
				//	break;

				case SDL_KEYDOWN:
					keyboard = true;
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:

						for (int i = 0; i < RESO_MENU; i++)
						{
							SDL_DestroyTexture(menu[i]);
						}
						return 0;

					case SDLK_DOWN:

						opt_index += 1;

						if (opt_index > 3)
						{
							opt_index = 1;
						}


						std::cout << opt_index << std::endl;

						updateSelection(selected, menu, temp, gRenderer, font, labels, TextColour, opt_index);
						break;

					case SDLK_UP:
	
						opt_index -= 1;

						if (opt_index < 1)
						{
							opt_index = 3;
						}

						std::cout << opt_index << std::endl;
						updateSelection(selected, menu, temp, gRenderer, font, labels, TextColour, opt_index);
						break;

					case SDLK_SPACE:
						SDL_RenderClear(gRenderer);
						SDL_DestroyRenderer(gRenderer);
						return opt_index;
						break;
					}

				}

			}


		}
		SDL_DestroyRenderer(gRenderer);
	}


};
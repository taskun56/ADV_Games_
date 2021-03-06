#include "../include/SDL.h"
#include "../include/SDL_ttf.h"
#include "../include/SDL_image.h"
#include <iostream>

class GameOver
{

public:

	int showmenu(SDL_Surface *screen, SDL_Event e, SDL_Window *win)
	{
		bool menuRun = true;

		Uint32 time;
		int x, y;
		const int NUMMENU = 3;
		const char *labels[NUMMENU] = { "GameOver","SCORE","Exit" };
		bool selected[NUMMENU] = { 0,0,0};
		SDL_Texture* background;
		SDL_Surface* background_surface;
		SDL_Texture * menu[NUMMENU];
		SDL_Rect POS[NUMMENU];
		SDL_Surface * temp[NUMMENU];
		SDL_Color TextColour[2] = { { 150, 0, 0 },{ 200, 200, 200 } };

		SDL_Renderer * gRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

		TTF_Font *font = TTF_OpenFont("impact.ttf", 80);
		if (font == NULL)
		{
			printf("Font not found");
		}

		for (int i = 0; i < NUMMENU; i++)
		{
			temp[i] = TTF_RenderText_Solid(font, labels[i], TextColour[0]);
		}
		int w;
		int h;

		SDL_GetWindowSize(win, &w, &h);

		background_surface = IMG_Load("background.png");
		background = SDL_CreateTextureFromSurface(gRenderer, background_surface);

		POS[0] = { w / 2, h / 5 , 120, 60 };
		POS[1] = { w / 2, (h / 5) * 2 , 100, 60 };
		POS[2] = { w / 2, (h / 5) * 4 , 80, 60 };
		


		for (int i = 0; i < NUMMENU; i++)
		{
			menu[i] = SDL_CreateTextureFromSurface(gRenderer, temp[i]);
		}



		while (true)
		{
			SDL_RenderClear(gRenderer);
			//do render stuff here
			SDL_RenderCopy(gRenderer, background, NULL, NULL);

			for (int i = 0; i < NUMMENU; i++)
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
					for (int i = 0; i < NUMMENU; i++)
					{
						SDL_DestroyTexture(menu[i]);
					}
					return 0;

				case SDL_MOUSEMOTION:
					x = e.motion.x;
					y = e.motion.y;
					for (int i = 2; i < NUMMENU; i++)
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
					x = e.button.x;
					y = e.button.y;

					for (int i = 2; i < NUMMENU; i++)
					{
						if (x >= POS[i].x && x <= POS[i].x + POS[i].w && y >= POS[i].y && y <= POS[i].y + POS[i].h)
						{
							for (int i = 0; i < NUMMENU; i++)
							{
								SDL_DestroyTexture(menu[i]);
							}
							SDL_RenderClear(gRenderer);
							SDL_DestroyRenderer(gRenderer);
							return i;
						}
					}
					break;

				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:

						for (int i = 0; i < NUMMENU; i++)
						{
							SDL_DestroyTexture(menu[i]);
						}
						return 0;
					}

				}

			}


		}


	}


};
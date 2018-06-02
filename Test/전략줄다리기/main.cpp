#include "SDL/SDL.h"
#include "socket.h"
#include "PullMind_include.hpp"

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib , "lib/SDL2.lib")
#undef main 

VEC_ENTI vec_enti;
MAP_ENTI map_enti;

queue<int> idQ;
void TTF_DrawText(SDL_Renderer* renderer, string text, SDL_Point point, TTF_Font *font, SDL_Color color = { 0,0,0,0 }) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);// 서피스로부터 텍스쳐를 생성한다
	SDL_FreeSurface(surface);//서피스 메모리를 해제 해준다.
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
	SDL_Rect dst;
	dst.x = point.x;
	dst.y = point.y;
	dst.w = src.w;
	dst.h = src.h;
	SDL_RenderCopy(renderer, texture, &src, &dst);
}

int main(void) {

#ifdef SOOHAN
	
	bool quit = false;
	SDL_Window * Window = NULL;
	SDL_Color color = { 0,0,0 ,0 };
	TTF_Init();
	HitMind_TTF_Init();
	HitMind_TTF2_Init();

	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)connectServer, NULL, 0, NULL);


	int Display_X = 1920;
	int Display_Y = 1080;
	SDL_Window* window= SDL_CreateWindow("HitMind_2", 0,0, 1920,1080, SDL_WINDOW_SHOWN);
	SDL_Renderer*	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	string str;
	TTF_Init();
	TTF_Font* font =TTF_OpenFont(".\\font\\NanumGothic.ttf", 30);
	

	SDL_Texture *lobiimage = LoadTexture(renderer, ".\\resources\\image\\tema.jpg");
	SDL_Texture *inputimage = LoadTexture(renderer, ".\\resources\\image\\input.png");
	if (lobiimage == NULL)
		printf("이미지 못불러옴");

	RenderTextureXYWH(renderer, lobiimage, 0, 0, Display_X, Display_Y);
	RenderTextureXYWH(renderer, inputimage, 710, 470, 500, 141);
	while (!quit) {

		SDL_WaitEvent(&event);

		RenderTextureXYWH(renderer, lobiimage, 0, 0, Display_X, Display_Y);
		RenderTextureXYWH(renderer, inputimage, 710, 470, 500, 141);
		

		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_BACKSPACE && !str.empty())
				str.pop_back();
			else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER && !str.empty()) {
				
				
			}
			break;
		case SDL_TEXTINPUT:
			str += event.text.text;
			//SDL_RenderClear(renderer);
			cout << str << endl;
			break;
		case SDL_QUIT:
			quit = TRUE;
			break;
		default:
			break;
		}

		TTF_DrawText(renderer, str, Point(720, 555), font, color);
		SDL_RenderPresent(renderer);

	}

	SDL_Quit();
	return 0;

#else

	SDL_Init(SDL_INIT_EVERYTHING);
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)connectServer, NULL, 0, NULL);

	SDL_Window *window = SDL_CreateWindow("hi", 100, 100, 640, 480, SDL_WINDOW_SHOWN);


	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Entity *entity = new Entity(renderer, "./resources/image/porg.jpg", Rect(100, 100, 100, 100), Rect(100, 100, 100, 100), 1, 1, 1);
	Entity *entity2 = new Entity(renderer, "./resources/image/porg.jpg", Rect(300, 100, 100, 100), Rect(300, 100, 100, 100), 2, 1, 1);
	Entity *entity3 = new Entity(renderer, "./resources/image/porg.jpg", Rect(300, 300, 100, 100), Rect(300, 300, 100, 100), 3, 1, 1);


	entity->RenderEntity();
	entity2->RenderEntity();
	entity3->RenderEntity();
	SDL_RenderPresent(renderer);
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_WaitEventTimeout(&event, 10);
		SDL_RenderClear(renderer);
		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			Entity* entity = (*it);
			if (entity->flag == false) {
				//애니메이션 중이 아님
				entity->RenderEntity();
			}
		}
		int size = idQ.size();
		for (int i = 0; i < size; i++) {
			int id = idQ.front(); idQ.pop();
			Entity * entity = map_enti.find(id)->second;
			if (entity->v.empty()) {
				entity->flag = false;
			}
			else {
				SDL_Point p = entity->v.back(); entity->v.pop_back();
				entity->Animation(p);
				idQ.push(id);
			}
			entity->RenderEntity();
		}



		for (auto it = vec_enti.begin(); it != vec_enti.end(); it++) {
			if ((*it)->Callback(event)) { break; }

		}
		SDL_RenderPresent(renderer);
	}

#endif


	
}
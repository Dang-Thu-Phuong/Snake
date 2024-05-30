#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <SDL.h>
#include <SDL_ttf.h>

SDL_Color Red = { 220, 20, 60 }; // win/over
SDL_Color White = { 255, 255, 255 }; // score
SDL_Color Green = { 85, 107,47 }; // bg

using namespace std;
void renderScore(SDL_Renderer* renderer, int tailLength, int tile, int wtile) ;
void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int tile, vector<int> tailX, vector<int> tailY, int tailLength);
void renderFood(SDL_Renderer* renderer, SDL_Rect food);
bool check(int foodx, int foody, int playerx, int playery) ;
pair<int, int> getFood(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int tile, int wtile, int tailLength) ;
void gameOver(SDL_Renderer* renderer, SDL_Event event, int tile, int wtile, int tailLength) ;
void youWin(SDL_Renderer* renderer, SDL_Event event, int tile, int wtile, int tailLength) ;

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);

	if (TTF_Init() < 0) {
		cout << "Error: " << TTF_GetError() << endl;
	}

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_Rect player;
	player.x = 0;
	player.y = 0;
	player.h = 24;
	player.w = 24;

    SDL_Rect food;
	food.w = 24;
	food.h = 24;
	food.x = 0;
	food.y = 0;

	// so luong duoi ran
	int tailLength = 0;

	// duoi ran
	vector<int> tailX;
	vector<int> tailY;

	// kich thuoc o vuong va so o vuong
	int tile = 28;
	int wtile = 28;

	// vi tri ran
	int x = 0;
	int y = 0;
	int newX = 0;
	int newY = 0;

	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	bool Frame = false; // kiểm tra xem có sự kiện nhập nào chưa
	bool food_y_n = false; // kiểm tra xem có cần tạo lại thức ăn không?

	// vi tri mới cho thưc ăn
	pair<int, int> foodNew = getFood(tailX, tailY, x, y, tile, wtile, tailLength);
	food.x = foodNew.first;
	food.y = foodNew.second;

	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tile*wtile+1, tile*wtile+1, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	float time = SDL_GetTicks() / 500;

	// vòng lặp chính
	while (true) {

		float newTime = SDL_GetTicks() / 200;
		float delta = newTime - time;
		time = newTime;
		Frame = false;

		// điều kiện thắng
        if (tailLength >= 50) {
			youWin(renderer, event, tile, wtile, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			food_y_n = false;
			foodNew = getFood(tailX, tailY, x, y, tile, wtile, tailLength);

			if (food.x == -100 && food.y == -100) {
				food_y_n = true;
			}

			food.x = foodNew.first;
			food.y = foodNew.second;
		}

		// Điều khiển
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.type == SDL_KEYDOWN && Frame == false) {

				if ( event.key.keysym.scancode == SDL_SCANCODE_UP) {
					up = true;
					left = false;
					right = false;
					down = false;
					Frame = true;
				}
				else if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					up = false;
					left = true;
					right = false;
					down = false;
					Frame = true;
				}
				else if ( event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					up = false;
					left = false;
					right = false;
					down = true;
					Frame = true;
				}
				else if ( event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					up = false;
					left = false;
					right = true;
					down = false;
					Frame = true;
				}

			}

		}
		// cập nhập vị trí người chơi
		newX = x;
		newY = y;

		if (up) {
			y -= delta * tile;
		}
		else if (left) {
			x -= delta * tile;
		}
		else if (right) {
			x += delta * tile;
		}
		else if (down) {
			y += delta * tile;
		}

		if (food_y_n == true) {

			food_y_n = false;
			foodNew = getFood(tailX, tailY, x, y, tile, wtile, tailLength);
			food.x = foodNew.first;
			food.y = foodNew.second;

			if (food.x == -100 && food.y == -100) {
				food_y_n = true;
			}

		}
        // va chạm với thức ăn
		if (check(food.x, food.y, x, y)) {

			foodNew = getFood(tailX, tailY, x, y, tile, wtile, tailLength);
			food.x = foodNew.first;
			food.y = foodNew.second;

			if (food.x == -100 && food.y == -100) {
				food_y_n = true;
			}

			tailLength++;
		}

		if (delta * tile == 28) {

			if (tailX.size() != tailLength) {
				tailX.push_back(newX);
				tailY.push_back(newY);
			}

			for (int i = 0; i < tailLength; i++) {

				if (i > 0) {
					tailX[i - 1] = tailX[i];
					tailY[i - 1] = tailY[i];
				}

			}

			if (tailLength > 0) {
				tailX[tailLength - 1] = newX;
				tailY[tailLength - 1] = newY;
			}

		}

        // kết thúc khi cắn đuôi
		for (int i = 0; i < tailLength; i++) {

			if (x == tailX[i] && y == tailY[i]) {
				gameOver(renderer, event, tile, wtile, tailLength);
				x = 0;
				y = 0;
				up = false;
				left = false;
				right = false;
				down = false;
				tailX.clear();
				tailY.clear();
				tailLength = 0;
				food_y_n = false;

				foodNew = getFood(tailX, tailY, x, y, tile, wtile, tailLength);
				if (food.x == -100 && food.y == -100) {
					food_y_n = true;
				}

				food.x = foodNew.first;
				food.y = foodNew.second;
			}

		}


        // kết thúc khi chạm tường
        if (x < 0 || y < 0 || x > tile * wtile - tile || y > tile * wtile - tile) {
			gameOver(renderer, event, tile, wtile, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			food_y_n = false;
			foodNew = getFood(tailX, tailY, x, y, tile, wtile, tailLength);
			food.x = foodNew.first;
			food.y = foodNew.second;

			if (food.x == -100 && food.y == -100) {
				food_y_n = true;
			}

		}

		renderFood(renderer, food);
		renderPlayer(renderer, player, x, y, tile, tailX, tailY, tailLength);
		renderScore(renderer, tailLength, tile, wtile);

		SDL_RenderDrawLine(renderer, 0, 0, 0, 28 * 28);
		SDL_RenderDrawLine(renderer, 0, 28*28, 28 * 28, 28 * 28);
		SDL_RenderDrawLine(renderer, 28*28, 28 * 28, 28*28, 0);
		SDL_RenderDrawLine(renderer, 28*28, 0, 0, 0);

		SDL_RenderPresent(renderer);

		SDL_SetRenderDrawColor(renderer, 85, 107, 47, 255); // bg

		SDL_RenderClear(renderer);
	}

	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;

}


void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int tile, vector<int> tailX, vector<int> tailY, int tailLength) {
	SDL_SetRenderDrawColor(renderer, 0, 34, 81, 64);

	for (int i = 0; i < tailLength; i++) {
		player.x = tailX[i];
		player.y = tailY[i];
		SDL_RenderFillRect(renderer, &player);
	}

	player.x = x;
	player.y = y;

	SDL_RenderFillRect(renderer, &player);
}
void renderFood(SDL_Renderer* renderer, SDL_Rect food) {
	SDL_SetRenderDrawColor(renderer, 225, 0, 0, 255);
	SDL_RenderFillRect(renderer, &food);
}
void renderScore(SDL_Renderer* renderer, int tailLength, int tile, int wtile) {

	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 20);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), White);
	SDL_Texture* scoreM = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 28;
	scoreRect.x = 0;
	scoreRect.y = (tile*wtile)-28;
	SDL_RenderCopy(renderer, scoreM, NULL, &scoreRect);

	TTF_CloseFont(font);
}

bool check(int foodx, int foody, int playerx, int playery) {

	if (playerx == foodx && playery == foody){
		return true;
	}

	return false;
}

pair<int, int> getFood(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int tile, int wtile, int tailLength) {
	bool valid = false;
	int x = 0;
	int y = 0;
	srand(time(0));
	x = tile * (rand() % wtile);
	y = tile * (rand() % wtile);
	valid = true;

	// kiểm tra xem có trùng duôi không
	for (int i = 0; i < tailLength; i++) {

		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY)) {
			valid = false;
		}

	}

	if (!valid) {
		pair<int, int> foodNew;
		foodNew = make_pair(-100, -100);
		return foodNew;
	}

	pair<int, int> foodNew;
	foodNew = make_pair(x, y);

	return foodNew;
}

void gameOver(SDL_Renderer* renderer, SDL_Event event, int tile, int wtile, int tailLength) {

	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 20);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* gameover = TTF_RenderText_Solid(font, "Game Over ", Red);
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), White);
	SDL_Texture* gameOverM = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* scoreM = SDL_CreateTextureFromSurface(renderer, score);

	SDL_Rect gameoverRect;
	gameoverRect.w = 400;
	gameoverRect.h = 300;
	gameoverRect.x = ((tile*wtile) / 2)-(gameoverRect.w/2);
	gameoverRect.y = ((tile*wtile) / 2)-(gameoverRect.h/2)+100;

    SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 28;
	scoreRect.x = ((tile*wtile) / 2)-(scoreRect.w/2) ;
	scoreRect.y = ((tile*wtile) / 2)-(scoreRect.h/2)-50;;

	SDL_RenderCopy(renderer, gameOverM, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, scoreM, NULL, &scoreRect);

	TTF_CloseFont(font);

	while (true) {
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}

		}

	}

}

void youWin(SDL_Renderer* renderer, SDL_Event event, int tile, int wtile, int tailLength) {

	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 20);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* youwin = TTF_RenderText_Solid(font, "You won!", Red);
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), White);
	SDL_Texture* winM = SDL_CreateTextureFromSurface(renderer, youwin);
	SDL_Texture* scoreM = SDL_CreateTextureFromSurface(renderer, score);

	SDL_Rect winRect;
	winRect.w = 200;
	winRect.h = 100;
	winRect.x = ((tile*wtile) / 2) - (winRect.w / 2);
	winRect.y = ((tile*wtile) / 2) - (winRect.h / 2) - 50;

    SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 28;
	scoreRect.x = ((tile*wtile) / 2)-(scoreRect.w/2) ;
	scoreRect.y = ((tile*wtile) / 2)-(scoreRect.h/2)-50;;

	SDL_RenderCopy(renderer, winM, NULL, &winRect);
	SDL_RenderCopy(renderer, scoreM, NULL, &scoreRect);

	TTF_CloseFont(font);

	while (true) {
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}
		}
	}
}

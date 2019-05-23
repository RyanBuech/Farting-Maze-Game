#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

//Game constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 20;
const int BALL_WIDTH = 20;
const int BALL_HEIGHT = 20;
const int BALL_MAX_SPEED = 20;

const int WALL_WIDTH = 20;
const int WALL_HEIGHT = 20;

const int WALL_WIDTH1 = 800;
const int WALL_HEIGHT1 = 20;

const int WALL_WIDTH2 = 20;
const int WALL_HEIGHT2 = 200;

const int WALL_WIDTH3 = 80;
const int WALL_HEIGHT3 = 300;

const int PLAYER_X = PLAYER_WIDTH;

const int PLAYER_SPEED = 10;

const int FPS = 30;
const int FRAME_DELAY = 1000 / FPS;

//Surfaces
SDL_Surface *Backbuffer = NULL;
SDL_Surface *BackgroundImage = NULL;
SDL_Surface *FINISH = NULL;
SDL_Surface *WallImage = NULL;
SDL_Surface *WallImage1 = NULL;
SDL_Surface *WallImage2 = NULL;
SDL_Surface *WallImage3 = NULL;


SDL_Surface *PlayerImage = NULL;

//Font
TTF_Font *GameFont = NULL;

//Sounds
Mix_Chunk *BallBounceSound = NULL;
Mix_Chunk *BallSpawnSound = NULL;
Mix_Chunk *PlayerScoreSound = NULL;
Mix_Chunk *EnemyScoreSound = NULL;
Mix_Chunk *FartSound = NULL;

//Music
Mix_Music *GameMusic = NULL;

SDL_Rect PlayerRect;
SDL_Rect BallRect;
SDL_Rect FinishRect;
SDL_Rect WallRect;
SDL_Rect WallRect1;
SDL_Rect WallRect2;
SDL_Rect WallRect3;
SDL_Rect WallRect4;
SDL_Rect WallRect5;
SDL_Rect WallRect6;
SDL_Rect WallRect7;
SDL_Rect WallRect8;
SDL_Rect WallRect9;
SDL_Rect WallRect10;
SDL_Rect WallRect11;
SDL_Rect WallRect12;
SDL_Rect WallRect13;
SDL_Rect WallRect14;
SDL_Rect WallRect15;
SDL_Rect WallRect16;
SDL_Rect WallRect17;
SDL_Rect WallRect18;
SDL_Rect WallRect19;
SDL_Rect WallRect20;
SDL_Rect WallRect21;
SDL_Rect WallRect22;

SDL_Surface* LoadImage(char* fileName);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
//void DrawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b);
bool ProgramIsRunning();
bool LoadFiles();
void FreeFiles();
bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2);
bool InitSDL();
void ResetGame();
bool InitGame();
void UpdatePlayer();
void RunGame();
void DrawGame();
void FreeGame();

int main(int argc, char *argv[])
{
	if (!InitGame())
	{
		FreeGame();   //If InitGame failed, kill the program
		return 0;
	}

	while (ProgramIsRunning())
	{
		long int oldTime = SDL_GetTicks();  //We will use this later to see how long it took to update the frame
		SDL_FillRect(Backbuffer, NULL, 0);  //Clear the screen
		RunGame();                          //Update the game
		DrawGame();                         //Draw the screen

		int frameTime = SDL_GetTicks() - oldTime;

		if (frameTime < FRAME_DELAY)            //Dont delay if we dont need to
			SDL_Delay(FRAME_DELAY - frameTime);     //Delay
		SDL_Flip(Backbuffer);               //Flip the screen
	}

	FreeGame();     //Gracefully release SDL and its resources.

	return 0;
}

SDL_Surface* LoadImage(char* fileName)
{
	SDL_Surface* imageLoaded = NULL;
	SDL_Surface* processedImage = NULL;

	imageLoaded = SDL_LoadBMP(fileName);

	if (imageLoaded != NULL)
	{
		processedImage = SDL_DisplayFormat(imageLoaded);
		SDL_FreeSurface(imageLoaded);

		if (processedImage != NULL)
		{
			Uint32 colorKey = SDL_MapRGB(processedImage->format, 0xFF, 0, 0xFF);
			SDL_SetColorKey(processedImage, SDL_SRCCOLORKEY, colorKey);
		}

	}

	return processedImage;
}

void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;

	SDL_BlitSurface(image, NULL, destSurface, &destRect);
}

bool ProgramIsRunning()
{
	SDL_Event event;

	bool running = true;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			running = false;

		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;
			}
		}
	}

	return running;
}

bool LoadFiles()
{
	//Load images
	BackgroundImage = LoadImage("graphics/blackBG.bmp");
	FINISH = LoadImage("graphics/finish.bmp");
	WallImage = LoadImage("graphics/wall.bmp");
	WallImage1 = LoadImage("graphics/wall1.bmp");
	WallImage2 = LoadImage("graphics/wall2.bmp");
	WallImage3 = LoadImage("graphics/wall3.bmp");
	PlayerImage = LoadImage("graphics/ball.bmp");

	//Error checking images
	if (BackgroundImage == NULL)
		return false;
	if (WallImage == NULL)
		return false;
	if (WallImage1 == NULL)
		return false;
	if (PlayerImage == NULL)
		return false;

	//Load font
	GameFont = TTF_OpenFont("graphics/alfphabet.ttf", 30);

	//Error check font
	if (GameFont == NULL)
		return false;

	//Load sounds
	BallBounceSound = Mix_LoadWAV("audio/ballBounce.wav");
	BallSpawnSound = Mix_LoadWAV("audio/ballSpawn.wav");
	PlayerScoreSound = Mix_LoadWAV("audio/playerScore.wav");
	EnemyScoreSound = Mix_LoadWAV("audio/enemyScore.wav");
	FartSound = Mix_LoadWAV("audio/fart.wav");

	//Error check sounds
	if (BallBounceSound == NULL)
		return false;
	if (PlayerScoreSound == NULL)
		return false;
	if (EnemyScoreSound == NULL)
		return false;

	//Load music
	GameMusic = Mix_LoadMUS("audio/song.mp3");

	//Error check music
	if (GameMusic == NULL)
		return false;

	return true;
}

void FreeFiles()
{
	//Free images
	SDL_FreeSurface(BackgroundImage);
	SDL_FreeSurface(WallImage);
	SDL_FreeSurface(WallImage1);
	SDL_FreeSurface(WallImage2);
	SDL_FreeSurface(WallImage3);
	SDL_FreeSurface(FINISH);
	SDL_FreeSurface(PlayerImage);

	//Free font
	TTF_CloseFont(GameFont);

	//Free sounds
	Mix_FreeChunk(BallBounceSound);
	Mix_FreeChunk(BallSpawnSound);
	Mix_FreeChunk(PlayerScoreSound);
	Mix_FreeChunk(EnemyScoreSound);
	Mix_FreeChunk(FartSound);

	//Free music
	Mix_FreeMusic(GameMusic);
}

bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2)
{
	if (rect1.x >= rect2.x + rect2.w)
		return false;

	if (rect1.y >= rect2.y + rect2.h)
		return false;

	if (rect2.x >= rect1.x + rect1.w)
		return false;

	if (rect2.y >= rect1.y + rect1.h)
		return false;

	return true;
}

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	//Init audio subsystem
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		return false;
	}

	//Init TTF subsystem
	if (TTF_Init() == -1)
	{
		return false;
	}

	//Generate screen
	Backbuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);

	//Error check Backbuffer
	if (Backbuffer == NULL)
		return false;

	return true;
}

void ResetGame()
{
	//Position the player's paddle
	PlayerRect.x = PLAYER_X;
	PlayerRect.y = SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2;
	PlayerRect.w = PLAYER_WIDTH;
	PlayerRect.h = PLAYER_HEIGHT;

	FinishRect.x = 780;
	FinishRect.y = 200;
	FinishRect.w = WALL_WIDTH2;
	FinishRect.h = WALL_HEIGHT2;

	//Position the wall
	WallRect.x = 100;
	WallRect.y = 100;
	WallRect.w = WALL_WIDTH;
	WallRect.h = WALL_HEIGHT;

	WallRect1.x = 0;
	WallRect1.y = 580;
	WallRect1.w = WALL_WIDTH1;
	WallRect1.h = WALL_HEIGHT1;

	WallRect2.x = 0;
	WallRect2.y = 0;
	WallRect2.w = WALL_WIDTH1;
	WallRect2.h = WALL_HEIGHT1;

	WallRect3.x = 0;
	WallRect3.y = 20;
	WallRect3.w = WALL_WIDTH2;
	WallRect3.h = WALL_HEIGHT2;

	WallRect4.x = 0;
	WallRect4.y = 380;
	WallRect4.w = WALL_WIDTH2;
	WallRect4.h = WALL_HEIGHT2;

	WallRect5.x = 780;
	WallRect5.y = 20;
	WallRect5.w = WALL_WIDTH2;
	WallRect5.h = WALL_HEIGHT2;

	WallRect6.x = 780;
	WallRect6.y = 380;
	WallRect6.w = WALL_WIDTH2;
	WallRect6.h = WALL_HEIGHT2;

	WallRect7.x = 80;
	WallRect7.y = 200;
	WallRect7.w = WALL_WIDTH3;
	WallRect7.h = WALL_HEIGHT3;

	WallRect8.x = 400;
	WallRect8.y = 100;
	WallRect8.w = WALL_WIDTH3;
	WallRect8.h = WALL_HEIGHT3;

	WallRect9.x = 540;
	WallRect9.y = 200;
	WallRect9.w = WALL_WIDTH3;
	WallRect9.h = WALL_HEIGHT3;

	WallRect10.x = 400;
	WallRect10.y = 280;
	WallRect10.w = WALL_WIDTH3;
	WallRect10.h = WALL_HEIGHT3;

	WallRect11.x = 680;
	WallRect11.y = 120;
	WallRect11.w = WALL_WIDTH3;
	WallRect11.h = WALL_HEIGHT3;

	WallRect12.x = 640;
	WallRect12.y = 20;
	WallRect12.w = WALL_WIDTH2;
	WallRect12.h = WALL_HEIGHT2;

	WallRect13.x = 40;
	WallRect13.y = 500;
	WallRect13.w = WALL_WIDTH2;
	WallRect13.h = WALL_HEIGHT2;

	WallRect14.x = 200;
	WallRect14.y = 20;
	WallRect14.w = WALL_WIDTH3;
	WallRect14.h = WALL_HEIGHT3;

	WallRect15.x = 200;
	WallRect15.y = 200;
	WallRect15.w = WALL_WIDTH2;
	WallRect15.h = WALL_HEIGHT2;

	WallRect16.x = 680;
	WallRect16.y = 420;
	WallRect16.w = WALL_WIDTH3;
	WallRect16.h = WALL_HEIGHT3;

	WallRect17.x = 720;
	WallRect17.y = 60;
	WallRect17.w = WALL_WIDTH;
	WallRect17.h = WALL_HEIGHT;

	WallRect18.x = 200;
	WallRect18.y = 560;
	WallRect18.w = WALL_WIDTH3;
	WallRect18.h = WALL_HEIGHT3;

	WallRect19.x = 260;
	WallRect19.y = -100;
	WallRect19.w = WALL_WIDTH3;
	WallRect19.h = WALL_HEIGHT3;

	WallRect20.x = 580;
	WallRect20.y = -160;
	WallRect20.w = WALL_WIDTH3;
	WallRect20.h = WALL_HEIGHT3;

	WallRect21.x = 320;
	WallRect21.y = 460;
	WallRect21.w = WALL_WIDTH3;
	WallRect21.h = WALL_HEIGHT3;

	WallRect22.x = 340;
	WallRect22.y = 320;
	WallRect22.w = WALL_WIDTH;
	WallRect22.h = WALL_HEIGHT;

	//Play the spawn sound
	Mix_PlayChannel(-1, BallSpawnSound, 0);
}

bool InitGame()
{
	//Init SDL
	if (!InitSDL())
		return false;

	//Load Files
	if (!LoadFiles())
		return false;

	//Initiatialize game variables

	//Set the title
	SDL_WM_SetCaption("Farting Maze Game!", NULL);

	//This can also set the initial variables
	ResetGame();

	//Play Music
	Mix_PlayMusic(GameMusic, -1);

	return true;
}

void UpdatePlayer()
{
	Uint8 *keys = SDL_GetKeyState(NULL);

	//Move the paddle when the up/down key is pressed
	if (keys[SDLK_UP]||keys[SDLK_w])
		PlayerRect.y -= PLAYER_SPEED;

	if (keys[SDLK_DOWN]||keys[SDLK_s])
		PlayerRect.y += PLAYER_SPEED;

	if (keys[SDLK_LEFT]||keys[SDLK_a])
		PlayerRect.x -= PLAYER_SPEED;

	if (keys[SDLK_RIGHT]||keys[SDLK_d])
		PlayerRect.x += PLAYER_SPEED;

	//Make sure the paddle doesn't leave the screen
	if (PlayerRect.y < 0)
		PlayerRect.y = 0;

	if (PlayerRect.y > SCREEN_HEIGHT - PlayerRect.h)
		PlayerRect.y = SCREEN_HEIGHT - PlayerRect.h;

	if (PlayerRect.x < 0)
		PlayerRect.x = 0;

	if (PlayerRect.x > SCREEN_WIDTH - PlayerRect.h)
		PlayerRect.x = SCREEN_WIDTH - PlayerRect.h;

	//Player collider
	
		//If the ball hits the player, make it bounce
		if (RectsOverlap(WallRect, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect1, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect2, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect3, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect4, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect5, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect6, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect7, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect8, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect9, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect10, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect11, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect12, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect13, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}
		
		if (RectsOverlap(WallRect14, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect15, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}


		if (RectsOverlap(WallRect16, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}


		if (RectsOverlap(WallRect17, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect18, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect19, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect20, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect21, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}

		if (RectsOverlap(WallRect22, PlayerRect))
		{
			PlayerRect.y = 300;
			PlayerRect.x = 0;
			int channel = Mix_PlayChannel(-1, FartSound, 0);
		}
}



void RunGame()
{
	UpdatePlayer();
}

void DrawGame()
{
	DrawImage(BackgroundImage, Backbuffer, 0, 0);
	DrawImage(FINISH, Backbuffer, FinishRect.x, FinishRect.y);
	DrawImage(WallImage, Backbuffer, WallRect.x, WallRect.y);
	DrawImage(WallImage1, Backbuffer, WallRect1.x, WallRect1.y);
	DrawImage(WallImage1, Backbuffer, WallRect2.x, WallRect2.y);
	DrawImage(WallImage2, Backbuffer, WallRect3.x, WallRect3.y);
	DrawImage(WallImage2, Backbuffer, WallRect4.x, WallRect4.y);
	DrawImage(WallImage2, Backbuffer, WallRect5.x, WallRect5.y);
	DrawImage(WallImage2, Backbuffer, WallRect6.x, WallRect6.y);
	DrawImage(WallImage3, Backbuffer, WallRect7.x, WallRect7.y);
	DrawImage(WallImage3, Backbuffer, WallRect8.x, WallRect8.y);
	DrawImage(WallImage3, Backbuffer, WallRect9.x, WallRect9.y);
	DrawImage(WallImage3, Backbuffer, WallRect10.x, WallRect10.y);
	DrawImage(WallImage3, Backbuffer, WallRect11.x, WallRect11.y);
	DrawImage(WallImage2, Backbuffer, WallRect12.x, WallRect12.y);
	DrawImage(WallImage2, Backbuffer, WallRect13.x, WallRect13.y);
	DrawImage(WallImage3, Backbuffer, WallRect14.x, WallRect14.y);
	DrawImage(WallImage3, Backbuffer, WallRect15.x, WallRect15.y);
	DrawImage(WallImage3, Backbuffer, WallRect16.x, WallRect16.y);
	DrawImage(WallImage, Backbuffer, WallRect17.x, WallRect17.y);
	DrawImage(WallImage3, Backbuffer, WallRect18.x, WallRect18.y);
	DrawImage(WallImage3, Backbuffer, WallRect19.x, WallRect19.y);
	DrawImage(WallImage3, Backbuffer, WallRect20.x, WallRect20.y);
	DrawImage(WallImage3, Backbuffer, WallRect21.x, WallRect21.y);
	DrawImage(WallImage, Backbuffer, WallRect22.x, WallRect22.y);
	DrawImage(PlayerImage, Backbuffer, PlayerRect.x, PlayerRect.y);

	char playerHUD[64];
	char enemyHUD[64];
}

void FreeGame()
{
	Mix_HaltMusic();    //Stop the music
	FreeFiles();        //Release the files we loaded
	Mix_CloseAudio();   //Close the audio system
	TTF_Quit();         //Close the font system
	SDL_Quit();         //Close SDL
}

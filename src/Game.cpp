#include "Game.h"
#include <SDL_image.h>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>

#include <Drunk.h>
#include <SimpleAI.h>
#include <HamiltonianCircuitAI.h>

namespace game{
//*************************************************************************************************
Game::Game():
    m_backbuffer(nullptr),
    m_background(nullptr),
    m_bricks(nullptr),
    m_font(nullptr),
    m_music(nullptr),
    m_sfxEat(nullptr),
    m_sfxGameover(nullptr)
{

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    m_rng.seed((unsigned long)seed);
    m_distX = new std::uniform_int_distribution<std::mt19937::result_type>(0, BOARD_WIDTH - 1);
    m_distY = new std::uniform_int_distribution<std::mt19937::result_type>(0, BOARD_HEIGT - 1);

    m_ai = nullptr;//new HamiltonianCircuitAI();
}

//*************************************************************************************************
Game::~Game()
{
    delete m_ai;

    delete m_distX;
    delete m_distY;

    if (m_background) SDL_FreeSurface(m_background);
    if (m_bricks) SDL_FreeSurface(m_bricks);
    if (m_backbuffer) SDL_FreeSurface(m_backbuffer);
    if (m_font) SDL_FreeSurface(m_font);
    if (m_music) Mix_FreeMusic(m_music);
    if (m_sfxEat) Mix_FreeChunk(m_sfxEat);
    if (m_sfxGameover) Mix_FreeChunk(m_sfxGameover);
    Mix_CloseAudio();
    SDL_Quit();
}

//*************************************************************************************************
bool Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 )
    {
        return false;
    }

    m_backbuffer = SDL_SetVideoMode(400, 600, 32, SDL_SWSURFACE);

    if (m_backbuffer == nullptr)
        return false;

    m_background = loadImage("res/bg.png");
    if (m_background == nullptr)
        return false;

    m_bricks = loadImage("res/bricks.bmp");
    if (m_bricks == nullptr)
        return false;

    m_font = loadImage("res/numbers.png");
    if (m_font == nullptr)
        return false;

    m_music = Mix_LoadMUS("res/music.wav");
    if (m_music == nullptr)
    {
         printf("Failed to load audio : music.wav\n");
         return false;
    }

    m_sfxEat = Mix_LoadWAV("res/eat.wav" );
    if (m_sfxEat == nullptr)
    {
        printf("Failed to load SFX\n");
        return false;
    }

    m_sfxGameover = Mix_LoadWAV("res/gameover.wav" );
    if (m_sfxGameover == nullptr)
    {
        printf("Failed to load SFX\n");
        return false;
    }

    SDL_WM_SetCaption("Snake", nullptr);

    initGame();

    return true;
}

//*************************************************************************************************
SDL_Surface* Game::loadImage(char* fileName)
{
    SDL_Surface* imageLoaded = nullptr;
    imageLoaded = IMG_Load(fileName);
    if (imageLoaded == nullptr)
    {
        printf("Failed to load image : %s\n", fileName);
        return nullptr;
    }

    SDL_Surface* surface = SDL_DisplayFormat(imageLoaded);
    SDL_FreeSurface(imageLoaded);
    if (surface == nullptr)
    {
        printf("Failed to load image : %s\n", fileName);
        return nullptr;
    }

    Uint32 colorKey = SDL_MapRGB(surface->format, 255, 0, 255);
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY, colorKey);

    return surface;
}

//*************************************************************************************************
void Game::start()
{
    while (isRunning())
    {
        checkInput();
        if (checkCollision() == true) // Game Over
        {
            continue;
        }
        move();
        repaint();
        SDL_Delay(GAME_SPPED);
    }
}

//*************************************************************************************************
void Game::checkInput()
{
    if (m_ai != nullptr)
    {
        m_board.m_direction = m_ai->getNextMove(m_board);
        return;
    }

    Uint8 * keys = SDL_GetKeyState(nullptr);
    if(keys[SDLK_LEFT])
        if (m_board.m_direction != RIGHT)
            m_board.m_direction = LEFT;
    if(keys[SDLK_RIGHT])
        if (m_board.m_direction != LEFT)
            m_board.m_direction = RIGHT;
    if(keys[SDLK_UP])
        if (m_board.m_direction != DOWN)
            m_board.m_direction = UP;
    if(keys[SDLK_DOWN])
        if (m_board.m_direction != UP)
            m_board.m_direction = DOWN;
}

//*************************************************************************************************
bool Game::checkCollision()
{
    if (m_board.m_x[0] == (BOARD_WIDTH - 1) && m_board.m_direction == RIGHT)
    {
       gameOver();
       return true;
    }
    else if (m_board.m_x[0] == 0 && m_board.m_direction == LEFT)
    {
       gameOver();
       return true;
    }
    if (m_board.m_y[0] == (BOARD_HEIGT - 1) && m_board.m_direction == DOWN)
    {
       gameOver();
       return true;
    }
    else if (m_board.m_y[0] == 0 && m_board.m_direction == UP)
    {
       gameOver();
       return true;
    }
    if (m_board.m_GoalX == m_board.m_x[0] && m_board.m_GoalY == m_board.m_y[0])
    {
        Mix_PlayChannel(-1, m_sfxEat, 0);
        m_board.m_length++;
        m_score += 1;
        if (m_board.m_length == CELL_COUNT)
        {
            gameOver();
            return true;
        }

        addGoal();
        return false;
    }
    if (m_board.m_length == 3)
        return false;
    for (int i = m_board.m_length - 1; i > 0; --i)
    {
        if (m_board.m_x[0] == m_board.m_x[i] && m_board.m_y[0] == m_board.m_y[i])
        {
            gameOver();
            return true;
        }
    }
    return false;
}

//*************************************************************************************************
void Game::gameOver()
{
    Mix_HaltMusic();
    Mix_PlayChannel(-1, m_sfxGameover, 0);
    SDL_Delay(500);
    initGame();
}

//*************************************************************************************************
void Game::addGoal()
{
    // TODO : Lookup table based goal generation
    int iteration = 0;
    bool found = true;
    do {
        m_board.m_GoalX = (*m_distX)(m_rng);
        m_board.m_GoalY = (*m_distY)(m_rng);
        std::cout <<  m_board.m_GoalX << "," << m_board.m_GoalY << std::endl;
        found = true;
        iteration++;
        for (int i = 0; i < m_board.m_length - 1; ++i)
        {
            if (m_board.m_x[i] == m_board.m_GoalX && m_board.m_y[i] == m_board.m_GoalY)
            {
                found = false;
                break;
            }
        }
        SDL_Delay(1);
    } while(iteration < 4 && !found);

    if (!found)
    {
        int emptyCellCount = CELL_COUNT - m_board.m_length;
        int emptyCells[emptyCellCount];
        bool board[CELL_COUNT];
        std::fill(board, board + CELL_COUNT, false);

        for (int i = 0; i < m_board.m_length - 1; ++i)
        {
            board[m_board.m_x[i] * BOARD_WIDTH + m_board.m_y[i]] = true;
        }

        for (int i = 0, j = 0; i < CELL_COUNT; ++i)
        {
            if (board[i])
                continue;
            emptyCells[j++] = i;
        }
        std::uniform_int_distribution<std::mt19937::result_type> tempDist(0, emptyCellCount - 1);
        int pos = tempDist(m_rng);
        m_board.m_GoalX = emptyCells[pos] % BOARD_WIDTH;
        m_board.m_GoalY = emptyCells[pos] / BOARD_WIDTH;
    }
}

//*************************************************************************************************
void Game::repaint()
{
    SDL_BlitSurface(m_background, nullptr, m_backbuffer, nullptr);

    SDL_Rect sourceRect;
    sourceRect.y = 0;
    sourceRect.x = 10;
    sourceRect.w = CELL_SIZE;
    sourceRect.h = CELL_SIZE;

    SDL_Rect destRect;
    destRect.x = toDisplayPosition(m_board.m_x[0]);
    destRect.y = toDisplayPosition(m_board.m_y[0]);
    SDL_BlitSurface(m_bricks, &sourceRect, m_backbuffer, &destRect);

    sourceRect.x = 0;
    for (int i = 1; i < m_board.m_length; ++i)
    {
        destRect.x = toDisplayPosition(m_board.m_x[i]);
        destRect.y = toDisplayPosition(m_board.m_y[i]);
        SDL_BlitSurface(m_bricks, &sourceRect, m_backbuffer, &destRect);
    }

    sourceRect.x = 20;
    destRect.x = toDisplayPosition(m_board.m_GoalX);
    destRect.y = toDisplayPosition(m_board.m_GoalY);
    SDL_BlitSurface(m_bricks, &sourceRect, m_backbuffer, &destRect);

    renderScore();

    SDL_Flip(m_backbuffer);
}

//*************************************************************************************************
void Game::renderScore()
{
    SDL_Rect sourceRect;
    sourceRect.y = 0;
    sourceRect.w = FONT_WIDTH;
    sourceRect.h = FONT_HEIGHT;

    SDL_Rect destRect;
    destRect.y = 507;

    int score = m_score;
    int pos = 0;
    while (score > 0)
    {
        sourceRect.x = (score % 10) * FONT_WIDTH;
        destRect.x = 320 - (pos * FONT_WIDTH);
        SDL_BlitSurface(m_font, &sourceRect, m_backbuffer, &destRect);
        score = score / 10;
        ++pos;
    }
}

//*************************************************************************************************
void Game::move()
{
    for (int i = m_board.m_length - 1; i > 0; --i)
    {
        m_board.m_x[i] = m_board.m_x[i - 1];
        m_board.m_y[i] = m_board.m_y[i - 1];
    }
    switch (m_board.m_direction)
    {
        case LEFT:  m_board.m_x[0] -= 1; break;
        case RIGHT: m_board.m_x[0] += 1; break;
        case UP:    m_board.m_y[0] -= 1; break;
        case DOWN:  m_board.m_y[0] += 1; break;
    }
}

//*************************************************************************************************
void Game::initGame()
{
    m_board.m_direction = RIGHT;
    m_board.m_length = 3;
    int startPos = START_POS;
    for (int i = 0; i < m_board.m_length ; ++i)
    {
        m_board.m_x[i] = startPos - i;
        m_board.m_y[i] = startPos;
    }

    m_score = 0;
    addGoal();

    if (!Mix_PlayingMusic())
        Mix_PlayMusic(m_music, -1);
}

//*************************************************************************************************
bool Game::isRunning()
{
    SDL_Event event;
    bool isRunning = true;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            isRunning = false;
    }
    return isRunning;
}

}

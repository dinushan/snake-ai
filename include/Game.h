#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <chrono>
#include <random>

#define CELL_SIZE 10
#define GAME_WIDTH 300
#define BOARD_WIDTH 30
#define BOARD_HEIGT 40
#define START_POS 10
#define GAME_HEIGHT 400
#define CELL_COUNT (BOARD_WIDTH * BOARD_HEIGT)
#define FONT_WIDTH 20
#define FONT_HEIGHT 25

#define GAME_SPPED 70

class AI;
namespace game{

    enum Direction{
        UNDEFINED = 0,
        LEFT,
        UP,
        RIGHT,
        DOWN
    };

    struct Board
    {
        int m_x[CELL_COUNT];
        int m_y[CELL_COUNT];
        int m_length;
        Direction m_direction;

        int m_GoalX;
        int m_GoalY;
    };


    class Game
    {
        public:
            Game();
            ~Game();
            bool init();
            void start();

        protected:

        private:

            SDL_Surface* m_backbuffer;
            SDL_Surface* m_background;
            SDL_Surface* m_bricks;
            SDL_Surface* m_font;
            Mix_Music *m_music;
            Mix_Chunk *m_sfxEat;
            Mix_Chunk *m_sfxGameover;

            Board m_board;

            int m_score;

            bool isRunning();
            void checkInput();
            void addGoal();
            void checkGoal();
            bool checkCollision();
            void move();
            void repaint();
            void renderScore();
            void initGame();
            void gameOver();
            inline int toDisplayPosition(int pos) { return pos * CELL_SIZE + 50; }

            SDL_Surface* loadImage(char* filename);

            std::mt19937 m_rng;
            std::uniform_int_distribution<std::mt19937::result_type>* m_distX;
            std::uniform_int_distribution<std::mt19937::result_type>* m_distY;

            AI* m_ai;
    };
}
#endif // GAMEH

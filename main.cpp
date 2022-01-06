#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL_image.h>

namespace t3new {
    /**
     * Prints the explanatory string of an exception.
     * If the exception is nested, recursive to print the explanatory of the exception it holds.
     * @param e Exception.
     * @param level Depth of exception.
     */
    void print_exception(const std::exception &e, int level = 0) {
        std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
        try {
            std::rethrow_if_nested(e);
        } catch (const std::exception &e) {
            print_exception(e, level + 1);
        }
    }

    /**
     * Throws exception with comments.
     * @param s Commentary.
     */
    void throw_exception(const std::string &s, const char *SDL_ErrorFunction(void) = SDL_GetError) {
        auto msg = std::string("SDL Error: " + std::string(SDL_ErrorFunction()));
        std::throw_with_nested(std::runtime_error(msg));
    }

    class Window {

    };

    class Mouse {

    };

    class Board {
    private:
        Cell cells;
    public:
    };

    class Cell : public Player {
    private:
        char player;
    public:
        explicit Cell(char player) : player(player) {}
        virtual ~Cell() = default;
    };

    enum class Player : char {
        X = 'X', O = 'O'
    };

    class Game {
    private:
        int screenWidth = 512;
        int screenHeight = 512;

        bool quit = false;

        SDL_Window *window{};
        SDL_Renderer *renderer{};
        SDL_Event event{};

        void load_media() {
        }

        void main_loop() {
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&event) != 0) {
                    //User requests quit
                    if (event.type == SDL_QUIT) {
                        quit = true;
                        break;
                    }

                    clear_screen();
                    draw_grid();
                    SDL_RenderPresent(renderer);
                }
            }
        }

        void clear_screen() {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);
        }

        void draw_grid() {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            for (int i = 0; i < 2; ++i) {
                int x = (screenWidth * (1 + i)) / 3;
                int y = (screenHeight * (1 + i)) / 3;

                SDL_RenderDrawLine(renderer, x, 0, x, screenHeight);
                SDL_RenderDrawLine(renderer, 0, y, screenWidth, y);
            }
        }

    public:
        void run() {
            try {
                load_media();
            }
            catch (const std::exception &e) {
                print_exception(e);
            }

            main_loop();
        }

        void init() {
            //Initialize SDL
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                throw_exception("SDL could not initialize!");
            }

            //Create window
            window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      screenWidth, screenHeight, SDL_WINDOW_SHOWN);
            if (!window) {
                throw_exception("Window could not be created!");
            }

            //Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (!renderer) {
                throw_exception("Renderer could not be created!");
            }

            //Initialize renderer color
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                throw_exception("SDL_image could not initialize!", IMG_GetError);
            }
        }

        Game() = default;

        virtual ~Game() {
            //Destroy window
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            window = nullptr;
            renderer = nullptr;

            //Quit SDL subsystems
            IMG_Quit();
            SDL_Quit();
        }
    };
}

int main() {
    t3new::Game game;

    try {
        game.init();
    }
    catch (const std::exception &e) {
        t3new::print_exception(e);
    }

    game.run();

    return 0;
}

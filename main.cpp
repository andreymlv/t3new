#include <iostream>
#include <sstream>

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
        std::stringstream msg;
        msg << s << "SDL Error: " << SDL_ErrorFunction();
        std::throw_with_nested(std::runtime_error(msg.str()));
    }

    class Mouse {

    };

    class Game {
    private:
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 480;

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

                    render();
                }
            }
        }

        void render() {
            //Clear screen
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);

            //Render red filled quad
            SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &fillRect);

            //Render green outlined quad
            SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawRect(renderer, &outlineRect);

            //Draw blue horizontal line
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

            //Draw vertical line of yellow dots
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
            for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
                SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i);
            }

            //Update screen
            SDL_RenderPresent(renderer);
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

            //Set texture filtering to linear
            if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
                std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;
            }

            //Create window
            window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

#include <SDL.h>
#include <SDL_audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int WIDTH = 135;
int HEIGHT = 100;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

int frames_type_one = 4;
int frames_type_two = 7;

int checker_if_corner(int x, int y)
{
    if(x >= -550 && x < 330 && y < 230 && x >= -640)
    {
        return 0;
    }
    return 1;
}
void render_grid(SDL_Renderer *renderer, int x, int y)
{
    int cell_size = 20;

    srand(time(NULL));

    SDL_Rect cell;
    cell.w = cell_size;
    cell.h = cell_size;

    for(int i = 0; i < 50; i++)
    {
        for(int j = 0; j < 50; j++)
        {
            cell.x = x + i * cell_size;
            cell.y = y + j * cell_size;

            int pick = 1 + rand() % 6;

            if(i == 0 || j == 0)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &cell);
            }
            else
            {
                if(pick == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 13, 28, 43, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else if(pick == 2)
                {
                    SDL_SetRenderDrawColor(renderer, 43, 33, 3, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else if(pick == 3)
                {
                    SDL_SetRenderDrawColor(renderer, 12, 25, 11, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else if(pick == 4)
                {
                    SDL_SetRenderDrawColor(renderer, 11, 19, 25, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else if(pick == 5)
                {
                    SDL_SetRenderDrawColor(renderer, 20, 16, 25, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 25, 16, 20, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}
int loadTextureArray(SDL_Renderer* renderer, const char** fileNames, int arraySize, SDL_Texture** textureArray)
{
    for(int i = 0; i < arraySize; i++)
    {
        SDL_Surface* surface = SDL_LoadBMP(fileNames[i]);
        if (surface == NULL)
        {
            printf("Error loading image %d: %s\n", i, SDL_GetError());
            SDL_DestroyRenderer(renderer);
            return 1;
        }
        textureArray[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (textureArray[i] == NULL)
        {
            printf("Error creating texture for image %d: %s\n", i, SDL_GetError());
            SDL_DestroyRenderer(renderer);
            return 1;
        }
    }
    return 0;
}

struct enemies
{
    int health1;
    int health2;
    int health3;
    int health4;

    int for_1;
    int for_2;
    int for_3;
    int for_4;
};

int main(int argc, char* argv[])
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Knight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        printf("Error in creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("Error in creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texture_for_hitting_in_plus[frames_type_two];
    SDL_Texture *texture_for_hitting_in_minus[frames_type_two];
    SDL_Texture *texture_for_standing_in_plus[frames_type_one];
    SDL_Texture *texture_for_standing_in_minus[frames_type_one];
    SDL_Texture *texture_for_standing_in_up[frames_type_one];
    SDL_Texture *texture_for_standing_in_down[frames_type_one];
    SDL_Texture *texture_for_running_in_plus[frames_type_one];
    SDL_Texture *texture_for_running_in_minus[frames_type_one];
    SDL_Texture *texture_for_running_up[frames_type_one];
    SDL_Texture *texture_for_running_down[frames_type_one];
    SDL_Texture *texture_for_hitting_up[frames_type_one];
    SDL_Texture *texture_for_hitting_down[frames_type_one];

    SDL_Texture *bot_standing_in_up[frames_type_one];
    SDL_Texture *bot_standing_in_down[frames_type_one];
    //SDL_Texture *bot_hitting_in_up[];
    //SDL_Texture *bot_hitting_in_down[];

    const char *hitting_in_up[7] = {"up1.bmp", "up_hit3.bmp", "up1.bmp", "up_hit2.bmp","up1.bmp"};
    const char *hitting_in_down[7] = {"down1.bmp", "down_hit2.bmp", "down1.bmp", "down_hit3.bmp", "down1.bmp"};
    const char *running_files_up[4] = {"up1.bmp", "up2.bmp", "up1.bmp", "up3.bmp"};
    const char *running_files_down[4] = {"down1.bmp", "down2.bmp", "down1.bmp", "down3.bmp"};
    const char *hitting_files_in_plus[7] = {"voin1.bmp", "voin2.bmp", "voin3.bmp", "voin4.bmp", "voin3.bmp", "voin2.bmp", "voin1.bmp"};
    const char *hitting_files_in_minus[7] = {"voin1_1.bmp", "voin2_1.bmp", "voin3_1.bmp", "voin4_1.bmp", "voin3_1.bmp", "voin2_1.bmp", "voin1_1.bmp"};
    const char *standing_files_in_plus[4] = {"stand1.bmp", "stand2.bmp", "stand2.bmp", "stand1.bmp"};
    const char *standing_files_in_minus[4] = {"stand2_1.bmp", "stand1_1.bmp", "stand1_1.bmp", "stand2_1.bmp"};
    const char *standing_files_in_up[4] = {"up1.bmp", "up4.bmp", "up4.bmp", "up1.bmp"};
    const char *standing_files_in_down[4] = {"down1.bmp", "down4.bmp", "down4.bmp", "down1.bmp"};
    const char *running_files_in_plus[4] = {"stand1.bmp", "run1.bmp", "stand1.bmp", "run2.bmp"};
    const char *running_files_in_minus[4] = {"stand2_1.bmp", "run1_1.bmp", "stand1_1.bmp", "run1_1.bmp"};

    const char *bot_standing_files_in_down[4] = {"bot_down1.bmp", "bot_down2.bmp", "bot_down1.bmp", "bot_down2.bmp"};
    const char *bot_standing_files_in_up[4] = {"bot_up1.bmp", "bot_up2.bmp", "bot_up1.bmp", "bot_up2.bmp"};

    if(loadTextureArray(renderer, hitting_files_in_plus, frames_type_two, texture_for_hitting_in_plus) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, hitting_files_in_minus, frames_type_two, texture_for_hitting_in_minus) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, standing_files_in_plus, frames_type_one, texture_for_standing_in_plus) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, standing_files_in_minus, frames_type_one, texture_for_standing_in_minus) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, standing_files_in_up, frames_type_one, texture_for_standing_in_up) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, standing_files_in_down, frames_type_one, texture_for_standing_in_down) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, running_files_in_plus, frames_type_one, texture_for_running_in_plus) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, running_files_in_minus, frames_type_one, texture_for_running_in_minus) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, running_files_up, frames_type_one, texture_for_running_up) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, running_files_down, frames_type_one, texture_for_running_down) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, hitting_in_down, frames_type_one, texture_for_hitting_down) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, hitting_in_up, frames_type_one, texture_for_hitting_up) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if(loadTextureArray(renderer, bot_standing_files_in_down, frames_type_one, bot_standing_in_down) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if(loadTextureArray(renderer, bot_standing_files_in_up, frames_type_one, bot_standing_in_up) != 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int current_hit_in_plus = 0;
    int current_hit_in_minus = 0;
    int current_hit_up = 0;
    int current_hit_down = 0;

    int current_stand = 0;
    int current_vert = 0;
    int current_run = 0;
    int current_up = 0;

    int bot_up1 = 0;
    int bot_down1 = 0;
    int bot_up2 = 0;
    int bot_down2 = 0;
    int bot_up3 = 0;
    int bot_down3 = 0;
    int bot_up4 = 0;
    int bot_down4 = 0;

    int running = 1;
    int isSide = 0;
    int isHitting = 0;
    int direction = 0;
    int isUp = 0;
    int type_1 = 0;
    int type_2 = 1;

    SDL_Event event;

    int posX = 350;
    int posY = 250;

    int camera_x = posX - SCREEN_WIDTH / 2;
    int camera_y = posY - SCREEN_HEIGHT / 2;

    struct enemies enemies;
    enemies.health1 = 5;
    enemies.health2 = 5;
    enemies.health3 = 5;
    enemies.health4 = 5;

    int for_bot_1 = rand() % 2;
    int for_bot_2 = rand() % 2;
    int for_bot_3 = rand() % 2;
    int for_bot_4 = rand() % 2;

    int bot_cor_x_1 = 0;
    int bot_cor_y_1 = 0;
    int bot_cor_x_2 = 0;
    int bot_cor_y_2 = 0;
    int bot_cor_x_3 = 0;
    int bot_cor_y_3 = 0;
    int bot_cor_x_4 = 0;
    int bot_cor_y_4 = 0;

    int moving_bot_1 = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_q)
            {
                running = 0;
            }
            else if (event.key.keysym.sym == SDLK_SPACE)
            {
                isHitting = 1;
                
                if(type_1 == 1)
                {
                    if(type_2 == 1)
                    {
                        SDL_Rect rect_hit = {posX, posY, WIDTH, HEIGHT};
                        SDL_RenderCopy(renderer, texture_for_hitting_up[current_hit_up % frames_type_two], NULL, &rect_hit);
                        current_hit_up = (current_hit_up + 1) % frames_type_two;
                    }
                    else
                    {
                        SDL_Rect rect_hit = {posX, posY, WIDTH, HEIGHT};
                        SDL_RenderCopy(renderer, texture_for_hitting_down[current_hit_down % frames_type_two], NULL, &rect_hit);
                        current_hit_down = (current_hit_down + 1) % frames_type_two;
                    }
                }
                else
                {
                    if(type_2 == 1)
                    {
                        SDL_Rect rect_hit = {posX, posY, WIDTH, HEIGHT};
                        SDL_RenderCopy(renderer, texture_for_hitting_in_plus[current_hit_in_plus % frames_type_two], NULL, &rect_hit);
                        current_hit_in_plus = (current_hit_in_plus + 1) % frames_type_two;
                    }
                    else
                    {
                        SDL_Rect rect_hit = {posX, posY, WIDTH, HEIGHT};
                        SDL_RenderCopy(renderer, texture_for_hitting_in_minus[current_hit_in_minus % frames_type_two], NULL, &rect_hit);
                        current_hit_in_minus = (current_hit_in_minus + 1) % frames_type_two;
                    }
                }
            }
            else if (event.key.keysym.sym == SDLK_d)
            {
                isSide = 1;
                isUp = 0;
                direction = 0;

                type_1 = 0;
                type_2 = 1;

                SDL_Rect rect_run = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_in_plus[current_run % frames_type_one], NULL, &rect_run);
                current_run = (current_run + 1) % frames_type_one;

                if(camera_x >= -550)
                {
                    camera_x -= 10;
                }
            }
            else if(event.key.keysym.sym == SDLK_a)
            {
                isSide = 1;
                isUp = 0;
                direction = 1;

                type_1 = 0;
                type_2 = 0;

                SDL_Rect rect_run = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_in_minus[current_run % frames_type_one], NULL, &rect_run);
                current_run = (current_run + 1) % frames_type_one;
                if(camera_x < 330)
                {
                    camera_x += 10;
                }
            }
            else if(event.key.keysym.sym == SDLK_w)
            {
                isSide = 0;
                isUp = 1;
                direction = 0;

                type_1 = 1;
                type_2 = 1;

                SDL_Rect rect_run = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_up[current_up % frames_type_one], NULL, &rect_run);
                current_up = (current_up + 1) % frames_type_one;
                if(camera_y < 230)
                {
                    camera_y += 10;
                }
            }
            else if(event.key.keysym.sym == SDLK_s)
            {
                isSide = 0;
                isUp = 1;
                direction = 1;

                type_1 = 1;
                type_2 = 0;

                SDL_Rect rect_run = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_down[current_up % frames_type_one], NULL, &rect_run);
                current_up = (current_up + 1) % frames_type_one;
                if(camera_y >= -640)
                {
                    camera_y -= 10;
                }
            }
        }

        SDL_RenderClear(renderer);
        render_grid(renderer, camera_x, camera_y);

        moving_bot_1++;
        int speed = 25;
        if(moving_bot_1 == 5)
        {
            int type1 = rand() % 4;
            if(type1 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
            }
            if(type1 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
            }
            if(type1 == 2)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
            }
            if(type1 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
            }

            int type2 = rand() % 4;
            if(type2 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
            }
            if(type2 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
            }
            if(type2 == 2)
            {
                if(checker_if_corner(camera_x + + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
            }
            if(type2 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
            }

            int type3 = rand() % 4;
            if(type3 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
            }
            if(type3 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
            }
            if(type3 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
            }
            if(type3 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
            }

            int type4 = rand() % 2;
            if(type4 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
            }
            if(type4 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
            }
            if(type4 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
            }
            if(type4 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
            }
        }
        if(moving_bot_1 == 10)
        {
            int type1 = rand() % 4;
            if(type1 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
            }
            if(type1 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
            }
            if(type1 == 2)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
            }
            if(type1 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
            }

            int type2 = rand() % 4;
            if(type2 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
            }
            if(type2 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
            }
            if(type2 == 2)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
            }
            if(type2 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
            }

            int type3 = rand() % 4;
            if(type3 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
            }
            if(type3 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
            }
            if(type3 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
            }
            if(type3 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
            }

            int type4 = rand() % 2;
            if(type4 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
            }
            if(type4 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
            }
            if(type4 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
            }
            if(type4 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
            }
        }
        if(moving_bot_1 == 15)
        {
            int type1 = rand() % 4;
            if(type1 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
            }
            if(type1 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
            }
            if(type1 == 2)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
            }
            if(type1 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
            }

            int type2 = rand() % 4;
            if(type2 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
            }
            if(type2 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
            }
            if(type2 == 2)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
            }
            if(type2 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
            }

            int type3 = rand() % 4;
            if(type3 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
            }
            if(type3 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
            }
            if(type3 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
            }
            if(type3 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
            }

            int type4 = rand() % 2;
            if(type4 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
            }
            if(type4 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
            }
            if(type4 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
            }
            if(type4 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
            }
        }
        if(moving_bot_1 == 20)
        {
            int type1 = rand() % 4;
            if(type1 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
            }
            if(type1 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
            }
            if(type1 == 2)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_x_1 = bot_cor_x_1 - speed;
                }
                else
                {
                    bot_cor_x_1 = bot_cor_x_1 + speed;
                }
            }
            if(type1 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1))
                {
                    bot_cor_y_1 = bot_cor_y_1 - speed;
                }
                else
                {
                    bot_cor_y_1 = bot_cor_y_1 + speed;
                }
            }

            int type2 = rand() % 4;
            if(type2 == 0)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
            }
            if(type2 == 1)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
            }
            if(type2 == 2)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_x_2 = bot_cor_x_2 + speed;
                }
                else
                {
                    bot_cor_x_2 = bot_cor_x_2 - speed;
                }
            }
            if(type2 == 3)
            {
                if(checker_if_corner(camera_x + 100 + bot_cor_x_2, camera_y + 600 + bot_cor_y_2))
                {
                    bot_cor_y_2 = bot_cor_y_2 + speed;
                }
                else
                {
                    bot_cor_y_2 = bot_cor_y_2 - speed;
                }
            }

            int type3 = rand() % 4;
            if(type3 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
            }
            if(type3 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
            }
            if(type3 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_y_3 = bot_cor_y_3 - speed;
                }
                else
                {
                    bot_cor_y_3 = bot_cor_y_3 + speed;
                }
            }
            if(type3 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3))
                {
                    bot_cor_x_3 = bot_cor_x_3 - speed;
                }
                else
                {
                    bot_cor_x_3 = bot_cor_x_3 + speed;
                }
            }

            int type4 = rand() % 2;
            if(type4 == 0)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
            }
            if(type4 == 1)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
            }
            if(type4 == 2)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_x_4 = bot_cor_x_4 + speed;
                }
                else
                {
                    bot_cor_x_4 = bot_cor_x_4 - speed;
                }
            }
            if(type4 == 3)
            {
                if(checker_if_corner(camera_x + 600 + bot_cor_x_4, camera_y + 600 + bot_cor_y_4))
                {
                    bot_cor_y_4 = bot_cor_y_4 + speed;
                }
                else
                {
                    bot_cor_y_4 = bot_cor_y_4 - speed;
                }
            }
            moving_bot_1 = 0;
        }


        SDL_Rect enemy_1 = {camera_x + 100 + bot_cor_x_1, camera_y + 100 + bot_cor_y_1, 100, 100};
        if(enemies.health1 >= 0)
        {
            if(for_bot_1 == 1)
            {
                SDL_RenderCopy(renderer, bot_standing_in_down[bot_down1 % frames_type_one], NULL, &enemy_1);
            }
            else
            {
                SDL_RenderCopy(renderer, bot_standing_in_up[bot_up1 % frames_type_one], NULL, &enemy_1);
            }
        }

        SDL_Rect enemy_2 = {camera_x + 100 + bot_cor_x_2, camera_y + 800 + bot_cor_y_2, 100, 100};
        if(enemies.health2 >= 0)
        {
            if(for_bot_2 == 1)
            {
                SDL_RenderCopy(renderer, bot_standing_in_down[bot_down2 % frames_type_one], NULL, &enemy_2);
            }
            else
            {
                SDL_RenderCopy(renderer, bot_standing_in_up[bot_up2 % frames_type_one], NULL, &enemy_2);
            }
        }

        SDL_Rect enemy_3 = {camera_x + 800 + bot_cor_x_3, camera_y + 100 + bot_cor_y_3, 100, 100};
        if(enemies.health3 >= 0)
        {
            if(for_bot_3 == 1)
            {
                SDL_RenderCopy(renderer, bot_standing_in_down[bot_down3 % frames_type_one], NULL, &enemy_3);
            }
            else
            {
                SDL_RenderCopy(renderer, bot_standing_in_up[bot_up3 % frames_type_one], NULL, &enemy_3);
            }
        }

        SDL_Rect enemy_4 = {camera_x + 800 + bot_cor_x_4, camera_y + 800 + bot_cor_y_4, 100, 100};
        if(enemies.health4 >= 0)
        {
            if(for_bot_4 == 1)
            {
                SDL_RenderCopy(renderer, bot_standing_in_down[bot_down4 % frames_type_one], NULL, &enemy_4);
            }
            else
            {
                SDL_RenderCopy(renderer, bot_standing_in_up[bot_up4 % frames_type_one], NULL, &enemy_4);
            }
        }

        if (isSide)
        {
            if(direction == 1)
            {
                SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_in_minus[current_run % frames_type_one], NULL, &rect_render);
            }
            else
            {
                SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_in_plus[current_run % frames_type_one], NULL, &rect_render);
            }
            isSide = 0;
        }
        else if (isHitting)
        {
            if(type_1 == 1)
            {
                if(type_2 == 1) // удар вверх
                {
                    SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_hitting_up[current_hit_up % frames_type_one], NULL, &rect_render);
                }
                else // удар вниз
                {
                    SDL_Rect rect_render = {posX - 3, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_hitting_down[current_hit_down % frames_type_one], NULL, &rect_render);
                }
            }
            else
            {
                if(type_2 == 1) // удар влево
                {
                    SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_hitting_in_plus[current_hit_in_plus % frames_type_two], NULL, &rect_render);
                }
                else // удар вправо
                {
                    SDL_Rect rect_render = {posX - 42, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_hitting_in_minus[current_hit_in_minus % frames_type_two], NULL, &rect_render);
                }
            }
            isHitting = 0;
        }
        else if(isUp)
        {
            if(direction == 0)
            {
                SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_up[current_up % frames_type_one], NULL, &rect_render);
            }
            else
            {
                SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                SDL_RenderCopy(renderer, texture_for_running_down[current_up % frames_type_one], NULL, &rect_render);
            }
            isUp = 0;
        }
        else
        {
            if(type_1 == 1)
            {
                if(type_2 == 1)
                {
                    SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_standing_in_up[current_vert % frames_type_one], NULL, &rect_render);
                }
                else
                {
                    SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_standing_in_down[current_vert % frames_type_one], NULL, &rect_render);
                }
            }
            else
            {
                if(type_2 == 1)
                {
                    SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_standing_in_plus[current_stand % frames_type_one], NULL, &rect_render);
                }
                else
                {
                    SDL_Rect rect_render = {posX, posY, WIDTH, HEIGHT};
                    SDL_RenderCopy(renderer, texture_for_standing_in_minus[current_stand % frames_type_one], NULL, &rect_render);
                }
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(100);

        current_stand = (current_stand + 1) % 4;
        current_vert = (current_vert + 1) % 4;

        bot_down1 = (bot_down1 + 1) % 4;
        bot_up1 = (bot_up1 + 1) % 4;

        bot_down2 = (bot_down2 + 1) % 4;
        bot_up2 = (bot_up2 + 1) % 4;

        bot_down3 = (bot_down3 + 1) % 4;
        bot_up3 = (bot_up3 + 1) % 4;

        bot_down4 = (bot_down4 + 1) % 4;
        bot_up4 = (bot_up4 + 1) % 4;
    }

    for (int i = 0; i < frames_type_two; i++)
    {
        SDL_DestroyTexture(texture_for_hitting_in_plus[i]);
        SDL_DestroyTexture(texture_for_hitting_in_minus[i]);
        SDL_DestroyTexture(texture_for_hitting_up[i]);
        SDL_DestroyTexture(texture_for_hitting_down[i]);
    }
    for (int i = 0; i < frames_type_one; i++)
    {
        SDL_DestroyTexture(bot_standing_in_up[i]);
        SDL_DestroyTexture(bot_standing_in_down[i]);
        SDL_DestroyTexture(texture_for_standing_in_plus[i]);
        SDL_DestroyTexture(texture_for_standing_in_minus[i]);
        SDL_DestroyTexture(texture_for_running_in_plus[i]);
        SDL_DestroyTexture(texture_for_running_in_minus[i]);
        SDL_DestroyTexture(texture_for_running_up[i]);
        SDL_DestroyTexture(texture_for_running_down[i]);
        SDL_DestroyTexture(texture_for_standing_in_up[i]);
        SDL_DestroyTexture(texture_for_standing_in_down[i]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

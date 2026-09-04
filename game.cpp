//doing this so vscode intellisense stops showing errors in this file since i'm doing a unity build
#ifndef SDL_WINDOW_HPP
#include "sdl_window.hpp"
#include "platform_common.cpp"
#include "renderer.cpp"

#endif

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!(input->buttons[b].is_down) && input->buttons[b].changed)

void reset_arena();
void check_arena_collision();
void check_ball_to_paddle_collision();

const int PLAYER_POSITION_X = 80;

float player_1_p = 0.f, player_1_vel = 0.f;
float player_2_p = 0.f, player_2_vel = 0.f;
float player_1_accel= 0.f, player_2_accel = 0.f;
float damping_factor = 10.f;

float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;

float ball_p_x, ball_p_y, ball_vel_x {100}, ball_vel_y = 0.f, ball_half_size=1.f;

void simulate_game(INPUT *input, float dt){
    clear_screen(0xff5500);

    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);
    
    player_1_accel = 0.f;
    if(is_down(BUTTON_UP)) player_1_accel += 2000;
    if(is_down(BUTTON_DOWN)) player_1_accel -= 2000;
    
    player_1_accel -= player_1_vel * damping_factor;
    
    player_1_p = player_1_p + (player_1_vel * dt) + (player_1_accel * dt * dt * .5f);
    player_1_vel = player_1_vel + player_1_accel * dt;
    
    // Player 2 - W/S
    player_2_accel = 0.f;
    if(is_down(BUTTON_W)) player_2_accel += 2000;
    if(is_down(BUTTON_S)) player_2_accel -= 2000;

    player_2_accel -= player_2_vel * damping_factor;

    player_2_p = player_2_p + (player_2_vel * dt) + (player_2_accel * dt * dt * .5f);
    player_2_vel = player_2_vel + player_2_accel * dt;

    ball_p_x += ball_vel_x * dt;
    ball_p_y += ball_vel_y * dt;
    
    // draw_rect(player_pos_x, player_pos_y, 1, 1, 0x00ff22);
    draw_rect(ball_p_x, ball_p_y, 1, 1, 0xffffff);

    check_arena_collision();
    check_ball_to_paddle_collision();

    draw_rect(PLAYER_POSITION_X, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);
    draw_rect(-PLAYER_POSITION_X, player_2_p, player_half_size_x, player_half_size_y, 0xff0000);
}

void check_ball_to_paddle_collision()
{
    if (ball_p_x + ball_half_size > PLAYER_POSITION_X - player_half_size_x &&
        ball_p_x - ball_half_size < PLAYER_POSITION_X + player_half_size_x &&
        ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
        ball_p_y - ball_half_size < player_1_p + player_half_size_y)
    {
        ball_p_x = PLAYER_POSITION_X - player_half_size_x - ball_half_size;
        ball_vel_x *= -1;
        ball_vel_y = (ball_vel_y - player_1_p)*2 + player_1_vel * 0.3;
    }
    else if (ball_p_x - ball_half_size < -PLAYER_POSITION_X + player_half_size_x &&
        ball_p_x + ball_half_size > -PLAYER_POSITION_X - player_half_size_x &&
        ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
        ball_p_y - ball_half_size < player_2_p + player_half_size_y)
        {
            ball_p_x = -PLAYER_POSITION_X + player_half_size_x + ball_half_size;
            ball_vel_x *= -1;
            ball_vel_y = (ball_vel_y - player_2_p)*2 + player_2_vel * 0.3;
    }
}

void check_arena_collision()
{
    if (player_1_p + player_half_size_y > arena_half_size_y)
    {
        player_1_p = arena_half_size_y - player_half_size_y;
        player_1_vel = 0.f;
    }
    else if (player_1_p - player_half_size_y < -arena_half_size_y)
    {
        player_1_p = -arena_half_size_y + player_half_size_y;
        player_1_vel = 0.f;
    }
    if (player_2_p + player_half_size_y > arena_half_size_y)
    {
        player_2_p = arena_half_size_y - player_half_size_y;
        player_2_vel = 0.f;
    }

    else if (player_2_p - player_half_size_y < -arena_half_size_y)
    {
        player_2_p = -arena_half_size_y + player_half_size_y;
        player_2_vel = 0.f;
    }

    if(ball_p_y + ball_half_size > arena_half_size_y){
        ball_p_y = arena_half_size_y-ball_half_size;
        ball_vel_y *= -1;
    }else if(ball_p_y - ball_half_size < -arena_half_size_y){
        ball_p_y = -arena_half_size_y+ball_half_size;
        ball_vel_y *= -1;
    }

    if(ball_p_x + ball_half_size > arena_half_size_x || ball_p_x - ball_half_size < -arena_half_size_x){
        reset_arena();
    }
}

void reset_arena(){
    ball_p_x = ball_p_y = player_1_p = player_2_p = player_1_vel = player_2_vel = player_1_accel = player_2_accel = ball_vel_y = 0.f;
    ball_vel_x = 100.f;
}

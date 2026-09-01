#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!(input->buttons[b].is_down) && input->buttons[b].changed)

float player_1_p = 0.f, player_1_vel = 0.f;
float player_2_p = 0.f, player_2_vel = 0.f;
float damping_factor = 10.f;

void simulate_game(INPUT *input, float dt){
    clear_screen(0xff5500);

    draw_rect(0, 0, 85, 45, 0xffaa33);
    
    // Player 1 - arrow keys
    float player_1_accel = 0.f;
    if(is_down(BUTTON_UP)) player_1_accel += 2000;
    if(is_down(BUTTON_DOWN)) player_1_accel -= 2000;

    player_1_accel -= player_1_vel * damping_factor;

    player_1_p = player_1_p + (player_1_vel * dt) + (player_1_accel * dt * dt * .5f);
    player_1_vel = player_1_vel + player_1_accel * dt;

    // Player 2 - W/S
    float player_2_accel = 0.f;
    if(is_down(BUTTON_W)) player_2_accel += 2000;
    if(is_down(BUTTON_S)) player_2_accel -= 2000;

    player_2_accel -= player_2_vel * damping_factor;

    player_2_p = player_2_p + (player_2_vel * dt) + (player_2_accel * dt * dt * .5f);
    player_2_vel = player_2_vel + player_2_accel * dt;
    
    // draw_rect(player_pos_x, player_pos_y, 1, 1, 0x00ff22);
    draw_rect(0, 0, 1, 1, 0xffffff);

    draw_rect(80, player_1_p, 2.5, 12, 0xff0000);
    draw_rect(-80, player_2_p, 2.5, 12, 0xff0000);
}

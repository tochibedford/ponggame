#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!(input->buttons[b].is_down) && input->buttons[b].changed)

float player_p, player_vel = 0.f;
float damping_factor = 10.f;

void simulate_game(INPUT *input, float dt){
    clear_screen(0xff5500);

    draw_rect(0, 0, 85, 45, 0xffaa33);
    
    float player_accel = 0.f;
    if(is_down(BUTTON_UP)) player_accel += 2000;
    if(is_down(BUTTON_DOWN)) player_accel -= 2000;

    player_accel -= player_vel * damping_factor;

    player_p = player_p + (player_vel * dt) + (player_accel * dt * dt * .5f);
    player_vel =  player_vel + player_accel * dt;
    
    // draw_rect(player_pos_x, player_pos_y, 1, 1, 0x00ff22);
    draw_rect(0, 0, 1, 1, 0xffffff);

    draw_rect(80, player_p, 2.5, 12, 0xff0000);
    draw_rect(-80, 0, 2.5, 12, 0xff0000);
}

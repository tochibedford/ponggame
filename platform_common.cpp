struct Button_State{
    bool is_down, changed;
};

enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,

    BUTTON_COUNT
};

struct INPUT {
    Button_State buttons[BUTTON_COUNT];
};

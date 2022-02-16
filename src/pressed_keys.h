#pragma once

enum KeyPressState {
    KEY_PRESS_STATE_ACTIVE,
    KEY_PRESS_STATE_BACKGROUND,
    KEY_PRESS_STATE_INACTIVE
};

struct PressedKeys {
    enum KeyPressState w;
    enum KeyPressState s;
    enum KeyPressState up;
    enum KeyPressState down;
};

#pragma once

enum class EventType {
    Key,
    Mouse,
};

enum class KeyEventType {
    Press,
    Release,
};

enum class MouseEventType {
    Move,
    ButtonPress,
    ButtonRelease,
    Wheel,
    HorizontalWheel,
};

struct Event {
    EventType type;
    union {
        struct {
            KeyEventType type;
            int key_code;
        } key;
        struct {
            MouseEventType type;
            int x, y;
            int button;
            short wheel_delta;
        } mouse;
    };
};

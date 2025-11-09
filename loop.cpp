#include "loop.h"

#include <windows.h>
#include <conio.h>
#include "screen.h"
#include "object.h"
#include "log.h"

constexpr uint32_t kMaxFrameCount = UINT_MAX;
constexpr double kFpsUpdateUnit = 10.0;
constexpr double kClockPerSec = static_cast<double>(CLOCKS_PER_SEC);
constexpr int kEventBufferSize = 32;

HANDLE input_handle;
INPUT_RECORD input_record[kEventBufferSize];

void Loop::Initialize() {
    screen.Clear();
    
    // * Initialize Random Seed
    srand(static_cast<uint32_t>(time(NULL)));

    // * Initialize Screen Buffer Size
    setvbuf(stdout, NULL, _IOFBF, screen.area);

    // * Enable Mouse Mode
    input_handle = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(input_handle, (ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

    // * Hide Cursor
    printf("\x1b[?25l");

    // * Initialize Loop Variables
    _current_clock = _last_clock = _clock_for_fps = clock();
    frame_count = _frame_count_for_fps = 0;
    fps = 0;
    _is_running = true;
    root_object = new Object();
    ObjectRenderer::TryAddObject(root_object);

    OnInitialize();
}

void Loop::Update() {
    UpdateTick();
    UpdateEvent();
    if (IsKeyDown(VK_F4)) {
        Log::is_visible = !Log::is_visible;
    }
    OnUpdate(_delta_time);
    frame_count++;
    if (frame_count >= kMaxFrameCount)
        frame_count = 0;
}

void Loop::UpdateTick() {
    _current_clock = clock();
    _delta_time = (_current_clock - _last_clock) / kClockPerSec;
    if (++_frame_count_for_fps >= kFpsUpdateUnit) {
        fps = kFpsUpdateUnit * kClockPerSec / (_current_clock - _clock_for_fps);
        _frame_count_for_fps = 0;
        _clock_for_fps = _current_clock;
    }
    _last_clock = _current_clock;
}

void Loop::UpdateEvent() {
    event_queue = {};
    for (int i = 0; i < 256; i++) {
        is_key_up[i] = false;
        if (is_key_down[i]) {
            is_key_down[i] = false;
            is_key_pressed[i] = true;
        }
    }
    DWORD event_count = 0;
    GetNumberOfConsoleInputEvents(input_handle, &event_count);
    if (event_count > 0) {
        DWORD events_read = 0;
        ReadConsoleInput(input_handle, input_record, kEventBufferSize, &events_read);
        for (DWORD i = 0; i < events_read; i++) {
            switch (input_record[i].EventType) {
                case KEY_EVENT: {
                    const KEY_EVENT_RECORD& key_event = input_record[i].Event.KeyEvent;
                    Event event;
                    event.type = EventType::Key;
                    event.key.type = key_event.bKeyDown ? KeyEventType::Press : KeyEventType::Release;
                    event.key.key_code = key_event.wVirtualKeyCode;
                    event_queue.push(event);
                    if (key_event.bKeyDown) {
                        is_key_down[key_event.wVirtualKeyCode] = true;
                    }
                    else {
                        is_key_down[key_event.wVirtualKeyCode] = false;
                        is_key_pressed[key_event.wVirtualKeyCode] = false;
                        is_key_up[key_event.wVirtualKeyCode] = true;
                    }
                    break;
                }
                case MOUSE_EVENT: {
                    const MOUSE_EVENT_RECORD& mouse_event = input_record[i].Event.MouseEvent;
                    Event event;
                    event.type = EventType::Mouse;
                    switch (mouse_event.dwEventFlags) {
                        case MOUSE_MOVED:
                            event.mouse.type = MouseEventType::Move;
                            break;
                        case 0:
                            if (mouse_event.dwButtonState != 0)
                                event.mouse.type = MouseEventType::ButtonPress;
                            else 
                                event.mouse.type = MouseEventType::ButtonRelease;
                            break;
                        case MOUSE_WHEELED:
                            event.mouse.type = MouseEventType::HorizontalWheel;
                            break;
                    }
                    event.mouse.x = mouse_event.dwMousePosition.X;
                    event.mouse.y = mouse_event.dwMousePosition.Y;
                    event.mouse.button = mouse_event.dwButtonState;
                    event.mouse.wheel_delta = static_cast<short>(HIWORD(mouse_event.dwButtonState));
                    event_queue.push(event);
                    mouse_x = mouse_event.dwMousePosition.X;
                    mouse_y = mouse_event.dwMousePosition.Y;
                    break;
                }
            }
        }
    }
}

void Loop::Render() {
    screen.ReturnCursor();
    graphic.Clear();

    graphic.Save();
    ObjectRenderer::Render(graphic);
    graphic.Restore();
    graphic.Save();
    OnRender(graphic);
    graphic.Restore();

    if (is_fps_visible) {
        graphic.DrawTextWithFormat(0, 0, "frame: %lld, delta time: %lg", frame_count, _delta_time);
        graphic.DrawTextWithFormat(0, 1, "fps: %lg", fps);
    }

    if (Log::is_visible) {
        Log::Render(graphic);
    }

    graphic.Render();
}

void Loop::Dispose() {
    delete root_object;

    OnDispose();

    printf("\x1b[0m"); // reset color
    printf("\x1b[?25h"); // show cursor
}

Loop::Loop(Screen& screen)
    : screen(screen), graphic(screen) {
}

void Loop::Run() {
    Initialize();

    while (_is_running) {
        Update(); if (!_is_running) break;
        Render();
    }

    Dispose();
}

bool Loop::IsKeyDown(int key_code) {
    if (key_code < 0 || key_code >= 256) { return false; }
    return is_key_down[key_code];
}

bool Loop::IsKeyPressed(int key_code) {
    if (key_code < 0 || key_code >= 256) { return false; }
    return is_key_pressed[key_code];
}

bool Loop::IsKeyUp(int key_code) {
    if (key_code < 0 || key_code >= 256) { return false; }
    return is_key_up[key_code];
}

bool Loop::PollEvent(Event& out_event) {
    if (event_queue.empty()) { return false; }
    out_event = event_queue.front();
    event_queue.pop();
    return true;
}

void Loop::Quit() { _is_running = false; }
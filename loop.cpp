#include "loop.h"

#include <windows.h>
#include <conio.h>

#define MAX_FRAME_COUNT UINT_MAX
#define FPS_UPDATE_UNIT 10

void Loop::Initialize(Screen* screen) {
    _screen = screen;
    _screen->ClearScreen();

    srand(time(NULL)); // initialize random seed
    setvbuf(stdout, NULL, _IOFBF, _screen->area); // initialize screen buffer size
    printf("\x1b[?25l"); // hide cursor
    _current_clock = _last_clock = _clock_for_fps = clock();
    frame_count = _frame_count_for_fps = 0;
    fps = 0;
    _is_running = true;

    OnInitialize();
}

void Loop::Update() {
    UpdateTick();
    UpdateInput();
    OnUpdate(_delta_time);
    frame_count++;
    if (frame_count >= MAX_FRAME_COUNT)
        frame_count = 0;
}

void Loop::UpdateTick() {
    _current_clock = clock();
    _delta_time = (_current_clock - _last_clock) / (double)CLOCKS_PER_SEC;
    if (++_frame_count_for_fps >= FPS_UPDATE_UNIT) {
        fps = FPS_UPDATE_UNIT * CLOCKS_PER_SEC / (float)(_current_clock - _clock_for_fps);
        _frame_count_for_fps = 0;
        _clock_for_fps = _current_clock;
    }
    _last_clock = _current_clock;
}

void Loop::UpdateInput() {
    memset(keystate, 0, 256);
    if (_kbhit()) {
        char keycode = _getch();
        keystate[keycode] = 1;
    }
}

void Loop::Render() {
    _screen->ReturnCursor();
    _screen->ClearBuffer();

    _screen->SaveContext();
    OnRender();
    _screen->RestoreContext();

    if (is_fps_visible) {
        _screen->DrawTextWithFormat(0, 0, "frame: %d, delta time: %lg", frame_count, _delta_time);
        _screen->DrawTextWithFormat(0, 1, "fps: %f", fps);
    }

    _screen->RenderBuffer();
}

void Loop::Dispose() {
    OnDispose();

    printf("\x1b[0m"); // reset color
    printf("\x1b[?25h"); // show cursor

    // _screen->ClearScreen();
    // _screen->ReturnCursor();
    _screen = nullptr;
}

void Loop::Start(Screen* screen) {
    Initialize(screen);
    
    while (_is_running) {
        Update(); if (!_is_running) break;
        Render();
#ifdef LIMIT_FPS
        Sleep(LIMIT_FPS);
#endif
    }

    Dispose();
}

void Loop::Quit() { _is_running = false; }
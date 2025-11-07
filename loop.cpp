#include "loop.h"

#include <windows.h>
#include <conio.h>

constexpr uint32_t kMaxFrameCount = UINT_MAX;
constexpr double kFpsUpdateUnit = 10.0;
constexpr double kClockPerSec = static_cast<double>(CLOCKS_PER_SEC);

void Loop::Initialize() {
    _screen.Clear();

    srand(static_cast<uint32_t>(time(NULL))); // initialize random seed
    setvbuf(stdout, NULL, _IOFBF, _screen.area); // initialize screen buffer size
    printf("\x1b[?25l"); // hide cursor
    _current_clock = _last_clock = _clock_for_fps = clock();
    frame_count = _frame_count_for_fps = 0;
    fps = 0;
    _is_running = true;

    OnInitialize();
}

void Loop::Update() {
    UpdateTick();
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

void Loop::Render() {
    _screen.ReturnCursor();
    _graphic.Clear();

    _graphic.Save();
    OnRender(_graphic);
    _graphic.Restore();

    if (is_fps_visible) {
        _graphic.DrawTextWithFormat(0, 0, "frame: %lld, delta time: %lg", frame_count, _delta_time);
        _graphic.DrawTextWithFormat(0, 1, "fps: %lg", fps);
    }

    _graphic.Render();
}

void Loop::Dispose() {
    OnDispose();

    printf("\x1b[0m"); // reset color
    printf("\x1b[?25h"); // show cursor
}

Loop::Loop(Screen& screen)
    : _screen(screen), _graphic(screen) {
}

void Loop::Run() {
    Initialize();

    while (_is_running) {
        Update(); if (!_is_running) break;
        Render();
    }

    Dispose();
}

void Loop::Quit() { _is_running = false; }
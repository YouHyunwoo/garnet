#pragma once

#include <time.h>
#include <queue>
#include "graphic.h"
#include "event.h"

class Screen;
class Object;

class Loop {
private:
    bool _is_running = false;
    clock_t _current_clock, _last_clock;
    double _delta_time; // seconds
    int _frame_count_for_fps;
    clock_t _clock_for_fps;
    void Initialize();
    void Update();
    void UpdateTick();
    void UpdateEvent();
    void Render();
    void Dispose();
protected:
    Object* root_object = nullptr;
    Screen& screen;
    Graphic graphic;
    std::queue<Event> event_queue;
    bool is_key_down[256] = {};
    bool is_key_pressed[256] = {};
    bool is_key_up[256] = {};
    int mouse_x, mouse_y;
    virtual void OnInitialize() {}
    virtual void OnUpdate(double delta_time) {}
    virtual void OnRender(Graphic& graphic) {}
    virtual void OnDispose() {}
    public:
    uint64_t frame_count;
    double fps;
    bool is_fps_visible = true;
    Loop(Screen &screen);
    void Run();
    bool IsKeyDown(int key_code);
    bool IsKeyPressed(int key_code);
    bool IsKeyUp(int key_code);
    bool PollEvent(Event& out_event);
    void Quit();
};

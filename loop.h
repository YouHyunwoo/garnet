#pragma once

#include <time.h>
#include "screen.h"
#include "graphic.h"

class Object;

class Loop {
private:
    bool _is_running = false;
    clock_t _current_clock, _last_clock;
    double _delta_time; // sec
    int _frame_count_for_fps;
    clock_t _clock_for_fps;
    void Initialize();
    void Update();
    void UpdateTick();
    void Render();
    void Dispose();
protected:
    Object* _root_object = nullptr;
    Screen& _screen;
    Graphic _graphic;
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
    void Quit();
};

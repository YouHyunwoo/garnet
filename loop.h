#ifndef _LOOP_H_
#define _LOOP_H_

#include <time.h>
#include "screen.h"

class Loop {
private:
    Screen* _screen;
    bool _is_running = false;
    clock_t _current_clock, _last_clock;
    double _delta_time; // sec
    int _frame_count_for_fps;
    clock_t _clock_for_fps;
    
    void Initialize(Screen* screen);
    void Update();
    void UpdateTick();
    void UpdateInput();
    void Render();
    void Dispose();
    
protected:
    virtual void OnInitialize() {}
    virtual void OnUpdate(double delta_time) {}
    virtual void OnRender() {}
    virtual void OnDispose() {}
    
public:
    uint64_t frame_count;
    float fps;
    bool is_fps_visible = true;
    char keystate[256];

    void Start(Screen* screen);
    void Quit();
};

#endif
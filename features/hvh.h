#pragma once
#include "../globals.h"

class hvh : public c_singleton<hvh> {
public:
    void AntiAim();
public:
    void pitch();
    float attarget();
    void preparefake();
    void yaw();
    void adjustyaw();
    void lby_update(float sampletime, c_user_cmd* ucmd, bool& sendpacket);
public:
    void sendpacket(bool& send);
    void fakeduck();
    float corrected_tickbase();
    bool  m_in_duck;
};
#define g_hvh hvh::instance()
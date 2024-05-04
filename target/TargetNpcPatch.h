#pragma once

extern "C" void decorator(void);
extern "C" {
    void setStructure(intptr_t value);
}

extern "C" void bullet_decorator(void);
extern "C" {
    void setBulletStructure(intptr_t value);
}

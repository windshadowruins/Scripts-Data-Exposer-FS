#include "pch.h"

#include "bullet/BulletLog.h"

extern BulletLog* bulletLog;
extern "C" {
    void setBulletStructure(intptr_t value) {
        bulletLog->add(BulletInfo(value));
    }
}

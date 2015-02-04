#ifndef __INITIALIZER_H__
#define __INITIALIZER_H__

#include "nexaconfigstore.h"

class Initializer {
private:
    NexaConfigStore* _nexaConfigStore;

    public:

    Initializer(NexaConfigStore* nexaConfigStore) {
        _nexaConfigStore = nexaConfigStore;
    }

    void init();
    void configure();

};

#endif

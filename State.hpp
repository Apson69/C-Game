

#pragma once

namespace CPP {
    class State {
    public:

        virtual void Update()=0;
        virtual void Pause(){};
        virtual void Resume(){};
    };
}

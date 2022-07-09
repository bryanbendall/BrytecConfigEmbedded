#pragma once

#include "NodesVector.h"
#include <stdint.h>

float getValueFromBoard(int index)
{
    switch (index) {
    case 1:
        // return DDRB |= 1 << 3;
        break;
    }

    return 0;
}

void setOutputToBoard(int index)
{
    switch (index) {
    case 1:
        // DDRB &= 1 << PINB3;
        break;
    }
}

struct EBrytecNodeGroup {
    uint32_t startNodeIndex;
    uint8_t nodeCount;
    uint8_t boardIndex;

    void updateInitialValue() { getValueFromBoard(boardIndex); }
    void updateFinalValue() { setOutputToBoard(boardIndex); }
};

class EBrytecApp {

public:
    EBrytecApp();

    void deserializeModule(/*serialized data*/);

    void update(float timestep);

private:
    Embedded::NodesVector m_nodeVector;
    EBrytecNodeGroup* m_nodeGroups;
    uint32_t m_nodeGroupsCount = 0;
};

void main()
{

    // start up stuff

    EBrytecApp app;
    app.deserializeModule();

    while (1) {

        // board stuff

        // read physical pins

        app.update(2.0f);

        // write physical pins
    }
}
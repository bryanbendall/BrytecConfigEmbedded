#include "EBrytecApp.h"

#include "Nodes/EFinalValueNode.h"
#include "Nodes/EInitialValueNode.h"
#include "Utils/ENodeDeserializer.h"

struct EBrytecAppData {
    Embedded::NodesVector nodeVector;
    EBrytecNodeGroup* nodeGroups;
    uint32_t nodeGroupsCount = 0;
};
static EBrytecAppData s_data;

void EBrytecNodeGroup::setupPin()
{
    BrytecBoard::setupPin(boardPinIndex, type);
}

void EBrytecNodeGroup::updateInitialValue()
{
    ENode* node = EBrytecApp::getInitialValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    node->SetValue(0, BrytecBoard::getPinValue(boardPinIndex));
}

void EBrytecNodeGroup::updateFinalValue()
{
    ENode* node = EBrytecApp::getFinalValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    BrytecBoard::setPinValue(boardPinIndex, *node->GetOutput());
}

void EBrytecApp::deserializeModule(BinaryDeserializer& des)
{

    // deserialize modules specs
    des.readRaw<char>(); // M
    des.readRaw<char>(); // D
    // TODO check header

    uint8_t major = des.readRaw<uint8_t>();
    uint8_t minor = des.readRaw<uint8_t>();
    // TODO check version

    // module info
    des.readRaw<std::string>(); // name
    des.readRaw<uint8_t>(); // address
    des.readRaw<uint8_t>(); // enabled

    // deserialize node groups
    uint16_t nodeGroupCount = des.readRaw<uint16_t>();
    s_data.nodeGroupsCount = nodeGroupCount;
    s_data.nodeGroups = new EBrytecNodeGroup[nodeGroupCount];

    // node start and length

    for (int nodeGroupIndex = 0; nodeGroupIndex < nodeGroupCount; nodeGroupIndex++) {

        EBrytecNodeGroup& currentNodeGroup = s_data.nodeGroups[nodeGroupIndex];
        currentNodeGroup.startNodeIndex = s_data.nodeVector.count();
        currentNodeGroup.boardPinIndex = des.readRaw<uint16_t>();

        // deserialize modules specs
        des.readRaw<char>(); // N
        des.readRaw<char>(); // G
        // TODO check header

        uint8_t major = des.readRaw<uint8_t>();
        uint8_t minor = des.readRaw<uint8_t>();
        // TODO check version

        // node group info
        des.readRaw<std::string>(); // name
        des.readRaw<uint64_t>(); // uuid
        currentNodeGroup.type = (IOTypes::Types)des.readRaw<uint8_t>(); // type
        des.readRaw<uint8_t>(); // enabled

        // create nodes in vector
        {
            int nodeCount = des.readRaw<uint8_t>();
            for (int i = 0; i < nodeCount; i++) {
                ENodeSpec spec = ENodeDeserializer::deserializeNodeSpec(des);
                s_data.nodeVector.add(spec);
            }

            currentNodeGroup.nodeCount = nodeCount;
        }

        // connect nodes
        {
            int nodeCount = des.readRaw<uint8_t>();
            for (int i = 0; i < nodeCount; i++) {
                ENodeConnection connection = ENodeDeserializer::deserializeNodeConnection(des);
                // set connection
            }
        }
    }
}

void EBrytecApp::setupPins()
{
    for (int i = 0; i < s_data.nodeGroupsCount; i++)
        s_data.nodeGroups[i].setupPin();
}

void EBrytecApp::update(float timestep)
{
    // get inputs
    for (int i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateInitialValue();
    }

    // calculate all nodes
    s_data.nodeVector.evaluateAll(timestep);

    // set outputs
    for (int i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateFinalValue();
    }
}

ENode* EBrytecApp::getInitialValueNode(int startIndex, int nodeCount)
{
    for (int i = startIndex; i < startIndex + nodeCount; i++) {
        if (auto node = dynamic_cast<EInitialValueNode*>(s_data.nodeVector.at(i)))
            return node;
    }

    return nullptr;
}

ENode* EBrytecApp::getFinalValueNode(int startIndex, int nodeCount)
{
    // Start at the last one because it should always be the last
    for (int i = startIndex + nodeCount - 1; i >= startIndex; i--) {
        if (auto node = dynamic_cast<EFinalValueNode*>(s_data.nodeVector.at(i)))
            return node;
    }

    return nullptr;
}

ENode* EBrytecApp::getNode(int index)
{
    return s_data.nodeVector.at(index);
}
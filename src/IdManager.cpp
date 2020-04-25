#include <GLRF/IdManager.hpp>

IdManager::IdManager() {
    this->next_node_id = 0;
}

IdManager::~IdManager() {

}

IdSpaceSize IdManager::getNodeId() {
    return this->next_node_id++;
}

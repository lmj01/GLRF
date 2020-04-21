#pragma once
#include <memory>

typedef unsigned long long IdSpaceSize;

class IdManager {
private:
    IdSpaceSize next_node_id;
    IdManager();
    IdManager(const IdManager&);
    IdManager & operator = (const IdManager &);
public:
    static IdManager& getInstance() {
        static IdManager instance;
        return instance;
    }

    ~IdManager();

    IdSpaceSize getNodeId();
};
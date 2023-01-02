#pragma once

#include <vector>
#include <unordered_map>
#include <limits.h>

#include "simple_server/common.h"
#include "simple_server/event_loop.h"

class ReactorPool{
public:
    DISALLOW_COPY_AND_MOVE(ReactorPool);
    ReactorPool(int num);
    ~ReactorPool();

    EventLoop* BindConnectionToReactor(int fd);
    EventLoop* GetReactorByIndex(int index);
    int RemoveConnection(int fd);

    int GetConnectionNumInReactor(int index);
    int GetReactorIndexOfConnection(int fd);

private:
    std::vector<int> connection_num;
    std::vector<std::unique_ptr<EventLoop>> sub_reactors;
    std::unordered_map<int, int> reactor_index_of_connection;
    int GetIndexOfAvailabelReactor();
    int IncreaseConnection(int index);
    int DecreaseConnection(int index);


};

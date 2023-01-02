#pragma once
#include <memory>
#include <string>

#include "simple_server/common.h"

class Buffer {
public:
    DISALLOW_COPY_AND_MOVE(Buffer);
    Buffer() = default;
    ~Buffer() = default;

    const std::string &GetBuf() const;
    const char* c_str() const;
    size_t Size() const;

    void SetBuf(const char *buf);
    void Append(const char *data, int length);
    void Clear();
    

private:
    std::string buf;
};

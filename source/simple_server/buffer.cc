#include "simple_server/buffer.h"

const std::string &Buffer::GetBuf() const { 
    return buf; 
}

const char *Buffer::c_str() const { 
    return buf.c_str(); 
}

void Buffer::SetBuf(const char *buf) {
    std::string new_buf(buf);
    this->buf.swap(new_buf);
}

size_t Buffer::Size() const { 
    return buf.size(); 
}

void Buffer::Append(const char *str, int size) {
    for (int i = 0; i < size; ++i) {
        if (str[i] == '\0') {
            break;
        }
        buf.push_back(str[i]);
    }
}

void Buffer::Clear() { 
    buf.clear(); 
}

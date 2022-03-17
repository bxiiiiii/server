#ifndef NET_BUFFER_H
#define NET_BUFFER_H
#include <vector>

class Buffer
{
public:
    static const int kPrepend = 8;
    static const int kInitialSize = 1024;

    Buffer(int initialSize = kInitialSize);

    void swap(Buffer& rhs);

    int readableBytes();
    int writableBytes();
    int prependableBytes();

    char* begin();
    const char* peek();

    void retrieve(size_t n);
    void retrieveAll();
    std::string retrieveAllAsString();
    std::string retrieveAsString(size_t len);

    void append(const char* data, int len);
    void ensureWritableBytes(size_t len);
    char* beginWrite();
    void hasWritten(size_t len);
    int readFd(int fd, int* Errno);
    void makeSpace(size_t len);

private:

    std::vector<char> buffer_;
    int readIndex_;
    int writeIndex_;
};

#endif
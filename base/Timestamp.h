#include <iostream>

class Timestamp
{
public:
    Timestamp(int time);
    static Timestamp now();
};
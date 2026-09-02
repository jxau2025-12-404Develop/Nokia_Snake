#include <iostream>

namespace Utils
{
    namespace Out
    {
        template <typename T> void Out(T&& msg)
        {
            std::cout << msg << std::endl;
        }
    } // namespace Out
} // namespace Utils
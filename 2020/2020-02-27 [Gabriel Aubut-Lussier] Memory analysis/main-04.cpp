#include <vector>

void f()
{
        std::vector<std::vector<char>> v;
        const int n = 10;
        v.reserve(n);
        for (int i = 0; i < n; ++i) {
                v.emplace_back();
                v.back().reserve(127 * 1024);
        }
}

int main()
{
        std::vector<char> v;
        v.reserve(127*1024);
        f();
}
char* f()
{
        return new char[128*1024 - 15];
}

int main()
{
        f();
        return 42;
}
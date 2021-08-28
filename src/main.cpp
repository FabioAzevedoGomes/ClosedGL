#include "ProgramManager.hpp"

int main(int argc, char **argv)
{
    char *inputFile = nullptr;
    if (argc > 0)
        inputFile = argv[1];

    ProgramManager manager(inputFile);

    manager.Run();

    return 0;
}

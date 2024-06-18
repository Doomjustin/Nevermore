#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "nevermore/posix/File.h"
#include "nevermore/posix/Writable.h"
#include "nevermore/posix/Readable.h"

using namespace sf::posix;

class WriteTest: public Writable<WriteTest> {
public:
    ~WriteTest() = default;

    constexpr int fd() const noexcept
    {
        return STDOUT_FILENO;
    }
};

class ReadTest: public Readable<ReadTest> {
public:
    ~ReadTest() = default;

    constexpr int fd() const noexcept
    {
        return STDIN_FILENO;
    }
};

int main(int argc, char* argv[])
{
    WriteTest write_test{};

    const char* buf = "input file name which to open:\n->";
    write_test.write(buf, strlen(buf));

    ReadTest read_test{};
    char file_path[4096];
    auto read_bytes = read_test.read(file_path, 4096);
    file_path[read_bytes - 1] = '\0';

    File file{ file_path, O_RDWR | O_APPEND };
    file.write("test msg\n", 9);
    
    return EXIT_SUCCESS;
}   
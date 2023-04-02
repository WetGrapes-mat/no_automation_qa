#include <hello_lib.hxx>

int main(int /*argc*/, char* /*argv*/[], char* /*argv_env*/[])
{
    bool is_good = greetings();
    int  result  = is_good ? EXIT_SUCCESS : EXIT_FAILURE;
    return result;
}

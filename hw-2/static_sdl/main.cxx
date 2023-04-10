#include <SDL3/SDL_version.h>
#include <iostream>

int main()
{

    SDL_version compiled, linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    int len_compiled = printf("compiled: %d.%d.%d \n",
                              compiled.major,
                              compiled.minor,
                              compiled.patch);
    int len_linked =
        printf("linked: %d.%d.%d \n", linked.major, linked.minor, linked.patch);
    if (len_compiled > 0 && len_linked > 0)
    {
        return 0;
    }
    else
    {
        std::cerr << "Error: printf failed!" << std::endl;
        return 1;
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <libtguy.h>
#include <string.h>

#if defined(_WIN32)
    #include <Windows.h>
#elif defined(__unix__)
    #include <unistd.h>
#endif

void tsleep(long int ms) {
#if defined(_WIN32)
    Sleep(ms);
#elif defined(__unix__)
    usleep(ms * 1000);
#endif
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Input a string!");
        return EXIT_FAILURE;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    {
        TrashGuyState st = tguy_init_str(argv[1], strlen(argv[1]), 4);
        for (int i = 0; i < st.max_frames; i++) {
            tguy_from_frame(&st, i);
            tguy_print(&st);
            putchar('\r');
            tsleep(500);
        }
    }
    return EXIT_SUCCESS;
}

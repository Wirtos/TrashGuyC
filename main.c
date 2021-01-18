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
    TrashGuyState *st;
    if (argc == 2) {
        st = tguy_init_str(argv[1], strlen(argv[1]), 4);
    } else if (argc > 2) {
        size_t len = (argc - 1) * 2 - 1;
        CString *cs_arr = calloc(len, sizeof(*cs_arr));
        if (!cs_arr) {
            puts("Not enough memory!");
            return EXIT_FAILURE;
        }
        for (int i = 1, c = 0; i < argc; i++, c += 2) {
            cs_arr[c] = (CString) {argv[i], strlen(argv[i])};
            if (i + 1 != argc) {
                cs_arr[c + 1] = (CString) {" ", 1};
            }
        }
        st = tguy_init_arr(cs_arr, len, 4);
        free(cs_arr);
    } else {
        puts("Provide at least one string!");
        return EXIT_FAILURE;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    {
        for (int i = 0, frames = tguy_get_frames_count(st); i < frames; i++) {
            tguy_from_frame(st, i);
            tguy_print(st);
            putchar('\r');
            tsleep(500);
        }
    }
    tguy_free(st);
    return EXIT_SUCCESS;
}

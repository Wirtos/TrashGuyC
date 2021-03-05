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
        st = tguy_from_utf8(argv[1], strlen(argv[1]), 4);
    } else if (argc > 2) {
        size_t len = (argc - 1);
        TGString *cs_arr = calloc(len, sizeof(*cs_arr));
        if (!cs_arr) {
            puts("Not enough memory!");
            return EXIT_FAILURE;
        }
        for (int i = 1; i < argc; i++) {
            size_t slen = strlen(argv[i]);
            argv[i][slen] = ' ';
            cs_arr[i - 1] = (TGString) {argv[i],  slen + 1};
        }
        st = tguy_from_arr(cs_arr, len, 4);
        free(cs_arr);
    } else {
        puts("Provide at least one string!");
        return EXIT_FAILURE;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    {
        for (int i = 0, frames = tguy_get_frames_count(st); i < frames; i++) {
            tguy_set_frame(st, i);
            tguy_print(st);
            putchar('\n');
            tsleep(500);
        }
    }
    tguy_free(st);
    return EXIT_SUCCESS;
}

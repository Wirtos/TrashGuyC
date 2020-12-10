#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *str;
    size_t len;
} CString;

/*
 * Create a constant containerized string
 */
#define CStringConst(str) ((CString){str, sizeof(str) - 1})

/*
 * Get the first frame index involving working with the element n
 */
static inline int get_frame_lower_boundary(int a1, int n) {
    return n * (a1 + n - 1);
}

typedef struct {
    CString arr[100];
    size_t len;
} TrashField;

typedef struct {
    int a1;
    size_t cur_frame;
    size_t max_frames;
    CString sprite_right, sprite_left, sprite_can;
    CString text;
    TrashField field;
} TrashGuyState;

static inline void putsn(const char *str, size_t n) { while (n--) putchar(*str++); }

/*
 * Clear the field(including the guy) and remove the first n chars of the string
 * todo: use a grapheme array
 */
static inline void tguy_clear_field(TrashGuyState *st, size_t n) {
    size_t i = 1;
    for (size_t j = 0; j < (st->a1 / 2) + n; i++, j++) st->field.arr[i] = CStringConst(" ");
    for (size_t j = n; j < st->text.len; j++, i++) st->field.arr[i] = (CString) {&st->text.str[j], 1};
}

/*
 * Fill the state with desirable frame
 */
void tguy_from_frame(TrashGuyState *st, size_t frame) {
    /* n ^ 2 + a1n - frame = 0 */
    if (frame < st->max_frames) {
        /* int a = 1,*/
        int b = (st->a1 - 1),
            c = -frame;
        /* school math */
        size_t n = (-b + sqrt(pow(b, 2) - (4 /* * a */ * c))) / 2/* * a */;
        /* total number of frame drawn for moving the letter with index n (moving from the start to the end)*/
        size_t frames_per_n = st->a1 + (2 * n);
        /* order of the frame in the frame series (up to frames_per_n) */
        size_t sub_frame = (frame - get_frame_lower_boundary(st->a1, n));
        /* if we're in the first half frames we're moving right, otherwise left */
        int right = (sub_frame < frames_per_n / 2);
        /* index yields 0 twice, the difference is whether we're moving right */
        size_t i = right ? sub_frame : frames_per_n - sub_frame - 1;

        st->cur_frame = frame; /* unused */
        /* if we're not moving right, then we're not drawing the element n because trashguy holds it */
        tguy_clear_field(st, n + !right);

        /* don't overwrite the trash can */
        st->field.arr[i + 1] = right ? st->sprite_right : st->sprite_left;
        /* Draw the element trashguy holds */
        if (!right && i != 0) {
            st->field.arr[i] = (CString) {&st->text.str[n], 1}; /* todo: don't. use the array. */
        }
    }
}

TrashGuyState tguy_init(CString text, int starting_distance) {
    TrashGuyState st = {
        .sprite_right = CStringConst("(> ^_^)>"),
        .sprite_left = CStringConst("<(^_^ <)"),
        .sprite_can = CStringConst("\xf0\x9f\x97\x91"),
        .a1 = (starting_distance + 1) * 2,
        .cur_frame = 0, /* currently unused */
        .field = {
            .len = starting_distance + text.len + 2
        },
        .max_frames = get_frame_lower_boundary(st.a1, text.len) + 1, /* number of frames up to the last + 1 */
        .text = text /* should use an array of containers instead */
    };

    st.field.arr[0] = st.sprite_can;
    tguy_clear_field(&st, 0);
    st.field.arr[1] = st.sprite_right;
    return st;
}

void tguy_print(const TrashGuyState *st) {
    for (size_t i = 0; i < st->field.len; ++i) {
        putsn(st->field.arr[i].str, st->field.arr[i].len);
    }
    putchar('\n');
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Input a string!");
        return EXIT_FAILURE;
    }
    {
        CString input = {argv[1], strlen(argv[1])};
        TrashGuyState st = tguy_init(input, 4);

        for (size_t i = 0; i < st.max_frames; i++) {
            tguy_from_frame(&st, i);
            tguy_print(&st);
        }
    }
    return EXIT_SUCCESS;
}

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
    CString sprite_right, sprite_left, sprite_can;
    TrashField text;
    TrashField field;
    size_t cur_frame;
    size_t max_frames;
} TrashGuyState;

static inline void putsn(const char *str, size_t n) { while (n--) putchar(*str++); }

/*
 * Clear the field(including the guy) and remove the first n chars of the string
 */
static inline void tguy_clear_field(TrashGuyState *st, size_t n) {
    size_t i = 1;
    for (size_t j = 0; j < (st->a1 / 2) + n; i++, j++) {
        st->field.arr[i] = CStringConst(" ");
    }
    for (size_t j = n; j < st->text.len; j++, i++) {
        st->field.arr[i] = st->text.arr[j];
    }
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
            st->field.arr[i] = st->text.arr[n];
        }
    }
}

static inline const char *utf8_next(const char *begin, const char *end) {
    if (begin == end) {
        return NULL;
    }

    if ((*begin & 0x80) == 0x0) {
        begin += 1;
    } else if ((*begin & 0xE0) == 0xC0) {
        begin += 2;
    } else if ((*begin & 0xF0) == 0xE0) {
        begin += 3;
    } else if ((*begin & 0xF8) == 0xF0) {
        begin += 4;
    }

    return begin;
}

static inline size_t utf8_distance(const char *begin, const char *end) {
    size_t dist = 0;
    while ((begin = utf8_next(begin, end))) dist++;
    return dist;
}

TrashGuyState tguy_init(CString text, int starting_distance) {
    TrashGuyState st = {
        .a1 = (starting_distance + 1) * 2,
        .sprite_right = CStringConst("(> ^_^)>"),
        .sprite_left = CStringConst("<(^_^ <)"),
        .sprite_can = CStringConst("\xf0\x9f\x97\x91"),
        .text = {
            .len = utf8_distance(&text.str[0], &text.str[text.len])
        },
        .field = {
            .len = starting_distance + st.text.len + 2 /* additional 2 elements to hold the guy and can sprites */
        },
        .cur_frame = 0, /* currently unused */
        .max_frames = get_frame_lower_boundary(st.a1, st.text.len) + 1, /* number of frames up to the last + 1 */
    };

    st.field.arr[0] = st.sprite_can;
    tguy_clear_field(&st, 0);
    st.field.arr[1] = st.sprite_right;
    { /* fill the array with ranges of the string representing whole utf-8 codepoints (up to 4 per element)*/
        const char *it = text.str;
        for (int i = 0; i < st.text.len; ++i) {
            const char *next;
            next = utf8_next(it, &text.str[text.len]);
            st.text.arr[i] = (CString) {it, next - it};
            it = next;
        }
    }
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

#include <stdio.h>
#include <stdlib.h>

/*
 * __same_type: 根據不同的編譯器有不同的版本，這邊以 gcc 為例，
 * 使用的 gcc 的 builtin-function __builtin_types_compatible_p ,
 * 當兩者 type 相同時回傳 1，否則回傳 0：
 *
 */
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

/*
 * Force a compilation error if condition is true, but also produce a
 * result (of value 0 and type size_t), so the expression can be used
 * e.g. in a structure initializer (or where-ever else comma expressions
 * aren't permitted).
 */
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:(-!!(e)); }))

/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a)    BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

/**
 * ARRAY_SIZE - get the number of elements in array @arr
 * @arr: array to be sized
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))


int main() {
    int *a = malloc(sizeof(int) * 10);
    int b[10];
 
    printf("%d\n", __builtin_types_compatible_p(typeof((a)), typeof((&(a)[0]))));
    printf("%d\n", __builtin_types_compatible_p(typeof(b), typeof(&(b)[0])));
    printf("%d\n", -!!(0));
    printf("%d\n", -!!(1));
    //printf("a size: %d\n",ARRAY_SIZE(a));
    printf("b arr size: %ld\n", ARRAY_SIZE(b));

    // sizeof(struct { int: (-!!(0)); });
    // sizeof(struct { int: (-!!(1)); });

    return 0;
}
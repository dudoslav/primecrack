#include<stdio.h>
#include<time.h>
#include<math.h>

typedef unsigned long long number_t;

typedef struct {
    number_t n1;
    number_t n2;
} divres_t; //Division result struct

static void test(const char* funcName, int (*func)(number_t, divres_t*), number_t number) {
    printf("TEST: %s with number: %llu\n", funcName, number);
    clock_t startTicks = -clock();
    divres_t result = {0};
    if (func(number, &result)) {
        printf("\tResult n1: %llu n2: %llu\n", result.n1, result.n2);
        printf("\tComputation time: %ld\n", (clock() + startTicks));
    } else {
        printf("\tCouldn't devide into prime numbers\n");
    }
}

#define TEST(func, number) test(#func, func, number)

/* Definitions of cracking functions */

static inline int devides(number_t dividend, number_t factor) {
    return (dividend % factor == 0);
}

static int sqrtFind(number_t number, divres_t *result) {
    number_t sqrtNumber = sqrt(number);
    for (number_t i = 2; i < sqrtNumber; ++i) {
        if (devides(number, i)) {
            result->n1 = i;
            result->n2 = number / i;
            return 1;
        }
    }

    return 0;
}

static int sqrtFindSmart(number_t number, divres_t *result) {
    number_t sqrtNumber = sqrt(number);

    if (devides(number, 2)) {
        result->n1 = 2;
        result->n2 = number / 2;
        return 1;
    }

    for (number_t i = 3; i < sqrtNumber; i += 2) {
        if (devides(number, i)) {
            result->n1 = i;
            result->n2 = number / i;
            return 1;
        }
    }

    return 0;
}

static int sqrtStepFind(number_t number, divres_t *result) {
    number_t steps[] = {2, 4, 2, 2};

    number_t sqrtNumber = sqrt(number);

    if (devides(number, 2)) {
        result->n1 = 2;
        result->n2 = number / 2;
        return 1;
    }

    if (devides(number, 3)) {
        result->n1 = 3;
        result->n2 = number / 3;
        return 1;
    }

    if (devides(number, 5)) {
        result->n1 = 5;
        result->n2 = number / 5;
        return 1;
    }

    if (devides(number, 7)) {
        result->n1 = 7;
        result->n2 = number / 7;
        return 1;
    }

    number_t pos = 0;
    for (number_t i = 11; i < sqrtNumber;) {
        if (devides(number, i)) {
            result->n1 = i;
            result->n2 = number / i;
            return 1;
        }
        i += steps[pos++ % 4];
        //pos = (++pos) % 4;
    }

    return 0;
}

/* END Definitions of cracking functions */

static number_t numbers[] = {21, 62615533, 805913377, 1074889006972199, 10142789312725007};

int main(void)
{
    for (size_t i = 0; i < (sizeof(numbers) / 8); ++i) {
        TEST(sqrtFind, numbers[i]);
        TEST(sqrtFindSmart, numbers[i]);
        TEST(sqrtStepFind, numbers[i]);
    }
    return 0;
}

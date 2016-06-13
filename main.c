#include<stdio.h>
#include<time.h>
#include<math.h>
#include<gmp.h>

typedef struct {
    mpz_t n1;
    mpz_t n2;
} divres_t; //Division result struct

static void test(const char* funcName, int (*func)(mpz_t, divres_t*), mpz_t number) {
    printf("TEST: %s on number: ", funcName);
    mpz_out_str(stdout, 10, number);
    printf("\n");

    divres_t result;
    mpz_init(result.n1);
    mpz_init(result.n2);

    clock_t startTicks = -clock();
    if (func(number, &result)) {
        printf("\t n1: "); mpz_out_str(stdout, 10, result.n1); printf("\n");
        printf("\t n2: "); mpz_out_str(stdout, 10, result.n2); printf("\n");
        printf("\tComputation time: %ld\n", (clock() + startTicks));
    } else {
        printf("\tCouldn't devide\n");
    }

    mpz_clear(result.n1);
    mpz_clear(result.n2);
}

#define TEST(func, number) test(#func, func, number)

/* Definitions of cracking functions */

static int sqrtFind(mpz_t number, divres_t *result) {
    mpz_t sqrtNumber;
    mpz_init(sqrtNumber);
    mpz_sqrt(sqrtNumber, number);

    mpz_t iter;
    mpz_init(iter);
    mpz_set_ui(iter, 2);
    for (; mpz_cmp(iter, sqrtNumber) < 0; mpz_add_ui(iter, iter, 1)) {
        if (mpz_divisible_p(number, iter)) {
            mpz_set(result->n1, iter);
            mpz_cdiv_q(result->n2, number, iter);
            mpz_clear(sqrtNumber);
            mpz_clear(iter);
            return 1;
        }
    }

    mpz_clear(sqrtNumber);
    mpz_clear(iter);
    return 0;
}

static int sqrtStepFind(mpz_t number, divres_t *result) {
    unsigned long int steps[] = {2, 4, 2, 2};

    mpz_t sqrtNumber;
    mpz_init(sqrtNumber);
    mpz_sqrt(sqrtNumber, number);

    if (mpz_divisible_ui_p(number, 2)) {
        mpz_set_ui(result->n1, 2);
        mpz_cdiv_q_ui(result->n2, number, 2);
        mpz_clear(sqrtNumber);
        return 1;
    }

    if (mpz_divisible_ui_p(number, 3)) {
        mpz_set_ui(result->n1, 3);
        mpz_cdiv_q_ui(result->n2, number, 3);
        mpz_clear(sqrtNumber);
        return 1;
    }

    if (mpz_divisible_ui_p(number, 5)) {
        mpz_set_ui(result->n1, 5);
        mpz_cdiv_q_ui(result->n2, number, 5);
        mpz_clear(sqrtNumber);
        return 1;
    }

    if (mpz_divisible_ui_p(number, 7)) {
        mpz_set_ui(result->n1, 7);
        mpz_cdiv_q_ui(result->n2, number, 7);
        mpz_clear(sqrtNumber);
        return 1;
    }


    unsigned int pos = 0;
    mpz_t iter;
    mpz_init(iter);
    mpz_set_ui(iter, 11);
    while (mpz_cmp(iter, sqrtNumber) < 0) {
        if (mpz_divisible_p(number, iter)) {
            mpz_set(result->n1, iter);
            mpz_cdiv_q(result->n2, number, iter);
            mpz_clear(sqrtNumber);
            mpz_clear(iter);
            return 1;
        }
        mpz_add_ui(iter, iter, steps[pos]);
        pos = (pos + 1) % 4;
    }

    return 0;
}

/* END Definitions of cracking functions */

static char* numbers[] = {"21"
                          , "62615533"
                          , "805913377"
                          , "999962000357"
                          , "1074889006972199"
                          , "10142789312725007"
                          , "308985181678252244034230903546833719469"
                          , "70333883891476121734563680282984266734194159038582242804233001028050105973573"};

static void runTests(mpz_t number) {
    //TEST(sqrtFind, number);
    TEST(sqrtStepFind, number);
}

int main(void)
{
    mpz_t number;
    mpz_init(number);

    for (size_t i = 0; i < (sizeof(numbers) / 4); ++i) {
        if (!mpz_set_str(number, numbers[i], 10)) {
            runTests(number);
        } else {
            fprintf(stderr, "ERROR: failed to convert string into number. String: %s", numbers[i]);
        }
    }

    mpz_clear(number);
    return 0;
}

#include<stdio.h>
#include<time.h>
#include<getopt.h>
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
        printf("\tComputation ticks: %ld ticks\n", (clock() + startTicks));
        printf("\tComputation time : %f seconds\n", (float)(clock() + startTicks) / CLOCKS_PER_SEC);
    } else {
        printf("\tCouldn't devide\n");
    }

    mpz_clear(result.n1);
    mpz_clear(result.n2);
}

#define TEST(func, number) test(#func, func, number)

/* Definitions of cracking functions */

static int sqrtInvertStepFind(mpz_t number, divres_t *result) {
    unsigned long int steps[] = {2, 2, 4, 2};

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

    mpz_t reminder;
    mpz_init(reminder);
    while (1) {
        mpz_mod_ui(reminder, sqrtNumber, 10);
        if (mpz_divisible_p(number, sqrtNumber)) {
            mpz_set_ui(result->n1, 7);
            mpz_cdiv_q_ui(result->n2, number, 7);
            mpz_clear(sqrtNumber);
            mpz_clear(reminder);
            return 1;
        }
        if (0 == mpz_cmp_ui(reminder, 1)) {
            break;
        }
        mpz_sub_ui(sqrtNumber, sqrtNumber, 1);
    }
    mpz_clear(reminder);

    unsigned int pos = 0;
    while (mpz_cmp_ui(sqrtNumber, 7) > 0) {
        if (mpz_divisible_p(number, sqrtNumber)) {
            mpz_set(result->n1, sqrtNumber);
            mpz_cdiv_q(result->n2, number, sqrtNumber);
            mpz_clear(sqrtNumber);
            return 1;
        }
        mpz_sub_ui(sqrtNumber, sqrtNumber, steps[pos]);
        pos = (pos + 1) % 4;
    }

    mpz_clear(sqrtNumber);
    return 0;
}

/* END Definitions of cracking functions */

static void crack(char *numberChar, int base) {
    mpz_t number;
    mpz_init(number);

    if (!mpz_set_str(number, numberChar, base)) {
        TEST(sqrtInvertStepFind, number);
    } else {
        fprintf(stderr, "ERROR: failed to convert string into number. String: %s\n", numberChar);
    }

    mpz_clear(number);
}

static void printHelp() {
    printf("############################################\n");
    printf("|PrimeCrack is program for deviding numbers|\n");
    printf("|into their fractions. Useful for RSA      |\n");
    printf("|cracking. Bigger numbers (64+ bit) may be |\n");
    printf("|slow, nearly impossible to crack.         |\n");
    printf("|Options:                                  |\n");
    printf("|    -d     followed by decimal     number |\n");
    printf("|    -o     followed by octal       number |\n");
    printf("|    -x     followed by hexadecimal number |\n");
    printf("|    -h     shows this help                |\n");
    printf("############################################\n");
}

int main(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "x:o:d:h")) != -1) {
        switch (opt) {
            case 'x':
                crack(optarg, 16);
                break;
            case 'o':
                crack(optarg, 8);
                break;
            case 'd':
                crack(optarg, 10);
                break;
            case 'h':
                printHelp();
                break;
            default:
                fprintf(stderr, "ERROR: Wrong argument\n");
        }
    }

    return 0;
}

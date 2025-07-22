#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint64_t fibonacci(uint64_t n)
{
    uint64_t* memo = malloc((n + 1) * sizeof(uint64_t));
    memo[0] = 0;
    memo[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        memo[i] = memo[i-1] + memo[i-2];
    }
    const uint64_t result = memo[n];
    free(memo);
    return result;
}

int main()
{
    printf("%llu", fibonacci(93));
    return 0;
}
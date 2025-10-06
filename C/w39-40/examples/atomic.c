#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

static int cnt = 0;
static atomic_int acnt = 0;

static int increment(void *data)
{
    (void)data;

    for (int n = 0; n < 1000; ++n)
    {
        atomic_fetch_add(&acnt, 1); // atomic
        cnt++;                      // undefined behavior, in practice some updates missed
    }

    return 0;
}

int main(void)
{
    thrd_t thr[10];

    for (int n = 0; n < 10; ++n)
    {
        thrd_create(&thr[n], increment, NULL);
    }

    for (int n = 0; n < 10; ++n)
    {
        thrd_join(thr[n], NULL);
    }

    (void)printf("The atomic counter is %d\n", acnt);
    (void)printf("The non-atomic counter is %d\n", cnt);

    return 0;
}

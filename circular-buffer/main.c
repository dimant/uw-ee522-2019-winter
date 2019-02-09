#include <stdio.h>
#include <math.h>

#include "minunit.h"

#include "queue.h"

int tests_run = 0;

const char * test_create() 
{
    queue_t queue;

    queue_create(&queue, 10);

    mu_assert("queue buffer is NULL", queue._queue != NULL);
    mu_assert("queue size is not 10", queue._size == 10);
    mu_assert("queue remaining bytes is not 10", queue._remaining == 10);
    mu_assert("queue in ptr is not 0", queue._in == 0);
    mu_assert("queue out ptr is not 0", queue._out == 0);
    return 0;
}

const char * test_delete() 
{
    queue_t queue;

    queue_create(&queue, 10);
    queue_delete(&queue);

    mu_assert("queue buffer is not NULL", queue._queue == NULL);
    mu_assert("queue size is not 0", queue._size == 0);
    mu_assert("queue remaining bytes is not 0", queue._remaining == 0);
    mu_assert("queue in ptr is not 0", queue._in == 0);
    mu_assert("queue out ptr is not 0", queue._out == 0);
    return 0;
}

const char * test_put() 
{
    queue_t queue;
    float data[] = { 1.0f, 2.0f, 3.0f };

    queue_create(&queue, 10);
    queue_put(&queue, data, 3);

    mu_assert("queue[0] !< queue[1]", queue._queue[0] < queue._queue[1]);
    mu_assert("queue[1] !< queue[2]", queue._queue[1] < queue._queue[2]);

    mu_assert("queue remaining bytes is not 7", queue._remaining == 7);
    mu_assert("queue in ptr is not 3", queue._in == 3);
    mu_assert("queue out ptr is not 0", queue._out == 0);

    queue_delete(&queue);
    return 0;
}

const char * test_get() 
{
    queue_t queue;
    float input[] = { 1.0f, 2.0f, 3.0f };
    float output[] = { -1.0f, -1.0f, -1.0f };
    uint32_t result = 0;

    queue_create(&queue, 10);

    result = queue_put(&queue, input, 3);
    mu_assert("result != 3", result == 3);

    result = queue_get(&queue, output, 3);
    mu_assert("result != 3", result == 3);

    mu_assert("output[0] < 0", output[0] > -0.5f);
    mu_assert("output[1] < 0", output[1] > -0.5f);
    mu_assert("output[2] < 0", output[2] > -0.5f);

    queue_delete(&queue);
    return 0;
}

const char * test_wrap() 
{
    queue_t queue;
    float input1[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    float input2[] = { 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f};
    float output[] = { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    uint32_t result = 0;

    queue_create(&queue, 10);
    result = queue_put(&queue, input1, 6);
    mu_assert("result != 6", result == 6);

    result = queue_get(&queue, output, 6);
    mu_assert("result != 6", result == 6);

    result = queue_put(&queue, input2, 6);
    mu_assert("result != 6", result == 6);

    result = queue_get(&queue, output, 6);
    mu_assert("result != 6", result == 6);

    mu_assert("output[0] != 7.0f", fabs(output[0] - 7.0f) < 0.1);
    mu_assert("output[1] != 8.0f", fabs(output[1] - 8.0f) < 0.1);
    mu_assert("output[2] != 9.0f", fabs(output[2] - 9.0f) < 0.1);

    queue_delete(&queue);
    return 0;
}

const char * all_tests() {
    mu_run_test(test_create);
    mu_run_test(test_delete);
    mu_run_test(test_put);
    mu_run_test(test_get);
    mu_run_test(test_wrap);
    return 0;
}

int main(int argc, char* argv[])
{
    const char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

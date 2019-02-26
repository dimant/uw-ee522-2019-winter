#include <stdio.h>
#include <math.h>
#include <string.h>

#include "minunit.h"

#include "queue.h"
#include "audio.h"
#include "morse.h"

#define epsilon 0.000001
#define FCOMP(x, y) (fabs(x - y) < epsilon)

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

    return NULL;
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

    return NULL;
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

    return NULL;
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

    return NULL;
}

const char * test_wrap() 
{
    queue_t queue;
    float input1[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
    float input2[] = { 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f };
    float output[] = { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f };
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

    return NULL;
}

const char* test_audio_pulse()
{
    const uint32_t buffer_size = 30;
    const uint32_t samples = 11;
    const uint32_t period = 8;

    float buffer[buffer_size];

    memset(buffer, -1.0f, sizeof(float) * buffer_size);

    audio_pulse(buffer, samples, period, 0, 50);
    audio_pulse(buffer + samples, samples, period, 1, 50);

    mu_assert("buffer[0] != 1.0f", FCOMP(buffer[0], 1.0f));
    mu_assert("buffer[3] != 1.0f", FCOMP(buffer[3], 1.0f));
    mu_assert("buffer[4] != 0.0f", FCOMP(buffer[4], 0.0f));
    mu_assert("buffer[7] != 0.0f", FCOMP(buffer[7], 0.0f));

    mu_assert("buffer[8] != 1.0f", FCOMP(buffer[8], 1.0f));
    mu_assert("buffer[11] != 1.0f", FCOMP(buffer[11], 1.0f));
    mu_assert("buffer[12] != 0.0f", FCOMP(buffer[12], 0.0f));
    mu_assert("buffer[15] != 0.0f", FCOMP(buffer[15], 0.0f));

    mu_assert("buffer[16] != 1.0f", FCOMP(buffer[16], 1.0f));
    mu_assert("buffer[19] != 1.0f", FCOMP(buffer[19], 1.0f));
    mu_assert("buffer[20] != 0.0f", FCOMP(buffer[20], 0.0f));
    mu_assert("buffer[21] != -1.0f", FCOMP(buffer[21], 0.0f));

    return NULL;
}

const char* test_audio_pulse_inverted()
{
    const uint32_t buffer_size = 100;
    const uint32_t samples = 32;
    const uint32_t period = 36;

    float buffer[buffer_size];

    memset(buffer, -1.0f, sizeof(float) * buffer_size);

    audio_pulse(buffer, samples, period, 0, 50);
    audio_pulse(buffer + samples, samples, period, 1, 50);

    mu_assert("buffer[0] != 1.0f", FCOMP(buffer[0], 1.0f));
    mu_assert("buffer[17] != 1.0f", FCOMP(buffer[17], 1.0f));
    mu_assert("buffer[18] != 0.0f", FCOMP(buffer[18], 0.0f));
    mu_assert("buffer[35] != 0.0f", FCOMP(buffer[35], 0.0f));

    mu_assert("buffer[36] != 1.0f", FCOMP(buffer[36], 1.0f));
    mu_assert("buffer[53] != 1.0f", FCOMP(buffer[53], 1.0f));
    mu_assert("buffer[54] != 0.0f", FCOMP(buffer[54], 0.0f));
    mu_assert("buffer[63] != 0.0f", FCOMP(buffer[63], 0.0f));

    return NULL;
}

const char* test_morse_put()
{
    uint32_t morse = 0;

    morse_put(&morse, MORSE_DOT);

    mu_assert("morse.encoded is not 01", morse == 1);

    return NULL;
}

const char* test_morse_put_two()
{
    uint32_t morse = 0;

    morse_put(&morse, MORSE_DOT);
    morse_put(&morse, MORSE_DASH);

    mu_assert("morse is not 110", morse == 6);

    return NULL;
}

const char* test_morse_decode_letter()
{
    uint32_t morse = 0;

    morse_put(&morse, MORSE_DOT);
    morse_put(&morse, MORSE_DASH);

    char c = morse_decode(&morse);

    mu_assert("character is not 'a'", c == 'a');

    return NULL;
}

const char* test_morse_decode_number()
{
    uint32_t morse = 0;

    morse_put(&morse, MORSE_DASH);
    morse_put(&morse, MORSE_DASH);
    morse_put(&morse, MORSE_DASH);
    morse_put(&morse, MORSE_DASH);
    morse_put(&morse, MORSE_DASH);

    char c = morse_decode(&morse);

    mu_assert("character is not '0'", c == '0');

    return NULL;
}

const char * all_tests() {
    mu_run_test(test_create);
    mu_run_test(test_delete);
    mu_run_test(test_put);
    mu_run_test(test_get);
    mu_run_test(test_wrap);
    mu_run_test(test_audio_pulse);
    mu_run_test(test_audio_pulse_inverted);
    mu_run_test(test_morse_put);
    mu_run_test(test_morse_put_two);
    mu_run_test(test_morse_decode_letter);
    mu_run_test(test_morse_decode_number);
    return NULL;
}

int main(int argc, char* argv[])
{
    const char *result = all_tests();
    if (result != NULL) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != NULL;
}

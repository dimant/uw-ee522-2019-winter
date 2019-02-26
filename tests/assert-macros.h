#ifndef ASSERT_MACROS_H
#define ASSERT_MACROS_H

#include <assert.h>

#define CHECK_ASSERT

#ifdef CHECK_ASSERT
#define ASSERT(x) assert(x);
#endif

#endif

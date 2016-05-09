#ifndef LOOPTYPE_H
#define LOOPTYPE_H

#include <stdlib.h>

#define RANDOM_NUM(x) rand() % x

// 循环控制..
typedef enum {
	ONELOOP,
	ALLLOOP,
	RANDOMLOOP
}LOOPTYPE;

#endif // LOOPTYPE_H

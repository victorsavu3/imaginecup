#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __GNUC__
#define likely(x)	__builtin_expect (!!(x), 1)
#define unlikely(x)	__builtin_expect (!!(x), 0)
#else
#define likely(x)	x
#define unlikely(x)	x
#endif

#define BUG_ON(x) 	do{ \
						if(unlikely(x)){ \
							fprintf(stderr, "*** BUG in %s: %s() at line %d\n", __FILE__, __FUNCTION__, __LINE__); \
							exit(2); \
						} \
					}while(0);

#define FAIL(...) 	do{ \
						fprintf(stderr, "*** ERROR in %s: %s() at line %d:\n*** ", __FILE__, __FUNCTION__, __LINE__); \
						fprintf(stderr, __VA_ARGS__); \
						fprintf(stderr, "\n"); \
						exit(1); \
					}while(0);

#define FAIL_ON(x, ...) do{if(unlikely(x)){ \
							FAIL(__VA_ARGS__); \
						}}while(0);


#define WARN(...) 	do{ \
						fprintf(stderr, "** WARNING in %s: %s() at line %d:\n** ", __FILE__, __FUNCTION__, __LINE__); \
						fprintf(stderr, __VA_ARGS__); \
						fprintf(stderr, "\n"); \
					}while(0);

#define WARN_ON(x, ...) do{ \
							if(unlikely(x)){ \
								WARN(__VA_ARGS__); \
							} \
						}while(0);

#define INFO(...) 	do{ \
						fprintf(stdout, "* INFO at %s: %s() at line %d: ", __FILE__, __FUNCTION__, __LINE__); \
						fprintf(stdout, __VA_ARGS__); \
						fprintf(stdout, "\n"); \
					}while(0);

#define DBG(...) 	do{ \
						fprintf(stdout, "* DEBUG (%s:%d): ", __FILE__, __LINE__); \
						fprintf(stdout, __VA_ARGS__); \
						fprintf(stdout, "\n"); \
					}while(0);

#endif

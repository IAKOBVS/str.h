#define JSTR_DEBUG 1

#include "test.h"

#define T_APPEND(fn, ...)                                                            \
	do {                                                                         \
		PRINT("Testing %s.\n", JSTR_STRINGIFY(fn));                          \
		ret = fn(__VA_ARGS__);                                               \
		if (ret == 0) {                                                      \
			fprintf(stderr, "%s", JSTR_STRINGIFY(fn));                   \
			jstr_err_exit("");                                           \
		}                                                                    \
		assert(strcmp(j.data, expected) == 0);                               \
		assert(j.size == strlen(expected));                                  \
		assert(memcmp(j.data, expected, j.size) == 0);                       \
		assert(strlen(expected) != 0 && j.size != 0 && j.capacity > j.size); \
		jstr_free_j(&j);                                                     \
	} while (0)

jstr_ty j = { 0 };

char buf[4096] = { 0 };

int
main(int argc, char **argv)
{
	int ret;
	char *expected;
	expected = "hello world";
	T_APPEND(jstr_cat, &j.data, &j.size, &j.capacity, "hello", " ", "world", NULL);
	expected = "hello world";
	T_APPEND(jstr_append_len, &j.data, &j.size, &j.capacity, expected, strlen(expected));
	expected = "hello world";
	T_APPEND(jstr_prepend_len, &j.data, &j.size, &j.capacity, expected, strlen(expected));
	expected = "hello world";
	T_APPEND(jstr_assign_len, &j.data, &j.size, &j.capacity, expected, strlen(expected));

	const char *hello4 = "hello hello hello hello";
	assert(jstr_assign_len(&j.data, &j.size, &j.capacity, hello4, strlen(hello4)));
	expected = "world hello hello hello";
	T_APPEND(jstr_replace_len, &j.data, &j.size, &j.capacity, "hello", "world", strlen("hello"), strlen("world"));

	assert(jstr_assign_len(&j.data, &j.size, &j.capacity, hello4, strlen(hello4)));
	expected = "world world world world";
	T_APPEND(jstr_replaceall_len, &j.data, &j.size, &j.capacity, "hello", "world", strlen("hello"), strlen("world"));

	SUCCESS();
	return 0;
}
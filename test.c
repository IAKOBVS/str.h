#include "_jstr.h"

int main(int argc, char **argv)
{
	const char *s = "hello world113";
	char *p = jstr_strrstr(s, "11");
	/* if (p) */
	/* 	puts(p); */
	return 0;
	/* jstr_ty s; */
	/* char *str = "hello-world"; */
	/* jstr_alloc_append_mem(&s.data, &s.size, &s.cap, str, strlen(str)); */
	/* if (s.data == NULL) */
	/* 	return 1; */
	/* char *searc = "\\w*"; */
	/* char *rplc = "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; */
	/* jstr_debug(&s); */
	/* regex_t r; */
	/* jreg_errcode_ty err = jreg_rplcall_now_mem(&s.data, &s.size, &s.cap, searc, rplc, strlen(rplc), &r, 0, 0); */
	/* jstr_rplcall_mem(&s.data, &s.size, &s.cap, searc, rplc, strlen(searc), strlen(rplc)); */
	/* jreg_rmall_now_mem(s.data, &s.size, searc, &r, 0, 0); */
	/* jstr_debug(&s); */
	/* jstr_free(s.data); */
	/* return err; */
}

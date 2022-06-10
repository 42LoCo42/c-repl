.global call
call:
#ifdef __amd64__
	call *8(%rsp)
#else
	call *28(%esp)
#endif
	ret

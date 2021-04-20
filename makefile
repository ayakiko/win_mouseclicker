WIN_LIBS=advapi32.lib iphlpapi.lib user32.lib gdi32.lib

CFLAGS=/O2 /W3 /wd4996 /nologo $(WIN_LIBS)

comp: main

main:
	$(CC) $(CFLAGS) main.cpp
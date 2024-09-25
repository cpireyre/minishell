CFLAGS := -fsanitize=address,undefined -g3 -Og
.PHONY: all
all: bin
	./bin "ls" "echo a" "date > outfile"
	./bin "| ah &&"
	./bin "echo\"a    b    c\""
	./bin '"a"'
	./bin '"a'

bin: main.c
	cc $(CFLAGS) $^ -o $@

CFLAGS := -fsanitize=address,undefined,leak -g3 -Og

.PHONY: all
all: bin
	./bin "ls" "echo a" "date > outfile"
	./bin "| ah &&"
	./bin "echo\"a    b    c\""
	./bin '"a"'
	./bin '"a'
	./bin "echo'a b c '"
	./bin "echo\"a b c \"'this should be one though   'still   lol"

bin: main.c
	cc $(CFLAGS) $^ -o $@

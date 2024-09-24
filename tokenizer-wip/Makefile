CFLAGS := -fsanitize=address
.PHONY: bin
bin: ./src/main.c
	cc $(CFLAGS) $^ -o $@
	./$@ 'cat"Makefile   a"'
	./$@ '"hello a b c "'
	./$@ "    echo'should still work''aa aa'"
	./$@ "   a b c"
	./$@ "ls<Makefile|pipe' and also some quotes hehe ' 'space here tho'"
	./$@ ""
	./$@ "<"
	./$@ '>>><<|||'
	./$@ '>>" hello  "><<|||'
	./$@ '>>" hello  "><<|||"aaa  b "'
	./$@ echo\'a b c\'
	./$@ "echo'a b c'"

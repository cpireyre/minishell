#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "tokenize.h"

void print_token(t_token token) 
{
    printf("Token: type=%d, value='%s', size=%zu\n", 
        token.type, token.value, token.size);
}

void print_tokens(t_token *tokens) 
{
    int i = 0;
    while (tokens[i].type != TOK_END) {
        print_token(tokens[i]);
        i++;
    }
    print_token(tokens[i]); // Print END token
}

void test_basic_tokenization(void)
{
    t_arena arena = arena_new();
    t_token *tokens;

    // Single word
    tokens = tokenize(arena, "hello");
	print_tokens(tokens);
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(strcmp(tokens[0].value, "hello") == 0);
    assert(tokens[1].type == TOK_END);

    // Multiple words
    tokens = tokenize(arena, "hello world test");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(strcmp(tokens[0].value, "hello") == 0);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_WORD);
    assert(tokens[3].type == TOK_END);

    // Weird spacing
    tokens = tokenize(arena, "   hello     world   ");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_END);

    // Tab characters
    tokens = tokenize(arena, "hello\tworld\t\t\ttest");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_WORD);
    assert(tokens[3].type == TOK_END);

    printf("Basic tokenization tests passed!\n");
	arena_dispose(&arena);
}

void test_quotes(void)
{
    t_arena arena = arena_new();
    t_token *tokens;

    // Double quotes
    tokens = tokenize(arena, "echo \"hello world\"");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(strcmp(tokens[1].value, "\"hello world\"") == 0);

    // Single quotes
    tokens = tokenize(arena, "echo 'hello world'");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(strcmp(tokens[1].value, "'hello world'") == 0);

    // Nested quotes
    tokens = tokenize(arena, "echo \"hello 'world'\"");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);

    // Quotes with spaces
    tokens = tokenize(arena, "   \"hello   world\"    'test'   ");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_END);

    // Empty quotes
    tokens = tokenize(arena, "\"\" ''");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_END);

    printf("Quote tests passed!\n");
	arena_dispose(&arena);
}

void test_operators(void)
{
    t_arena arena = arena_new();
    t_token *tokens;

    // All operators
    tokens = tokenize(arena, "|| >> << && > < | ( )");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_LOGICAL_OR);
    assert(tokens[1].type == TOK_APPEND);
    assert(tokens[2].type == TOK_HEREDOC);
    assert(tokens[3].type == TOK_LOGICAL_AND);
    assert(tokens[4].type == TOK_REDIRECT_OUT);
    assert(tokens[5].type == TOK_REDIRECT_IN);
    assert(tokens[6].type == TOK_PIPE);
    assert(tokens[7].type == TOK_OPEN_PAREN);
    assert(tokens[8].type == TOK_CLOSE_PAREN);

    // Operators with words
    tokens = tokenize(arena, "cat file1 >> file2 && cat file2");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[2].type == TOK_APPEND);
    assert(tokens[4].type == TOK_LOGICAL_AND);

    // Operators with no spaces
    tokens = tokenize(arena, "cat>file&&ls|wc");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_REDIRECT_OUT);
    assert(tokens[2].type == TOK_WORD);
    assert(tokens[3].type == TOK_LOGICAL_AND);
    assert(tokens[4].type == TOK_WORD);
    assert(tokens[5].type == TOK_PIPE);
    assert(tokens[6].type == TOK_WORD);

    printf("Operator tests passed!\n");
	arena_dispose(&arena);
}

void test_error_cases(void)
{
    t_arena arena = arena_new();
    t_token *tokens;

    // Unclosed quotes
    tokens = tokenize(arena, "echo \"hello");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);  // "echo"
    assert(tokens[1].type == TOK_ERROR); // unterminated quote
    assert(tokens[2].type == TOK_END);

    tokens = tokenize(arena, "echo 'hello");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);  // "echo"
    assert(tokens[1].type == TOK_ERROR); // unterminated quote
    assert(tokens[2].type == TOK_END);

    // Adjacent operators - these are valid at tokenizer level
    tokens = tokenize(arena, "|||");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_LOGICAL_OR);  // ||
    assert(tokens[1].type == TOK_PIPE);        // |
    assert(tokens[2].type == TOK_END);

    // Empty input still gets END token
    tokens = tokenize(arena, "");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_END);

    tokens = tokenize(arena, "     ");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_END);

    printf("Error case tests passed!\n");
	arena_dispose(&arena);
}

void test_edge_cases(void)
{
    t_arena arena = arena_new();
    t_token *tokens;

    // Very long input
    char *long_input = malloc(10000);
    memset(long_input, 'a', 9999);
    long_input[9999] = '\0';
    tokens = tokenize(arena, long_input);
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_END);
    free(long_input);

    // Mixed spaces
    tokens = tokenize(arena, "hello\n\t\r\f\vworld");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_END);

    // Quotes with operators
    tokens = tokenize(arena, "\">>\" '|' \"&&\"");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_WORD);

    // Complex combinations
    tokens = tokenize(arena, "echo \"hello>world\" >> 'file|name' && ls|wc -l");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_APPEND);
    assert(tokens[3].type == TOK_WORD);
    assert(tokens[4].type == TOK_LOGICAL_AND);
    assert(tokens[5].type == TOK_WORD);
    assert(tokens[6].type == TOK_PIPE);
    assert(tokens[7].type == TOK_WORD);
    assert(tokens[8].type == TOK_WORD);

    printf("Edge case tests passed!\n");
	arena_dispose(&arena);
}
void test_complex_operators(void)
{
    t_arena arena = arena_new();
    t_token *tokens;

    // Adjacent operators
    tokens = tokenize(arena, "echo a||>b ls");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_LOGICAL_OR);
    assert(tokens[3].type == TOK_REDIRECT_OUT);
    assert(tokens[4].type == TOK_WORD);
    assert(tokens[5].type == TOK_WORD);
    assert(tokens[6].type == TOK_END);

    // More tricky combinations
    tokens = tokenize(arena, "cat file1||>>file2");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_LOGICAL_OR);
    assert(tokens[3].type == TOK_APPEND);
    assert(tokens[4].type == TOK_WORD);

    tokens = tokenize(arena, "echo hi>><file");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_APPEND);
    assert(tokens[3].type == TOK_REDIRECT_IN);
    assert(tokens[4].type == TOK_WORD);

    // Operators right next to words
    tokens = tokenize(arena, "echo>>file&&ls||wc");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_APPEND);
    assert(tokens[2].type == TOK_WORD);
    assert(tokens[3].type == TOK_LOGICAL_AND);
    assert(tokens[4].type == TOK_WORD);
    assert(tokens[5].type == TOK_LOGICAL_OR);
    assert(tokens[6].type == TOK_WORD);

    // Mixed quotes and adjacent operators
    tokens = tokenize(arena, "echo \"test\">>file||'cat'<input");
    assert(tokens != NULL);
    assert(tokens[0].type == TOK_WORD);
    assert(tokens[1].type == TOK_WORD);
    assert(tokens[2].type == TOK_APPEND);
    assert(tokens[3].type == TOK_WORD);
    assert(tokens[4].type == TOK_LOGICAL_OR);
    assert(tokens[5].type == TOK_WORD);
    assert(tokens[6].type == TOK_REDIRECT_IN);
    assert(tokens[7].type == TOK_WORD);

    printf("Complex operator tests passed!\n");
    arena_dispose(&arena);
}

int test_tokenize(void)
{
    test_basic_tokenization();
    test_quotes();
    test_operators();
    test_error_cases();
    test_edge_cases();
	test_complex_operators();
    printf("All tests passed!\n");
    return 0;
}

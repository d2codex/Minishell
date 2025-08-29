/*
	██╗    ██╗██╗██████╗
	██║    ██║██║██╔══██╗
	██║ █╗ ██║██║██████╔╝
	██║███╗██║██║██╔═══╝
	╚███╔███╔╝██║██║
	╚══╝╚══╝ ╚═╝╚═╝
*/

#include <stdio.h>

typedef	enum {
	STATE_NORMAL,
	STATE_SINGLE_QUOTE,
	STATE_DOUBLE_QUOTE
} e_quote_state;

/*
	RULES:
		- from STATE_NORMAL, if ['] detected, we enter inside a single quote string
		- from STATE_NORMAL, if ["] detected, we enter inside a double quote string
		- from STATE_SINGLE_QUOTE, if ['] detected, back to STATE_NORMAL (means the single quotes are closed)
		- from STATE_DOUBLE_QUOTE, if ["] detected; back to STATE_NORMAL (means the double quotes are closed)

	NOTES:
	- [NESTED QUOTES]
		- single quotes PROTECT everything inside, even double quotes ["]
			ex: 'He said "hello"' -> the ["] are just regular characters
		- double quotes PROTECT everything inside, even single quotes [']
			ex : "I'm happy" -> the ['] is just a regular character
		- only the SAME quote type can close the quote block
			ex : "hello 'world'" -> the ['] inside doesn't close the ["...]
		- once inside quotes, ALL characters lose their special meaning (except the closing quote)
*/
e_quote_state	update_quote_state(e_quote_state current_state, char c)
{
	if (c == '\'' && current_state == STATE_NORMAL)
		current_state = STATE_SINGLE_QUOTE;
	else if (c == '"' && current_state == STATE_NORMAL)
		current_state = STATE_DOUBLE_QUOTE;
	else if (c == '\'' && current_state == STATE_SINGLE_QUOTE)
		current_state = STATE_NORMAL;
	else if (c == '"' && current_state == STATE_DOUBLE_QUOTE)
		current_state = STATE_NORMAL;
	return (current_state);
}

int	main(void)
{
	e_quote_state	state;

	state = STATE_NORMAL;
	state = update_quote_state(state, '"');
	printf("expected: [2], result: [%d]\n", state);

	state = update_quote_state(state, 'x');
	printf("expected: [2], result: [%d]\n", state);

	state = update_quote_state(state, '"');
	printf("expected: [0], result: [%d]\n", state);
}

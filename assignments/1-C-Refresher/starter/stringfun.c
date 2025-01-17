#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SZ 50

// prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);

// prototypes for functions to handle required functionality
int count_words(char *, int, int);
// add additional prototypes here
void reverse_string(char *, int, int);
void word_print(char *, int, int);

int setup_buff(char *buff, char *user_str, int len)
{
	char *buff_ptr = buff;
	char *str_ptr = user_str;
	int wlen = 0;
	bool in_whitespace = false;
	// iterate until we reach null pointer
	while (*str_ptr != '\0')
	{
		if (*str_ptr == ' ' || *str_ptr == '\t')
		{
			if (!in_whitespace)
			{ // only copy one space to buffer
				*buff_ptr++ = ' ';
				in_whitespace = true;
				wlen++;
			}
		}
		else
		{
			// copying normal characters to buffer
			*buff_ptr++ = *str_ptr;
			wlen++;
			in_whitespace = false;
		}
		str_ptr++; // to next character
	}
	// fill rest of buffer with dots
	while (buff_ptr < buff + len)
	{
		*buff_ptr++ = '.';
	}
	// user supplied string is too large
	if (wlen > len)
	{
		return -1;
	}

	return wlen;
}

void print_buff(char *buff, int len)
{
	printf("Buffer:  ");
	putchar('[');
	for (int i = 0; i < len; i++)
	{
		putchar(*(buff + i));
	}
	putchar(']');
	putchar('\n');
}

void usage(char *exename)
{
	printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len, int str_len)
{
	char *buff_ptr = buff;
	int wc = 0;
	bool in_word = false;

	// iterate through the buffer up to length of string
	for (int i = 0; i < str_len; i++)
	{
		if (*buff_ptr != ' ')
		{
			if (!in_word)
			{
				// we just started a new word in this condition
				wc++;
				in_word = true;
			}
		}
		else
		{
			// either space or within a word
			in_word = false;
		}
		buff_ptr++;
	}

	return wc;
}

void reverse_string(char *buff, int len, int str_len)
{
	char *start = buff;
	char *end = buff + str_len - 1;
	char temp;

	while (start < end)
	{
		temp = *start;
		*start = *end;
		*end = temp;
		start += 1;
		end -= 1;
	}
	return;
}

void word_print(char *buff, int len, int str_len)
{
	int wlen = 0;
	int wc = 0;
	char *buff_ptr = buff;
	bool in_word = false;

	printf("Word Print\n----------\n");

	// iterate through the buffer up to null terminator
	while (*buff_ptr != '.')
	{
		if (!in_word && *buff_ptr != ' ')
		{
			// keep track of word count
			in_word = true;
			wc++;
			wlen = 0;
			printf("%d. ", wc); // print word number
		}
		if (in_word)
		{
			if (*buff_ptr == ' ')
			{
				// end of word
				printf("(%d)\n", wlen);
				in_word = false;
			}
			else
			{
				// print character and increment length
				printf("%c", *buff_ptr);
				wlen++;
			}
		}
		buff_ptr++;
		// last case if the word isn't ended by space
		if (*buff_ptr == '.')
		{
			printf("(%d)\n", wlen);
		}
	}
	printf("\nNumber of words returned: %d\n", wc);
}

void word_replace(char *buff, char *search_word, char *replace_word, int len)
{
	char *buff_ptr = buff;
	char *search_ptr, *replace_ptr;
	int search_len = 0, replace_len = 0;
	char *start_ptr = NULL;
	char *intact_ptr = NULL; // marks the start of the rest of the string
	bool match_found = false;

	// get lengths of search and replace words
	for (search_ptr = search_word; *search_ptr != '\0'; search_ptr++)
		search_len++;
	for (replace_ptr = replace_word; *replace_ptr != '\0'; replace_ptr++)
		replace_len++;

	// error check for buffer overflow
	if (len - search_len + replace_len > BUFFER_SZ)
	{
		printf("REPLACEMENT WORD TOO LARGE: BUFFER OVERFLOW");
		exit(1);
	}

	// iterate through buffer and check for search at each character
	while (*buff_ptr != '\0' && !match_found)
	{
		search_ptr = search_word;
		char *match_ptr = buff_ptr;

		while (*match_ptr == *search_ptr && *search_ptr != '\0')
		{
			match_ptr++;
			search_ptr++;
		}

		// search was iterated to the end of the string, indicating a match was found
		if (*search_ptr == '\0' && (*match_ptr == '\0' || *match_ptr == ' '))
		{
			// copy address where match was found
			// also copy the start of the address
			start_ptr = buff_ptr;
			intact_ptr = match_ptr;
			// mark that a match was found
			match_found = true;
		}
		// move onto next character
		buff_ptr++;
	}

	// we have start index of where the match is; now need to make replacement
	// tmp buffer will be replace string + intact part of buffer + trailing periods
	if (match_found)
	{
		char *tmp_buff = malloc(BUFFER_SZ * sizeof(char));
		if (tmp_buff == NULL)
		{
			printf("faill");
			exit(1);
		}
		buff_ptr = buff;
		char *tmp_ptr = tmp_buff;

		// copy up to the match
		for (char *iterator = buff_ptr; iterator < start_ptr; iterator++)
		{
			*tmp_ptr++ = *iterator;
		}

		// copy replacement to temp buffer
		for (char *replace_ptr = replace_word; *replace_ptr != '\0'; replace_ptr++)
		{
			*tmp_ptr++ = *replace_ptr;
		}
		// copy rest of buffer to temp buffer
		for (char *iterator = intact_ptr; *iterator != '\0'; iterator++)
		{
			*tmp_ptr++ = *iterator;
		}
		// null terminate
		*tmp_ptr = '\0';

		char *src = tmp_buff;
		char *dst = buff;

		// modify the original buffer
		while (*src != '\0')
		{
			*dst++ = *src++;
		}
		*dst++ = '\0';
		// fill the rest of the buffer with dots
		while (dst < buff + BUFFER_SZ)
		{
			*dst++ = '.';
		}
		// reset buff_ptr for printing string, print out modified str
		printf("Modified String: ");
		buff_ptr = buff;
		while (*buff_ptr != '.' && buff_ptr != buff + BUFFER_SZ)
		{
			putchar(*buff_ptr);
			buff_ptr++;
		}
		putchar('\n');
		free(tmp_buff);
	}
	else
	{
		printf("Search word not found in the buffer.\n");
	}
}

int main(int argc, char *argv[])
{

	char *buff;			// placehoder for the internal buffer
	char *input_string; // holds the string provided by the user on cmd line
	char opt;			// used to capture user option from cmd line
	int rc;				// used for return codes
	int user_str_len;	// length of user supplied string

	// TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
	//       PLACE A COMMENT BLOCK HERE EXPLAINING
	//	this is safe since argc ensures enough arguments exist
	//	so the if block is short circuited and the body is
	//	executed if argc is less than 2
	if ((argc < 2) || (*argv[1] != '-'))
	{
		usage(argv[0]);
		exit(1);
	}

	opt = (char)*(argv[1] + 1); // get the option flag

	// handle the help flag and then exit normally
	if (opt == 'h')
	{
		usage(argv[0]);
		exit(0);
	}

	// WE NOW WILL HANDLE THE REQUIRED OPERATIONS

	// TODO:  #2 Document the purpose of the if statement below
	//  The purpose of this if statement is just validating
	//  and error checking user input. Specifically this
	//  checks if enough arguments were provided, if not
	//  it outputs the usage of this program to the user
	//  and exits the program.
	if (argc < 3)
	{
		usage(argv[0]);
		exit(1);
	}

	input_string = argv[2]; // capture the user input string

	// TODO:  #3 Allocate space for the buffer using malloc and
	//           handle error if malloc fails by exiting with a
	//           return code of 99
	//  CODE GOES HERE FOR #3
	buff = malloc(BUFFER_SZ * sizeof(char));

	if (buff == NULL)
	{
		exit(99);
	}

	user_str_len = setup_buff(buff, input_string, BUFFER_SZ); // see todos
	if (user_str_len < 0)
	{
		printf("Error setting up buffer, error = %d", user_str_len);
		exit(2);
	}

	switch (opt)
	{
	case 'c':
		rc = count_words(buff, BUFFER_SZ, user_str_len); // you need to implement
		if (rc < 0)
		{
			printf("Error counting words, rc = %d", rc);
			exit(2);
		}
		printf("Word Count: %d\n", rc);
		break;
	case 'r':
		reverse_string(buff, BUFFER_SZ, user_str_len);
		printf("Reversed string: ");
		char *end = buff + BUFFER_SZ;
		char *buff_ptr = buff;
		while (*buff_ptr != '.' && buff_ptr != end)
		{
			putchar(*buff_ptr);
			buff_ptr++;
		}
		putchar('\n');
		break;
	case 'w':
		word_print(buff, BUFFER_SZ, user_str_len);
		break;
	case 'x':
		if (argc < 5)
		{
			printf("Error: Missing arguments for -x flag.\n");
			usage(argv[0]);
			exit(1);
		}
		char *search_word = argv[3];
		char *replace_word = argv[4];
		word_replace(buff, search_word, replace_word, user_str_len);
		break;

	default:
		usage(argv[0]);
		exit(1);
	}
	print_buff(buff, BUFFER_SZ);
	free(buff);
	exit(0);
}

// TODO:  #7  Notice all of the helper functions provided in the
//           starter take both the buffer as well as the length.  Why
//           do you think providing both the pointer and the length
//           is a good practice, after all we know from main() that
//           the buff variable will have exactly 50 bytes?
//
//		specifying the size helps avoid potential buffer overflows. it also
//  	helps with flexibility, as the same function can work with buffers
//		of different sizes and reduces the number of "assumptions" being made
//		in function calls.

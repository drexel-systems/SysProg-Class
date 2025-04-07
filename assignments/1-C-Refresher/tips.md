## Help Information and Tips

#### Hints/Tips
Often time when string processing you should consider creating a state machine for your logic and flushing out how the transformations will work.  For example, the `print_words()` function needs to keep track of when words begin and end.  Given you pre-processed the input, for example `"  hello class"` would already have been transformed into `"hello class...........<to end of buffer>"` you can assume the first character is the start of a word.  You can then increment your word counter, and set start of word to false.  If you encounter a space, you set start of word to true.  If you don't encounter a space you print the character.  When just getting started sometimes its even helpful to map out an approach before you write code.  For example:

```text
int print_words(char *buff, int buff_len, int str_len)  where:
     - buff is a pointer to the buffer
     - buff_len is the total lenght of the buffer
     - str_len is the length of total characters in the buffer
               remember the buffer is padded with '.'

PRECONDITIONS: 
     if (str_len > buff_len) > sanity check, return error
     if (str_len == 0) buff is empty return 0 no words

RETURNS
     The count of the number of words

APPROACH
     word_count = 0    to start
     char_ctr = 0      lenght of current word
     at_start = true   this will capture a if we are at the start of a word

     loop over each character
         if (at_start == true)
            increment word_count
            set at_start = false  (we are processing a new word)

         let c = current_char_in_buffer

         if (c == ' ') we hit the end of the word
             print char_count    (length of current word)
             set char_ctr = 0    (start count for next word)
             set at_start = true (we are starting next word)

         else we are just encountering a regular character
             print c              (current character)
             char_ctr++           (add to length of current string)

      all characters processed
      print char_count            (this is for the last word)
      return word_count           (holds total number of words)
```

Notice how we have one boolean `at_start` that is used to manage state with respect to finding word boundaries. 



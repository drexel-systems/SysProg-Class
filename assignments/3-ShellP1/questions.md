1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

   > **Answer**: fgets() reads user input line by line, making it the ideal tool for an interactive command-line application where input is processed per command which is generally seperated by a new line. As said in the readme, "reading stops after an EOF or a newline." This translates well for a line by line application.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

   > **Answer**: Malloc involves dynamically allocated memory (allocation occurs at runtime whereas when using a fixed-size array the allocation happens during compilation). This can allow us to allocate memory based on user input size and also use heap memory as opposed to stack memory which is larger and more flexible.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

   > **Answer**: If we are processing a command without trimming spaces, we might now match it appropriately if the user entered a leading or trailing space in their input which makes for a defective shell. For example is I typed " ls" and the space wasn't trimmed, the shell wouldn't match "ls" since " ls" is not the same as "ls". It simply makes for cleaner command processing when we trim spaces.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

  > **Answer**: One redrection usecase is redirecting STDOUT to a file using > or >>. (dsh> echo "hi" > hi.txt) A challenge in implmenting this is handling the opening and closing of the file correctly. So things like checking if a file doesn't exist and making a new one if this is the case or overwriting one if it does exist. We also need to error check and ensure we have the right permissions to write to the file in the first place. Another redirection example is redirecting standard error to a file. (dsh> ls this_file_does_not_exist 2> error.log). A challenge here would be redirecting only STDERR while ensuring STDOUT is not involved in the redirection. The last example I will provide is if we redirect both STDOUT and STDERR to a file (dsh> ls this_file_does_nor_exist &> output.log). A challenge here is recognizing that both streams should be redirected by parsing the command properly (so knowing &> means redirect STDERR and STDOUT).

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

  > **Answer**: Redirection involves controlling where the ouput of a command goes or where the input comes from whereas piping is connecting the output of one command into the input of another. A key distrinction is that in redirection, the shell generally directs the input or output to or from a file, while in piping, the output of one command is passed directly to the input of another command and this output is not written to and did not originate from a file.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

  > **Answer**: This is important because we want to differentiate the regular, expected output of a program from the unintended, unexpected output for the purpose of debugging and knowing when something has gone awry. We may want to run scripts on the shell where we want to handle errors differently than normal outputs, and this can only be done when the two streams are distinct.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

  > **Answer**: The shell should output an error mesage on STDERROR and recognize the command returned a non-zero exit status. It would make sense to allow a user to merge both streams, and this can be done by running something like "dsh> cmd > output.txt 2>&1". The shell would need to parse the command properly (as stated in a prior answer) and recognize that both streams should be redirected.

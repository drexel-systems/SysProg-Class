## Assignment 2 Questions

#### Directions

Please answer the following questions and submit in your repo for the second assignment. Please keep the answers as short and concise as possible.

1. In this assignment I asked you provide an implementation for the `get_student(...)` function because I think it improves the overall design of the database application. After you implemented your solution do you agree that externalizing `get_student(...)` into it's own function is a good design strategy? Briefly describe why or why not.

   > **Answer**: Yes, I think adding a get_student function was a good design strategy. It was convenient for printing information on some student given an id and deleting a student. With deleting a student, it came in handy for verifying the student existed in the first place. While I only used it in that function, in a program about manipulating (inserting and deleting) a database of students, it is good to have a function to fetch some student from the databse given an id.

2. Another interesting aspect of the `get_student(...)` function is how its function prototype requires the caller to provide the storage for the `student_t` structure:

   ```c
   int get_student(int fd, int id, student_t *s);
   ```

   Notice that the last parameter is a pointer to storage **provided by the caller** to be used by this function to populate information about the desired student that is queried from the database file. This is a common convention (called pass-by-reference) in the `C` programming language.

   In other programming languages an approach like the one shown below would be more idiomatic for creating a function like `get_student()` (specifically the storage is provided by the `get_student(...)` function itself):

   ```c
   //Lookup student from the database
   // IF FOUND: return pointer to student data
   // IF NOT FOUND: return NULL
   student_t *get_student(int fd, int id){
       student_t student;
       bool student_found = false;

       //code that looks for the student and if
       //found populates the student structure
       //The found_student variable will be set
       //to true if the student is in the database
       //or false otherwise.

       if (student_found)
           return &student;
       else
           return NULL;
   }
   ```

   Can you think of any reason why the above implementation would be a **very bad idea** using the C programming language? Specifically, address why the above code introduces a subtle bug that could be hard to identify at runtime?

   > **ANSWER:** The scope of the student_t student variable is confined to the get_student function so it will only be on the stack when the function is executing. Once the function returns, the stack frame will be destroyed and the memory used for student would no longer be valid, so the address being returned is for a location which is no longer associated with the desired variable.

3. Another way the `get_student(...)` function could be implemented is as follows:

   ```c
   //Lookup student from the database
   // IF FOUND: return pointer to student data
   // IF NOT FOUND or memory allocation error: return NULL
   student_t *get_student(int fd, int id){
       student_t *pstudent;
       bool student_found = false;

       pstudent = malloc(sizeof(student_t));
       if (pstudent == NULL)
           return NULL;

       //code that looks for the student and if
       //found populates the student structure
       //The found_student variable will be set
       //to true if the student is in the database
       //or false otherwise.

       if (student_found){
           return pstudent;
       }
       else {
           free(pstudent);
           return NULL;
       }
   }
   ```

   In this implementation the storage for the student record is allocated on the heap using `malloc()` and passed back to the caller when the function returns. What do you think about this alternative implementation of `get_student(...)`? Address in your answer why it work work, but also think about any potential problems it could cause.

   > **ANSWER:** While correct in accomplishing the goal of the function, I think it can be problematic since the allocated memory will need to be freed. If this doesn't occur then this will be a memory leak, additionally this involves more overhead for the programmer and adds something else to worry about. So ultimately this is not a good implementation.

4. Lets take a look at how storage is managed for our simple database. Recall that all student records are stored on disk using the layout of the `student_t` structure (which has a size of 64 bytes). Lets start with a fresh database by deleting the `student.db` file using the command `rm ./student.db`. Now that we have an empty database lets add a few students and see what is happening under the covers. Consider the following sequence of commands:

   ```bash
   > ./sdbsc -a 1 john doe 345
   > ls -l ./student.db
       -rw-r----- 1 bsm23 bsm23 128 Jan 17 10:01 ./student.db
   > du -h ./student.db
       4.0K    ./student.db
   > ./sdbsc -a 3 jane doe 390
   > ls -l ./student.db
       -rw-r----- 1 bsm23 bsm23 256 Jan 17 10:02 ./student.db
   > du -h ./student.db
       4.0K    ./student.db
   > ./sdbsc -a 63 jim doe 285
   > du -h ./student.db
       4.0K    ./student.db
   > ./sdbsc -a 64 janet doe 310
   > du -h ./student.db
       8.0K    ./student.db
   > ls -l ./student.db
       -rw-r----- 1 bsm23 bsm23 4160 Jan 17 10:03 ./student.db
   ```

   For this question I am asking you to perform some online research to investigate why there is a difference between the size of the file reported by the `ls` command and the actual storage used on the disk reported by the `du` command. Understanding why this happens by design is important since all good systems programmers need to understand things like how linux creates sparse files, and how linux physically stores data on disk using fixed block sizes. Some good google searches to get you started: _"lseek syscall holes and sparse files"_, and _"linux file system blocks"_. After you do some research please answer the following:

   - Please explain why the file size reported by the `ls` command was 128 bytes after adding student with ID=1, 256 after adding student with ID=3, and 4160 after adding the student with ID=64?

     > **ANSWER:** Since each student_t record is 64 bytes, after adding the student with id=1, the starting offset will be 64 bytes, so 64-127 will contain the new student. So ls, which reports the logical size, will return 128 since 0-128 are "used". Then student with id=3 is added, 128-191 are skipped (still allocated just sparse) but 192-55 is written to, extending the file size to 256 bytes. When student with id=64 is added, bytes 4096 (64 \* 64) to 4195 are written to with a huge sparse file region.

   - Why did the total storage used on the disk remain unchanged when we added the student with ID=1, ID=3, and ID=63, but increased from 4K to 8K when we added the student with ID=64?

     > **ANSWER:** Students with ids 1,2 and 63 were added within the first 4096 bytes, so linux only allocated one block for the database. At id=64, the offset is 4096, so another block is allocated raising the disk size to 8k.

   - Now lets add one more student with a large student ID number and see what happens:

     ```bash
     > ./sdbsc -a 99999 big dude 205
     > ls -l ./student.db
     -rw-r----- 1 bsm23 bsm23 6400000 Jan 17 10:28 ./student.db
     > du -h ./student.db
     12K     ./student.db
     ```

     We see from above adding a student with a very large student ID (ID=99999) increased the file size to 6400000 as shown by `ls` but the raw storage only increased to 12K as reported by `du`. Can provide some insight into why this happened?

     > **ANSWER:** Student with id=99999 would be written at offset 6399936-6399999 causing the logical size to grow to 64 mb, but only one additional 4k block would be allocated by linux to actually store this new student resulting in the 12k returned by du.

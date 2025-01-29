## Introduction - How Things Really Work

The purpose of this folder is to break down how linux starts and executes a program you develop in `C`.  It does this via a series of examples that return a value to the shell after you execute the program.

The most basic example is in `ret21.c`:

```c
int main(){
    return 21;
}
```

This code simply returns the integer 21 from the `main()` function.  The questions we will be addressing are:

1. `main()` is nothing more than a standard `C` function, how does it get executed.
2. We are returning 21 from main, where does that go?  We will see its returned to the shell and its value can be shown using the `echo $?` command.
3. What happens when `main()` stops executing.

Now that the basics of `ret21.c` have been explained, we will examine other demonstrations to dig into the way this stuff really works.  The `ret22.c` and `ret23.c` files look into ways we can use systemcalls via the C standard library to exit programs differently.

From there we have a number of different demos `start24...28.c` that examine how we can bypass the need for a `main()` function itself.  The `start24.c` demonstration is interesting as we can see what happens when we dont properly exit without the help of the runtime library.  We fix this and dive deeper into what is going oin in demos `start25...28.c`.
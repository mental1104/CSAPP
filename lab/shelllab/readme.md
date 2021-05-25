# Shell Lab  

First up, unpack the package to anywhere you want.  
`tar -xf shelllab.tar`  

`tsh.c` is where we're going to jump into.  

we're going to implement 7 functions in the directory.  
+ `void eval(char *cmdline);`
+ `int builtin_cmd(char **argv);`
+ `void waitfg(pid_t pid);`
+ `void sigchld_handler(int sig);`
+ `void sigtstp_handler(int sig);`
+ `void sigint_handler(int sig);`
+ `void do_bgfg(char **argv);`

To see if your implementation is right, whenever you modify the file, you should run `make` in the terminal.  

Then you could run `make test[00-16]` and `make rtest[00-16]` to see if their outputs are identical.  

For example:  
`make test00`  
`make rtest00`  
when you input these two commands, and your terminal will output something.  
Your duty is to ensure their respective outputs are nearly the same(except for concrete PID etc.).  

---

## eval  

In fact, the book has already provided us with a simple, flawed version of shell:  

![](./pic/1.png)

So we simply paste most of its framework, and then we will implement some details. 

Apparently, it does not support signal, so it cannot avoid potential race and cannot reap the zombies spawned by background process. So what we should do is fix the code within the red retangular.  

```c
void eval(char *cmdline){
    ......
    sigset_t mask_one, mask_all, prev_one;
    sigfillset(&mask_all);
    ......
}
```
First, we need to define three sigset and fill the mask_all.  
`mask_one` is used to block child signal.  
`prev_one`, if you've already read chapter8, is used to store current blocked bit, which can recover to the initial status.    
`mask_all` is used to block every signal, which will be useful when we modify the global variable. In this case, it's `jobs`, conncected with `addjob` and `deletejob`. Everytime we use these two functions we have to block every signal, namely mask_all.  
```c
void eval(char *cmdline){
    ...
    if(!builtin_cmd(argv)){
        sigemptyset(&mask_one);
        sigaddset(&mask_one, SIGCHLD);

        sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
        if ((pid = fork()) == 0){
            ...
        }
        ...
    }
    ...
}
```
After the if-statement and right before fork(), we need to initialize mask_one with SIGCHLD, then block it and save the current blocked bit to prev_one.  
**It is to avoid race between child process to parent process**. If the child win the race, it would signal parent to `deletejob()`, which does nothing. However, this results in `addjob()` to add a non-exist job to the array `jobs[]`, which is pointless. So we need to ensure that addjob() happens before `deletejob()`, and the solutions is, as former, to block `SIGCHLD`.  

```c
if ((pid = fork()) == 0){
    sigprocmask(SIG_SETMASK, &prev_one, NULL);
    if(setpgid(0,0)<0){
        perror("SETPGID ERROR");
        exit(0);
    }
    ...
}
```

In the fork(), we should do more. We should know that there's no need for child process to block SIGCHLD. Otherwise, it will fail to signal the parent process that it has exited already, and consequently it would become a zombie. So we should unblock SIGCHLD or simply recover with prev_one.  

And before the `eval()` you would see that lecturer said that:
> Note: each child process must have a unique process group ID so that our background children don't receive SIGINT (SIGTSTP) from the kernel when we type ctrl-c (ctrl-z) at the keyboard.  

So we should add a group ID with their own, by `setpgid()`  


```c
        int state = (bg ? BG:FG);

        sigprocmask(SIG_BLOCK, &mask_all, NULL);
        addjob(jobs, pid, state, cmdline);
        sigprocmask(SIG_SETMASK, &prev_one, NULL);
        if(!bg)
            waitfg(pid);
        else
            printf("[%d] (%d) %s", pid2jid(pid),pid, cmdline);
```

And the rest is relatively easy.  
First we need to figure whether the cmdline is background or foreground. Luckily, the lecturer has already done this for us. So we simply define a state corresponding to the value of `bg`.  

As mentioned before, whenever we modify the global variable we need to block all signals. This is implemented as former.  

Finally, if it's a foreground process, we `waitfg` for all children process.  

If it's background process, we simply printout its jid, pid and cmdline.  

At first glance, the content of `printf` may require ingeneuity. However, identifying these format needs comparing two different output on your own and then modify, so don't worry.  


Complete code:
```c
void eval(char *cmdline) 
{
    char* argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    sigset_t mask_one, mask_all, prev_one;
    sigfillset(&mask_all);


    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if(argv[0] == NULL)
        return;
    
    if(!builtin_cmd(argv)){

        sigemptyset(&mask_one);
        sigaddset(&mask_one, SIGCHLD);

        sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
        if ((pid = fork()) == 0){

            sigprocmask(SIG_SETMASK, &prev_one, NULL);
            if(setpgid(0,0)<0){
                perror("SETPGID ERROR");
                exit(0);
            }
            
            if(execve(argv[0], argv, environ) < 0){
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        int state = (bg ? BG:FG);

        sigprocmask(SIG_BLOCK, &mask_all, NULL);
        addjob(jobs, pid, state, cmdline);
        sigprocmask(SIG_SETMASK, &prev_one, NULL);
        if(!bg)
            waitfg(pid);
        else
            printf("[%d] (%d) %s", pid2jid(pid),pid, cmdline);
        
    }
    return;

}
```

## builtin_cmd  

We may be inspirational if we look at the code provided by textbook:  
![](./pic/2.png)

So, all we need to do is to use standard `strcmp` to compare each string in the if-statement and then do something.  

We need to implement fg, bg, quit and jobs.  

bg, fg, jobs has their corresponding procedure and `jobs`'s function `listjobs()` is already applicable.  

So we simply insert function and then return 1. 
Note that quit is tantamount to exit, which never returns.  

```c
int builtin_cmd(char **argv) 
{
    if(!strcmp(argv[0],"quit"))
        exit(0);
    else if(!strcmp(argv[0],"jobs")){
        listjobs(jobs);
        return 1;
    }
    else if(!strcmp(argv[0],"bg")||!strcmp(argv[0],"fg")){
        do_bgfg(argv);
        return 1;
    }
    return 0;     /* not a builtin command */
}
```

## waitfg

Why would we not first implement do_bgfg? Since it's a builtin command and occurs in the latter testfile.  

This procefure is designed to wait foreground process, so we find the job by the argument pid and wait for a signal.  

To avoid the perfomance problem and "infinite-pause"
introduced in the book, we should not use `pause();` and `;` in the while loop. Instead, we use `sigsuspend(&mask)`.  

Notice we don't have to explictly `sigaddset(&mask, SIGCHLD)` here.

```c
void waitfg(pid_t pid)
{   
    sigset_t mask;
    sigemptyset(&mask);
    while(fgpid(jobs) == pid)
        sigsuspend(&mask);
    return;
}
``` 

Up to now, `make test04` still will result in a infinite loop since we haven't implement the handle for SIGCHLD properly. 

## sigchld_handler


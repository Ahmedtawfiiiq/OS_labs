#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

using namespace std;

int main()
{
    int defaultout = dup(1);

    int file_desc = creat("f3.txt", 0666);

    if (file_desc < 0)
    {
        perror("ls : create outfile");
        exit(2);
    }

    string s1 = "output to a file\n";
    write(file_desc, s1.c_str(), s1.size());

    dup2(file_desc, 1);
    close(file_desc);

    string s2 = "output to terminal\n";
    write(defaultout, s2.c_str(), s2.size());

    return 0;
}

// #include <unistd.h>

// int main(void)
// {
//   char *execArgs[] = { "echo", "Hello, World!", NULL };
//   execvp("echo", execArgs);

//   return 0;
// }

// char *execArgs[10];

// vector<string> test;

// test.push_back("ls");
// test.push_back("-a");
// test.push_back("-l");

// execArgs[0] = &test[0][0];
// execArgs[1] = &test[1][0];
// execArgs[2] = &test[2][0];
// execArgs[3] = NULL;

// pid_t pid;

// pid = fork();

// if (pid < 0)
// {
//     printf("A fork error has occurred.\n");
//     exit(-1);
// }
// else if (pid == 0) /* We are in the child. */
// {

//     printf("I am the child, about to call ps using execlp.\n");
//     execvp(execArgs[0], execArgs);
//     /*  If execlp() is successful, we should not reach this next line. */
//     printf("The call to execlp() was not successful.\n");
//     exit(127);
// }
// else /* We are in the parent. */
// {
//     wait(0); /* Wait for the child to terminate. */
//     printf("\nI am the parent.  The child just ended.  I will now exit.\n");
//     exit(0);
// }

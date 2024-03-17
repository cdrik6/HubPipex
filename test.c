#include <unistd.h>
#include <fcntl.h>

int main(char **, char **, char *envp[])
{
	fcntl(F_SETPIPE_SZ, );

	execve("/usr/bin/strace", (char []*){"strace", "-ff", "bash", "-c", "'yes | sleep 99999'"}, envp);
}

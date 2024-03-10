/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:57:35 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/06 23:23:31 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	execve_test(void);
// void	execve_test2(void);

int	main(void)
{
	execve_test();
	return (0);
}

void	execve_test(void)
{
	char	*args[2];

	// args[0] = "ls";
	// args[1] = "-l";
	// args[2] = NULL;
	 args[0] = "/bin/ls";
	 args[1] = NULL;
	
	printf("%d\n", execve("/bin/ls", args, NULL));
	printf("This line will not be executed.\n");
}

// void	execve_test2(void)
// {
// 	printf("Main program started\n");
//     char* argv[] = { "jim", "jams", NULL };
//     char* envp[] = { "some", "environment", NULL };
//     if (execve("./sub", argv, envp) == -1)
//         perror("Could not execve");
// }

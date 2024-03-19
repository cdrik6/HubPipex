Arthur, Axel, Alex, Alexandre, Leo, Medhi, Elsa
- on a envie d'ouvrir et de traiter l'infile et l'outfile des le debut, mais il faut pouvoir exit sans forcement sortir du programme en fonction des problemes d'ouverture, donc c'est a faire au niveau du child, exit depuis le child permet de ne pas exit le programme
- zsh n'est pas bash (126 permission denied vs 127 command not found sur la commande "")
- ls est une commande qui leak par construction
- ne pas confondre un leak de la commande et un leak de son programme
- si impossible de lire dans le pipe (outfile no_write par exemple) alors que quelque chose a ete ecrit de l'autre cote il y aura un SIGPIPE
- ou encore quand la première commande n’a pas fini de s’exécuter alors que la seconde,  elle, a fini (yes | head)
- bash renvoie l'exit-code de la derniere commande (l'idee c'est si jamais il y avait une suite, il y a l'info de la ou ca s'est arrete)
- echo $?
- pwd built_in function
- sleep 3 + sleep 2 = 3
- errno == echild quand il n'y plus de process child (merci Leo et le man)
- a chaque commande son pipe, "pas recyclable"
- /dev/sdtout comme outfile permet de sortir sur l'out
- waitpid(-1, &status, 0) = wait(&status), -1 = tous les process child, 0 = on attend, WNOHANG = on attend pas
- mnemo pour les pipe.fd : "lire, ecrire, compter" le but de l'ecole, donc lire = 0, ecrire = 1, (et stderr = 2)
- waitpid(pid...) = -1 (process pid over), waitpid(pid...) = pid (process pid en cours)
- valgrind --trace-children=yes --track-fds=yes 
- valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all --track-origins=yes
- function access utile ? la function open fait le boulot
- execve nettoie TOUT quand elle s'execute sans probleme (!= -1)
- apres un dup2(old, new) faire close(old), il ne sert plus, il est dans new
- la cmd ecrit d'abord dans le pipe (execution de la commande),puis lecture de son resultat dans le pipe (donc fd[1] puis fd[0], sens inverse)
- ./pipex NON "rm NON" cat out
- ./pipex out "rm out" cat cat out
- ./pipex out cat cat "rm out" out
- ./pipex out "rm out" "sleep 2" "echo toto" out
- !!! dup2((*pipex).fd[0], STDIN); dans la partie parent du child_out, il faut ca pour qu'un 'yes' precedent ne bloque plus la STDIN (fd[0] est vide, dernier pipe cree par la boucle) !!!
- ./pipex out wc cat bin/ls out (127)
- ./pipex out / "echo toto" cat out
- < out bin/. | echo toto | cat > out --> bash: bin/.: No such file or directory

./pipex out cath cakt lso "cat -ert" out

A faire
- < out /bin/. | echo toto | cat > out --> bash: /bin/.: Is a directory (126)
- < out / | echo toto | cat > out --> bash: /: Is a directory (126)
- < out cat | . > out --> 2 --> bash: .: filename argument required
                              .: usage: . filename [arguments]
- < out cat | . > --> 2 --> bash: syntax error near unexpected token `newline'
- < cat |
- ./pipex out / "echo toto" cat out

Testeurs
- bkody
- vfurmane
- scely

Ref

https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901

https://reactive.so/post/42-a-comprehensive-guide-to-pipex

https://www.codequoi.com/pipe-une-methode-de-communication-inter-processus/

https://www.codequoi.com/creer-et-tuer-des-processus-fils-en-c/

https://www.youtube.com/watch?v=tcYo6hipaSA&list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY&index=5


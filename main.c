#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#define MAX_LEN 128


//helper function to tokenize strings
void input_token(char *string, char *arg_array[]){
	int counter = 0;
	arg_array[counter] = strtok(string, " "); //tokenize first word
	//continuing until no more tokens
	while(arg_array[counter] != NULL && counter< MAX_LEN -1){
		counter++;
		arg_array[counter] = strtok(NULL, " "); //tokenize next
	}
}

//helper function to handle foreground execution and commands
void front_exec(char *arg_array[]){

	//create new fork process
	pid_t pid = fork();
	
	//fork failure
	if(pid<0){
		perror("fork error");
		return;
	}
	//execute using execvp(), give error output if command or file does not exist
	else if(pid==0){
		if(execvp(arg_array[0],arg_array)==-1){
			fprintf(stderr, "%s: No such file or directory.\n", arg_array[0]);
			exit(EXIT_FAILURE);
		}
	}
	else{
		int status;
		wait(NULL);//wait for child process to finish first
	}
}

//helper function to add cd functionality and change dirs
void change_dir(char *arg_array[]){

	int arg_count = 0;
	while(arg_array[arg_count] != NULL){
		arg_count++;
	}
	
	// if too many arguments are input
	if(arg_count>2){
		fprintf(stderr, "Too many arguments\n");
		return;
	}
	
	// go to home/<user>
	else if(arg_array[1]==NULL || !strcmp(arg_array[1],"~")){
		chdir(getenv("HOME"));	
	}

	// if no file found
       	else if(chdir(arg_array[1])==-1){
		fprintf(stderr, "%s: No such file or directory.\n", arg_array[1]);
	}
	
	else {chdir(arg_array[1]);
	}
}


int main() {

	//initializing array to store current working dir and hostname
	char cwd[MAX_LEN];
	char hostname[MAX_LEN];

	//get useername of the current user
	char *username;
	username=getlogin();
	
	int resume = 1; // for control of while loop
	
	while (resume)
	{
		if(getcwd(cwd,sizeof(cwd))==NULL||gethostname(hostname, sizeof(hostname))!=0||username == NULL){
                perror("error occurred");
                return 1;
        }
        	char prompt[3*MAX_LEN];

		//to store the string in prompt
        	snprintf(prompt,sizeof(prompt), "%s@%s: %s > ",username, hostname, cwd);

		char *reply = readline(prompt); //get user input

		//check if reply is NULL
		if(reply==NULL){
                        fprintf(stderr,"\nerror in readline\n");
                        continue;
                }

		//check if reply is empty string
		if(strlen(reply)==0){
			free(reply);
			continue;
		}


		//terminate ssi when user enters exit
		if(!strcmp(reply, "exit")){
			resume=0;
			free(reply);
			continue;
			}

		//for every other input, tokenize string and pass into helper fnctions	
		char *arg_array[MAX_LEN];
                input_token(reply,arg_array);

		// if cd is the first command from input
		if(!strcmp(arg_array[0],"cd")){
			change_dir(arg_array);
			free(reply);

		}
		// if pwd is input
		else if(!strcmp(arg_array[0],"pwd")){
			printf("%s\n",cwd);
			free(reply);
		}
		else{
			front_exec(arg_array);
			free(reply); //free allocated memory for user input
		}
	}
	return 0;
}

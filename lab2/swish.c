/* CSE 306: Sea Wolves Interactive SHell */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "mysyscall.h"
#include "swishHelperFunctions.c"




// Assume no input line will be longer than 1024 bytes
#define MAX_INPUT_BUFF_BUFF 1024

extern char **environ;


int main (int argc, char ** argv, char **envp) {



  /********************************************************************
  Command line parsing begins here
  First check to see if -d is defined, and debugging messages need to be printed.
   ********************************************************************/

  int finished = 0;
  char *prompt = "swish> ";
  char cmd[MAX_INPUT_BUFF_BUFF];
  char cmdTemp[MAX_INPUT_BUFF_BUFF];
  char *EXIT_CMD =  "exit\n";
  char cwd[MAX_INPUT_BUFF_BUFF]; 
  /********************************************************************
   Setting the commands that the shell recognizes 
   ********************************************************************/
  char *commandsNL[] = { "ls\n","cd\n","pwd\n"}; 
  char *commands[] = { "ls","cd","pwd"}; 
  





  /*******************************************************************
  Get the CWD path
  Add a space and a null terminator to the path
  Print the path 
  Print the 'swish' prompt
  ********************************************************************/
  int rv;
  char *theCWD = getcwd(cwd,sizeof(cwd));
  char *cursor;
  cursor = theCWD + strlen(theCWD);
  *cursor = ' ';
  cursor++;
  *cursor = '\0';
  cursor =NULL ;
  getCMDargs(commandsNL,1,argv,argc);

  //rv = write(1, theCWD, strlen(theCWD));
  //rv = write(1, prompt, strlen(prompt));

  while (!finished) {
  rv = write(1, theCWD, strlen(theCWD));
  rv = write(1, prompt, strlen(prompt));
  char last_char;
  int count;
  int iterator =0;
  int pid=-1;

    /********************************************************************
    Removing this should make the only exit occur from the proper exit.
    IE typing 'exit'.
    *********************************************************************
    if (!rv) { 
     finished = 1;
      break;
    } Ends while loop */
    
    
    /* read and parse the input put it into CMD */
    for(rv = 1, count = 0, cursor = cmd, last_char = 1;rv && (++count < (MAX_INPUT_BUFF_BUFF-1)) && (last_char != '\n'); cursor++) { 

      rv = read(0, cursor, 1);
      last_char = *cursor;
    } 
   
    *cursor = '\0';
  //Tokenize the commands by a space deilmeter store it into *p
  char cmdcpy[MAX_INPUT_BUFF_BUFF];
  strncpy(cmdcpy, cmd, strlen(cmd));
//  char *p = strtok(cmdcpy, " ");

  /********************************************************************
   Check the CMD string to see if the input matches any of the commands
   found in the commandsNL array.
+-   ********************************************************************/
    for(iterator= 0; iterator < 3; iterator++){

  /********************************************************************
   If the command is in the array, this line returns 0 so we ! to enter
   ********************************************************************/
      if (!strncmp(commandsNL[iterator],cmd,2)){
          

  /********************************************************************
   IF the command is CD
   ********************************************************************/
          if (strncmp(commandsNL[1],cmd,2) == 0){

          cursor = cmd; //Make a cursor to the CMD string
          cursor+=3; // Move the cursor past 'cd ' onto the argument for CD
          strcpy(cmdTemp,cursor); // Copy the value at location into the cmdTemp string (this now holds CD arguments)

          
          cursor =cmdTemp; // The copied over value still has '\n' so we make a cursor it
          cursor+=strlen(cmdTemp)-1; // Move the cursor to the end -1 (The location of the '\n')
          *cursor = '\0';  // Replace the '\n' with Null terminator
            if (strcmp(cmd,"cd\n") == 0){ // Base case CD needs to implement root CD
              /* TODO */
              printf("Needs to be implemented");
              return 1;
            } else 
                    if (chdir(cmdTemp) < 0) /* Fail case of opening directory */
                    {
                      printf("ERROR OPENING DIRECTORY:%s \n",cmdTemp);
                      return 1;

                    } else { 
                            theCWD = getcwd(cwd,sizeof(cwd));
                            cursor = theCWD + strlen(theCWD);
                            *cursor = ' ';
                            cursor++;
                            *cursor = '\0';
                        }
                
          }
//if the input is for a application (cat/ls) then we need to parse the arguments following it
      else if ((pid = fork()) < 0) printf("fork failed"); /* Fail case on fork */
      if (pid == 0){
        int j = 0, k = 0, in_index = 0, out_index = 0;
        char input[MAX_INPUT_BUFF_BUFF];
        char output[MAX_INPUT_BUFF_BUFF]; 
        for(j = 0; j < strlen(cmd); j++) {
          /**
          * Search for <, >, or |
          **/

          /* Handle input redirection */
          if(cmd[j] == '<') {
            int l = 0;
            if(cmd[j+1] == ' ') { 
              in_index = 2; 
            }
            else { 
              in_index = 1; 
            }
            for(k = j+in_index; k < strlen(cmd); k++) {
              if(cmd[k] != ' ') { 
                input[l] = cmd[k]; 
                l++; 
              }
              else { 
                break; 
              }
            } 
          input[l+1] = '\0';
          } //assign input buffer

          /* Handle output redirection */
          
          else if(cmd[j] == '>') {
            int l = 0;
            if(cmd[j+1] == ' ') { 
              out_index = 2; 
            }
            else{ 
              out_index = 1; 
            }
            for(k = j+out_index; k < strlen(cmd); k++) {
              if(cmd[k] != ' ') { 
                output[l] = cmd[k]; 
                l++; 
              }
              else { 
                break; 
              }
            }
      output[l+1] = '\0';
      } //assign output buffer
      else if(cmd[j] == '|') { } //piping
      }
      printf("Input: %s\n", input);
      printf("Output: %s\n", output);
       
      execvp(commands[iterator],argv);
        
        }

        }
      }
     


	  if(strncmp(cmd,EXIT_CMD,4) == 0) { 
      printf("Exiting. "); 
      finished = 1; 
      return 0; 
    }



  

    // Execute the command, handling built-in commands separately 
    // Just echo the command line for now
//    write(1, cmd, strnlen(cmd, MAX_INPUT_BUFF_BUFF));


  /*******************************************************************
  Print the CWD
  Print the prompt
  ********************************************************************/

  } /* End while */

  return 0;
}

/**
 *Dishant Bhatt
 *
 *
 */

#include "p2.h"



//Global Variables that will be used by the main, parse, and executePipe methods
char charArray[MAXCHARS];
char *wordArray[MAXITEM];
int inputFlag = 0, outputFlag = 0, pipeFlag = 0, andFlag = 0;
int pipe_index;
int char_count;
int andFlagFound = 0;
char *inputFile, *outputFile;
int outFile, inFile;
int status;
pid_t pid, childPid;






int main(){
    signal(SIGTERM,signalHandler);                  //Waits for kill signal
    DIR *dirp;                                      //Local variable used by main including ls -F, cd, and fork handling
    int dir_ptr;
    struct dirent *dp;
    int word_value; //what parse returns
    
    setpgid(0,0);   //sets p2 up in its own process group
    
    for(;;){
        printf("p2: ");
        word_value = parse(); //calls parser, stores return value in word_value
        
        //EOF found then exit
        if(char_count == -1){
            break;
        }
        
        /**If word value returns zero:
         *Check to see if there was '&' found
         *if there was we need to call the parser again rather then continue
         *in order to make sure the next commands are executed, if there is a new line after '&'.
         *
         *Otherwise just continue until the user has entered a command.
         */
        if(word_value == 0){
            if(andFlagFound > 0){
                word_value = parse();
            }
            else{
                continue;
            }
            
        }
        
        /**This portion of the code checks to see if there are an errors when it comes to redirection and pipes.
         *If there is a '<' or a '>' detected, but no accompanying file then it throws an error
         *If there are too many redirections or pipes, it also throws an error.
         */
        if((outputFile == NULL && outputFlag > 0) || outputFlag > 1){
            outputFlag = 0;
            fprintf(stderr, "Redirection not used properly: Too many redirections or no designated file.\n");
            continue;
        }
        
        if((inputFile == NULL && inputFlag > 0) || inputFlag > 1){
            inputFlag = 0;
            fprintf(stderr, "Redirection not used properly: Too many redirections or no designated file.\n");
            continue;
        }
        
        if(pipeFlag > 1){
            pipeFlag = 0;
            fprintf(stderr,"Too many pipes used. Please use maximum of 1 pipe.\n");
            continue;
        }
         
         
         
         
         /**This is the built in cd handler.
          *cd does not fork.
          *If there are no arguments supplied cd takes the user to the home directory
          *If there are arguments it checks to see if there is only one.
          *And then it executes cd with that argument as the destination
          */
        if(strcmp(wordArray[0],"cd") == 0){
            if(word_value > 2){
                fprintf(stderr,"Too many arguments passed to cd. Please use a maximum of 1 argument with cd.\n");
                continue;
            }
            
            if(word_value == 1){
                if(chdir(getenv("HOME")) != 0){
                    fprintf(stderr,"Home directory not found.\n");
                }  
            }
            
            if(word_value > 2){
                fprintf(stderr,"Too many arguments for cd.\n");
                exit(9);
            }
            
            if(word_value == 2){
                if(chdir(getenv(wordArray[1])) < 0){
                    fprintf(stderr,"%s no such directory.\n",wordArray[1]);
                           
                }
            }
            
            continue;
        }
        
        
        /**This is the built in ls-F handler
         *If there are no arguments, it opens up the current directory and prints out the contents. Just the names
         *If there are arguments, it takes the first argument, ignores the rest, and uses it as the directory.
         *It then prints out the contents of that directory.
         */
        if((strcmp(*wordArray,"ls - F") == 0) || (strcmp(*wordArray,"ls-F") == 0)){
            int i = 0;
    
            if(wordArray[1] == NULL){
        
                if((dirp = opendir(".")) == NULL){
                    fprintf(stderr,"Cannot open directory.\n");
                    exit(9);
                }
        
            while(dirp){
                if((dp = readdir(dirp)) != NULL){
                    printf("%s ",dp->d_name);
                    i++;
                }
                else{
                    printf("\n");
                    closedir(dirp);
                    break;
                
                }   
            
            
            }
        
        
            }
            else{
        
                if((dirp = opendir(wordArray[1])) == NULL){
                    fprintf(stderr,"Cannot open directory.\n");
                    exit(9);
                }
        
                while(dirp){
                    if((dp = readdir(dirp)) != NULL){
                        printf("%s ",dp->d_name);
                        i++;
                    }
                    else{
                        printf("\n");
                        closedir(dirp);
                        break;
                    }
                }
        
            }
    

    
    
            continue;
    
    
    
        }

       
        //If a pipe was found it calls executePipe instead of our default 
        if(pipeFlag == 1){
            executePipe();
            continue;
        }
        
        
        fflush(stdout);
        fflush(stderr);
        
        childPid = fork(); //creates a child of p2
        
        //In case fork failed
        if(childPid < 0){
            fprintf(stderr,"Cannot fork. Exiting Program.\n");
            exit(1);
        }
        else if(childPid == 0){
            
            /**If a '>' was found.
             *This opens the appropriate file.
             *The flags allow creation, reading, and writing to the file.
             */
            if(outputFlag == 1){
            int flags = O_CREAT | O_EXCL | O_RDWR;
            
            if((outFile = open(outputFile,flags,S_IRWXU)) < 0){
                fprintf(stderr,"%s :File already exists.\n",outputFile);
                outputFlag = 0;
                continue;
            }
            
               
            
        }
        
        
        /**If a '<' was found.
         *This opens up to appropriate file to read from.
         *It also makes sure the file exists.
         *The flags allow only reading from the file.
         */
        if(inputFlag == 1){
            int flags = O_RDONLY;
            
          
            if((inFile = open(inputFile,flags)) < 0){
                fprintf(stderr,"%s :File does not exist.\n",inputFile);
                inputFlag = 0;
                continue;
            }
            
            
        }
        
        /**If there is no redirection of input and a '&' was found:
         * Opens /dev/null and redirects std in to /dev/null.
         * This is used for putting processes in the background.
         *
         */
            if(inputFlag == 0 && andFlag == 1){
                int devnull;
                int flags = O_RDONLY;
                
                if((devnull = open("/dev/null",flags)) < 0){
                    fprintf(stderr,"/dev/null cannot be opened.\n");
                    exit(9);
                }
                dup2(devnull,STDIN_FILENO);
                close(devnull);
            }
            
            /**The following lines of code, set input files and output files
             *to stdin and stdout accoudingly.
             *This way p2 reads and writes from the files if redirection was found. 
             */
            if(outputFlag == 1){
                dup2(outFile,STDOUT_FILENO);
                close(outFile);
            }
            
            if(inputFlag == 1){
                dup2(inFile,STDIN_FILENO);
                close(inFile);
            }
            //executes the command      
            if((execvp(*wordArray,wordArray)) < 0){
                fprintf(stderr,"Invalid Command.\n");
                exit(9);
            }
            
            
        }
        
        
        else{
            //determines wether to wait or put process in the background and prints out childpid if necessary
            if(andFlag == 1){
            printf("%s [%d]\n",wordArray[0],childPid);
            andFlag = 0;
            continue;
            }  
            
            //waits until child has finished 
            while ((pid = wait(&status)) > 0);
            continue;
            
        }
        
       
    
        
        
        
    }
    
    //Kilss the child process and exits. 
    killpg(getpgrp(),SIGTERM);
    printf("p2 terminated.\n");
    exit(0);
        
    
    
}
        
    
/**This parser sets flags and puts valid command lines into an array for execution.
 * It returns the number of words on the command line
 * char_count is an universal variable. Its value changes everytime getword is called
 * If pipe is found the postion where it is found is NULL and the postion after is pipe_index.
 * This allows us to execute two commands in our executePipe.
 * After all commands and arguments are entered the parser puts a NULL in te array so we know when the line ends.
 */

int parse(){
    int char_ptr = 0;
    int word_ptr = 0;
    int numWords = 0;
    char_count =0;
    int redirectOutFound = 0;
    int redirectInFound = 0;
    int pipeFound = 0;
    inputFlag = 0;
    outputFlag = 0;
    pipeFlag = 0;
    andFlag = 0;
    int i = 0;    
    
   

    while((char_count = getword(charArray + char_ptr)) > 0){
        
        
        if(*(charArray + char_ptr) == '&'){
            andFlag++;
            andFlagFound++;
            break;
        }
        else if(*(charArray + char_ptr) == '>'){
            outputFlag++;
            redirectOutFound = 1;
            char_ptr += char_count + 1;
            continue;
            
        }
        else if(redirectOutFound == 1){
            outputFile = charArray + char_ptr;
            redirectOutFound = 0;
            char_ptr += char_count + 1;
            continue;
        }
        else if(*(charArray + char_ptr) == '<'){
            inputFlag++;
            redirectInFound = 1;
            char_ptr += char_count + 1;
            continue;
        }
        else if(redirectInFound == 1){
            inputFile = charArray + char_ptr;
            redirectInFound = 0;
            char_ptr += char_count + 1;
            continue;
        }
        else if(*(charArray + char_ptr) == '|'){
            pipeFlag++;
            wordArray[word_ptr++] = NULL;
            word_ptr++;
            pipe_index = word_ptr;
            char_ptr += char_count + 1;
            continue;
        }
        
        
        wordArray[word_ptr] = charArray + char_ptr;
        
        
        numWords += 1;
        char_ptr += char_count + 1;
        word_ptr++;
   
        
        
        
        
        
    }
    
    
  
   
    wordArray[word_ptr] = NULL;  
    return numWords;
    

    



}

void signalHandler(){}; //In order to bypass defauly signal handler


/**This function gets executed when there is a pipe found instead of the default.
 *This uses nested or vertical piping in order to execute two commands and redirect their stdin and stdout to one another.
 *First it creates a pipe.
 *Then it forks and immediatly makes another child within that child.
 *This grandchild is going to the first command line, before the pipe.
 *Its stdout gets redirected to the pipe accordingly.
 *The child will then have its stdin redirected to the pipe accordingly and execute the second command line.
 *This funciton also handles if there is redirection before or after the pipe.
 *However, There cannot be an "output" redirection before the pipe and an "input" redirection after.
 *The parent the waits for the children to complete and continues with p2.
 */
void executePipe(){
    int files[2];
    pid_t firstChild, secondChild;
    
    
    fflush(stderr);
    fflush(stdout);
    if((firstChild = fork()) < 0){
        fprintf(stderr,"Cannot Fork.\n");
        exit(0);
    }
    if(firstChild == 0){
        pipe(files);
        
        fflush(stderr);
        fflush(stdout);
        if((secondChild = fork()) < 0){
            fprintf(stderr,"Cannot Fork./n");
            exit(0);
        }
        if(secondChild == 0){
            CHK(dup2(files[1],STDOUT_FILENO));
            CHK(close(files[1]));
            CHK(close(files[0]));
            
            
            if(inputFlag == 1){
                int Flags = O_RDONLY;
                
                if((inFile = open(inputFile,Flags)) < 0){
                    fprintf(stderr,"File does not exist.\n");
                    inputFlag = 0;
                    exit(9);
                }
                
            }
            
            if(inputFlag == 1){
                dup2(inFile, STDIN_FILENO);
                close(inFile);
                
            }
            
            
            if((execvp(*wordArray, wordArray)) < 0){
                fprintf(stderr,"Command Not Found.\n");
                exit(9);
            }
            
        }
        else{
            
            CHK(dup2(files[0],STDIN_FILENO));
            CHK(close(files[0]));
            CHK(close(files[1]));
            
            if(outputFlag == 1){
                int Flags = O_CREAT | O_EXCL | O_RDWR;
                
                if((outFile = open(outputFile,Flags,S_IRWXU)) < 0){
                    fprintf(stderr,"File already exists.\n");
                    outputFlag = 0;
                    exit(9);
                }
            }
            
            if(outputFlag == 1){
                dup2(outFile,STDOUT_FILENO);
                close(outFile);
            }
            
            if((execvp(*(wordArray + pipe_index),wordArray + pipe_index)) < 0){
                fprintf(stderr,"Command Not Found.\n");
                exit(9);
               
            }
            
        }
        
        
        
    }
    else{
        
         if(andFlag == 1){
            printf("%s [%d]\n",wordArray[0],firstChild);
            andFlag = 0;
            }
            else{ 
    
                while ((pid = wait(&status)) > 0);
                
            }
        
    
    }
    
    
    
    
}



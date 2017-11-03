/**
*CLASS: cs570
*
*
*/


#include "getword.h"
#include <stdio.h>
#include <stdbool.h>

int getword(char *w)
{
	int charCount = 0;		//This will be the number of characters in a word
	char checkChar = getchar();	//This gets the first character from stdin
	bool qouteFound = false;

	
	// This while loops skips leading tabs and spaces and iterates through the input until a non-tab character is found
	while(checkChar == '\t' || checkChar == ' ')
		{	
			checkChar = getchar();
		}

	/**
	* This while loop will check to see if the next character from stdin is word character 
	* It will then store this character in the array, using the charactercount as an index
	* This while loop also handles the case if a qoute is found 
	* It will then increment the character count and get the next character from stdin
	*/
	while(checkChar != '\t' && checkChar != '\n' && checkChar != EOF && checkChar != '<' && checkChar != '>' && checkChar != '&' && checkChar != '|' && checkChar != ';' && checkChar != ' ')
		{
			
				if(checkChar == '\'')
				{
					checkChar = getchar();
					qouteFound = true;
					
					while(checkChar != '\'')
					{
						if(checkChar =='\\')
						{
							checkChar = getchar();
							
							if(checkChar == '\'')
							{
								w[charCount] = checkChar;
								charCount++;
								checkChar = getchar();
							}
							else
							{
								w[charCount] = '\\';
								charCount++;
							}
							
						}
						
						if(checkChar == '\n' || checkChar == EOF || checkChar == '\'')
						{
							break;
						}
					
						
						
						w[charCount] = checkChar;
						charCount++;
						checkChar = getchar();
						
						
						
						
					}
					
				}
				
			
				if(checkChar == '\'')
				{
					qouteFound = false;
			        checkChar = getchar();
					continue;
				}
				
				if(checkChar == EOF || checkChar == '\n')
				{
					break;
				}

	

					
				
			if(checkChar == '\\')
			{
				checkChar = getchar();
				
				if(checkChar == '\n' || checkChar == EOF)
				{
					break;
				}
				
				w[charCount] = checkChar;
				charCounnd t++;
				checkChar = getchar();
				continue;
				
			}
			
			

		
			
			w[charCount] = checkChar;
			charCount++;
			if(charCount == 254)
			{
				break;
			}
			checkChar = getchar();


		}
	
	/**
	 * If a word ending character is found, the word size is returned (If a word was found)
	 * ungetc gets called so the character will be put back into stdin and is read the next time getword.c is called
	 * If a word was not found and the character count is zero, it returns one and outs the character in the array
	 */
	if(checkChar == '<' || checkChar == '&' || checkChar == ' ' || checkChar == '|')
	{
		if(charCount != 0)
		{
			ungetc(checkChar,stdin);
			w[charCount] = '\0';
			return charCount;
		}
		
		
		if(charCount == 0)
		{
			w[charCount] = checkChar;
			charCount++;
			w[charCount] = '\0';
			return 1;
		}
		
	}
	
	/**
	 * For the special case of a '>' character we have to check if it is a '>' alone or an '>!'
	 * First it checks if a word was found. If it was it calls ungetc so '>' will be put back into stdin and returns the word size
	 * Then it determines if it is the special case or not and returns the right value
	 */
	if(checkChar == '>')
	{
		
		if(charCount != 0)
		{
			ungetc(checkChar,stdin);
			w[charCount] = '\0';
			return charCount;
			
		}
		checkChar = getchar();
		
		if(checkChar == '!')
		{
			w[charCount] = '>';
			charCount++;
			w[charCount] ='!';
			charCount++;
			w[charCount] ='\0';
			return 2;

		}
		else
		{
			w[charCount] = '>';
			charCount++;
			w[charCount] = '\0';
			return 1;
		}
	
	}
	
	
	/**
	* This if statement checks to see if a newline or a semi-colon was encountered directly after a word was found.
	* If true, ungetc is called to return the newline back into stdin so the proper return will happen next time getword.c is called
	* It then puts a null character after the found word in the array and returns the number of characters.
	*/
	if((checkChar == '\n' || checkChar == ';') && charCount != 0)
		{
			ungetc(checkChar, stdin);
			w[charCount] = '\0';
			return charCount;

		}	
	/**
	* This checks if a newline or semi-colon was found not directly after a word.
	* If true it puts a null character in the array and returns 0
	*/
	if((checkChar == '\n' || checkChar == ';') && charCount == 0)
		{	
			//w[charCount] = '\n';
			w[charCount] = '\0';
			return 0;
		}

	/**
	* If a newline was not found at all the current word is ended with a null character and the character count is returned.
	*/
	if(charCount != 0)
		{
			w[charCount] = '\0';
			return charCount;
		}

	/**
	* Finally, if a EOF is found then a null character is added to the array and -1 is returned.
	*/
	if(checkChar == EOF)
		{	
			w[charCount] = '\0';
			return -1;
		}



}










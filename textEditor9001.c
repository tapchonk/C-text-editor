/*
#############################################################################################################################################################################
##IMPORTANT! When compiling this remember to include the ncurses library as it's not a standard C library use "gcc -lncurses textEditor9001.c -o textEditor9001" IMPORTANT!##
#############################################################################################################################################################################

A general text editor which mixes in some extra UI elements using the ncurses library to make the quality of life for users significantly better.
However, whilst we do use some UI elements the general functionaility used to edit and perform line operations is still at its core a command line based interface.
To achieve the requirements of the design specification I took advantage of C's good implementation of a file handling system. This makes, writing to, reading from and 
appending text files a breeze. 

A description for the way in which each function is carried out is as follows:

To choose a function for the program to carry out the user can press the associated key alongside its function. So e.g. to create a new file the user can press "1".

File creation:
Prompt the user to enter an approriate file name. If the file already exists in the current directory then the program still accepts the input but nothing changes in the directory.
We use the fopen function to open the file along with the "w" keyword and specify a name based on the user input. The file is then created in the directory.
For general quality of life, the program auto appends the ".txt" file type into the inserted name using malloc and realloc to allocate the correct memory for the char array used to store the string
containing the name of the file.

File deletion:
We output all the files in the current directory with the ".txt" extension using strstr() (we can check if a string contains a substring using this) and prompt the user to enter
a valid file name. In an error case where the user attempts to delete a file not within the current directory then the program will output "invalid error" and then prompt the user
to input another file name until an appropriate one is found. The way we can achieve this is simply by checking if using the fopen function on the file and trying to read from it
returns "NULL". Once a valid file name has been inputted we use the remove() function in C to remove the file.
The outputting of the current text files in the current directory, in my opinion, was an absolute necessity. This was due to the fact that a user cannot delete what they cannot view
within the terminal. Also, checking for the ".txt" extension is helpful, as it reduces the risk of a user attempting to open an incompatible file type. Time complexity O(1).

File copying:
(All functions that require the user to input the name of a file to edit, copy or delete use the same input system as the one described in the "File deletion" section above. They all have the same
error checking/handling properties and all visually look the same)
Copying required that the user input two things: the name of the file they want to copy and the name of the file they want to copy it to.
This required two inputs from the user which was done using the wgetstr() function in the ncurses library.
The file name that the user wants to then copy to is created to be written to using fopen with the "w" pointer.
From this point we have to declare two pointers to achieve the desired copying effect: one to read text from the copied file and one to write text to the file we are copying to.
For every character in the file we read from we use fgetc to get the next character within the read file "r" we then insert the same character to the pointer "w" of the newly created file using fputc.
Until we reach the end of the line at which point the function terminates. Time complexity O(n) (where n is the size of the file.) as we have to copy every element from on file to another.

File viewing:
Prompt the user to input the name of the text file they want to open. We then access the file using the fopen function with the pointer starting at the beginning of the file using "r" in the fopen function.
Then, for every character in the file we output by using fgetc and outputting each character to the terminal window.
For extra functionality, I added a word and character count that works as follows:
For counting words we just count spaces alongside instances of \n within the file. Whilst it may not be the most accurate way of doing this, it works surprisingly well.
Counting characters was simpler as we only needed to keep track of the number of times we used fgetc within the while loop before reaching the end of the file. Time complexity O(n) (where n is the size of the file)

Appending Lines:
Appending a line to the end of a file is done in the editing window. The user can input a line, press enter, and then the line is appended to the end of the file automatically (similar to autosave).
We take in the user input in the form of a char array of size 1024 (we don't use the entirety of the array in most cases, this is mostly for redunancy as it is extremely unlikely
that a user would want to insert a thousand characters on a single line.). Afterwards, we access the file using the fopen function with the "a" or append pointer specification so that the pointer
points towards the end of the file. Then one bye one, we copy the text from our char array and use fputc() to write the appended text to the file. We use recursion and repeat the process 
until we reach the total strlen() of the user input. . Time complexity in this case is O(k) (where k is the size of the user input.)

Insert Lines:
Inserting lines at a specific location is significantly more difficult and time complex than appending. The way we do this is as follows:
1.) Declare two pointers: one at the start of a temporary file to briefly store the new updated file and write to it and another to read from the start of the file we want to append.
2.) We copy every element in our old file to the temporary text file. Exacxtly the same as we would when copying an entire file up to the point we reach a new line.
3.) Once we reach the line we want to insert to (we can keep track of what line we're on by counting the number of times the char we return from fgetc is equal to '\n') we copy the user inputted text into that line
    alongisde a '\n' at the end.
4.) Continue copying from where we left off in the old file into the temporary text file.
5.) At the end of the old file, we delete the old file and we rename the temporary text file to that of the old files.
Time complexity in this function is O(n + k) (where n is the length of the old file and k is the length of the line we're inserting)

Deleting lines:
Deleting lines is done by prompting the user for the specific line number they want removed from the text file. 
1.) Similarly to insertion, we have two pointers for reading from the beginning of the old file and writing to the start of a new temporary text file.
2.) Using fgetc and fputc we copy the elements from the old text file to the new one.
3.) Once we reach the line we want to remove we keep iterating the fgetc function but cancel the fputc operation to the new text file until we reach the next point
    at which the character is pointing to a '\n' at which point we don't copy the elements of the removed line to the new file.
4.) Continue copying characters to the updated file using fputc.
5.) Remove the old file and rename the temporary text file to that of the old one. Time complexity is O(n)

View lines:
There is no explicit function to view a certain line which is where ncurses comes in. The editing window shows the contents of the text file while your editing it along with its associated line number.
This in my opinion is a significant quality of life improvement as opposed to a solution involving the use of line searching. This is because the state of the contents of the file are being shown as the file is changing.
The output is a reflection of the current state of the file which will make users lives easier as they don't have to memorise where they put what text where.

Change log:
To record the changes done on files by by users, I used a separate text file which is created when the user runs the program for the first time. Essentially, everytime a user performs any action involving a file
we record what function carried out was along with the name of the file that was changed and commit it to the "changeLog.txt" file via appending. The user can then view this file throught the program in order
to remember the last file they made changes to.

Number of lines:
Simply count the number of times '\n' appears when reading from the file. Output this number to the terminal once we have iterated through the entire file using fgetc. Time complexity is O(n)

My extra functions:

Edit line, copy/paste, show word and character counts.

Edit line (Called append line in the program):
This is quite similar to the insert line function, except that rather than just putting the line into the file and shifting lines below that line one down, we overwrite whatever was on that line.
This could be useful in a case where in a user made a simple mistake in a line and does not want to use both insert and delete functions to fix it.
In terms of functionality, it's exactly the same as the insert line function, except that once we add the text at that line we iterate through the file we're reading from until we reach the next line and
don't insert the line that was previously there. Time complexity is O(n)

Copy pasting:

For this I had to make use of heap memory in the form of calloc and the free function. This was because in order to temporarily store the line we wanted to copy I found that using a normal array was incredibly
buggy. During testing I found that characters that were previously in the buffer I used to store the line content were appearing in the file I was trying to paste to. Originally I declared an array of size 1024
similarly to how I would take inputs with previous functions. I found that previous characters in the buffer weren't being removed once the copy function was executed for whatever reason when declaring a new array.
So instead I used calloc to allocate some heap memory to store the char array generated by the copied line. Afterwards, I then freed this memory once it was no longer needed. I found this to be a much more rigorous
solution as I tested the copy function on some random text files and found that the "ghost characters" no longer appeared when copy pasting text.
There are two aspects of the copy paste function:
Firstly, extracting text from the line we wanted to copy. This was done by declaring a read pointer on our file and to iterate using fgets. We count the number of times we encounter the '\n' character until we reach
the desired line at which we record the characters in a heap memory stack using calloc.
Secondly, we perform the edit function on our text line and overwrite the text on the line with our line of text stored in our heap and overwrite the old contents. Time complexity is O(n)

Edge case testing:

In order to reduce the chances of a fatal error when file handling I tested a couple of scenarios where an edge case could occur.
1.) Trying to create a file that already has certain name: When I attempted this the program still accepted the input. However, it didn't crash and the file with name had no changes performed on it.
    As far as I could tell the program was fine.
2.) Trying to access a file that's not in the directory: The program stops us from doing this. If the fopen function with the file name returns "NULL" it asks the user for another input.
3.) Copy pasting from a line that doesn't exist in the file yet: I opened a file with 10 lines currently present. I then tried copying line 100 to line 5. Doing this made the copy function act similar
    to the delete function as we are deleting what was currently present on that line and replacing it with nothing as that line does not exist yet.
4.) Opening the program in an empty directory: When using the editing, copying or viewing functions I found that the user got stuck in those functions as there are no files to open. A possible solution would be to
    prompt the user to create a new file if the program detects that its current directory is empty.



*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> // for appending strings and general on strings such as string length
#include <dirent.h> // specifically for directory access to see what files are currently available to write to
#include <ncurses.h> // simple text based window creation, makes the program look a little nicer

int writeToChangeLog(const char* changeLogInput) // function used to write the change log given text to input as a constant character pointer
{
    FILE * fp;

    fp = fopen("changeLog.txt", "a");   // opens the changelog file
    for (size_t i = 0; i < strlen(changeLogInput); i++)
    {
        fputc(changeLogInput[i], fp);   // append the file and insert the text stored in the change log input pointer
    }
    fputc('\n', fp);
    fclose(fp); // close the file afterwards
    return(0);
}

int deleteFile()    // function that removes a user specified file from the current directory
{
    int validFileName = 0;

    char deleteSelectString[19];
    int row, col;
    int x = 1;
    DIR *folder;    // open the current directory that the program file is present in
    struct dirent *entry;
    int files = 0;

    folder = opendir(".");  // to specify the directory as the current directory

    if (folder == ((void *)0))  // in case the current folder is currently empty or in a case where we do not have read permissions in this directory
    {
        perror("Could not open file or file is empty");
        return(0);
    }

    initscr();
    getmaxyx(stdscr, row, col);
    WINDOW *win = newwin(row, col, 0, 0);   // initialise the command window
    refresh();
    mvwprintw(win, 0, 0, "Please enter the name of the file you would like to delete:");

    while(entry = readdir(folder))  // while loop to output the file names of all the text files in our current directory
    {  
        files++;
        if(strstr(entry->d_name, ".txt") != NULL)   //used to output only text files (we don't want users opening files with file extensions that might not be compatible withe the program)
        {
            mvwprintw(win, x, 0, "File %3d: %s", files, entry->d_name);
            x++;
        }  
    }

    int n = 0;
    int validInput = 0;

    while (validInput == 0)  // used to take file name input from the user and validate the input
    {
        mvwprintw(win, row/2 + n, col/2,"Input here: ");
        wgetstr(win, deleteSelectString);
        n++;
        if (fopen(deleteSelectString, "r") != NULL)
        {
            validInput = 1;
        }
        else
        {
            mvwprintw(win, row/2 - 1, col/2,"Invalid input");
        }
    }
    wrefresh(win);  // close window
    clear();
    endwin();
    closedir(folder);   // close the current folder

    remove(deleteSelectString); // deletes file from directory
    writeToChangeLog("The following file was deleted:");    // commit changes to change log file
    writeToChangeLog(deleteSelectString);

    return(0);
}

int editingWindow(const char* editSelectString, int editMode) // bulk of code is used for line operations on files, takes filename and edit mode as parameters
{
    int row, col;
    int x = 3, y = 0;
    char c;
    FILE *fpRead, *fpAppend;    // pointers to the file we want to edit or read from

    fpRead = fopen(editSelectString, "r");

    int stillEditing = 1;   // to change what edit function we are carrying out

    if (editMode == 1) // to update a single line
    {
        stillEditing = 2;
    }
    if (editMode == 2) // to delete lines
    {
        stillEditing = 3;
    }   
    if (editMode == 3) // to copy paste to a line
    {
        stillEditing = 4;
    }
    if (editMode == 4) // to insert text at a certain line
    {
        stillEditing = 5;
    }      
        
    initscr();
    getmaxyx(stdscr, row, col);
    WINDOW *win2 = newwin(row, col, 0, 0); // initilialise command window
    scrollok(win2, TRUE);   // allows us to scroll down when text goes too far down the screen
    refresh();
    wprintw(win2, "0 ");
    int lineCounter = 0;
    while(1)    // iteration to output current file text to the screen
    {
        c = fgetc(fpRead); // iterate through the characters in the text file
        if(c==EOF)
            break;
        else
        {
            wprintw(win2,"%c", c); // output each character
            x++;
            if(c == '\n')   // iterate through the line counter if we encounter the new line character
            {
                lineCounter++;
                x = 3;
                y++;
                wprintw(win2, "%u ", lineCounter);
                
            }
        }
    }

    fpAppend = fopen(editSelectString, "a"); // open current file in append mode
      
    while (stillEditing == 1) // check if the user wants to perform any of the line based commands within the program (also for appending a line to the end of the file)
    {
        char userLineInput[1024];
        wgetstr(win2, userLineInput);
        if (strstr(userLineInput, ".exit") && strlen(userLineInput) <= 6) // to close editing mode
        {
            wrefresh(win2);
            clear();
            endwin();
            fclose(fpRead);
            fclose(fpAppend);
            writeToChangeLog("The following file was edited:"); // commit to change log
            writeToChangeLog(editSelectString);
            return(1);
        }
        if (strstr(userLineInput, ".append") && strlen(userLineInput) <= 8) // it may be called "append" but it's actually an edit line function
        {
            wrefresh(win2);
            clear();
            endwin();            
            fclose(fpRead);
            fclose(fpAppend);
            writeToChangeLog("A line on this file was appended:"); // commit to change log
            writeToChangeLog(editSelectString);
            return(2);
        }
        if (strstr(userLineInput, ".delete") && strlen(userLineInput) <= 8) // to remove a line from the program
        {
            wrefresh(win2);
            clear();
            endwin();            
            fclose(fpRead);
            fclose(fpAppend);
            writeToChangeLog("A line on this file was deleted:"); // commit to change log
            writeToChangeLog(editSelectString);
            return(3);
        }
        if (strstr(userLineInput, ".copy") && strlen(userLineInput) <= 6) // to copy paste
        {
            flushinp();
            wrefresh(win2);
            clear();
            endwin();            
            fclose(fpRead);
            fclose(fpAppend);
            writeToChangeLog("A line on this file was copied and pasted:"); // commit to change log
            writeToChangeLog(editSelectString);
            return(4);
        }
        if (strstr(userLineInput, ".insert") && strlen(userLineInput) <= 8) // to insert a line to the program
        {
            wrefresh(win2);
            clear();
            endwin();            
            fclose(fpRead);     // it's necessity to shut down the windows and the file access once they're no longer required
            fclose(fpAppend);
            writeToChangeLog("A line was inserted to:");  // commit to change log
            writeToChangeLog(editSelectString);
            return(5);
        }
        y++;
        lineCounter++;
        wprintw(win2, "%u ", lineCounter);
        for (int i = 0; i < strlen(userLineInput); i++)
        {
            fputc(userLineInput[i], fpAppend); // copy text to the file
        }
        fputc('\n', fpAppend); // copy a new line character
        move(y, 3); // move the cursor back to the beginning (left side)

    }

    char currentChar;

    if (stillEditing == 2)  // to overwrite any line with new text
    {
        int appendLine;
        char appendInput[1024]; // declare a char array to temporarily store the user input
        FILE *temporaryFileAppend, *currentFileAppend;
        currentFileAppend = fopen(editSelectString, "r");   // pointers to read from the old file
        temporaryFileAppend = fopen("temporaryFile.tmp", "w"); // pointers to write to the temporary updated file
        wprintw(win2, "Input the line number you want to append: ");
        char appendLineInput[10];   // prompt user to input the line number they want changed
        int lineAppend = 0;
        wgetstr(win2, appendLineInput); // input line number
        appendLine = atoi(appendLineInput);
        wprintw(win2, "Input the text you want to insert (any text currently on that line will be overwritten): ");
        wgetstr(win2, appendInput); // input text a user wants inserted
        while(1)
        {
            currentChar = fgetc(currentFileAppend); // get the next character in the text file
            if(currentChar==EOF)
                break;
            else
            {
                if(lineAppend != appendLine) // check if we're on the line we want to update
                {
                    fputc(currentChar, temporaryFileAppend);    // copy characters from old file to new tmporary file
                }
                else
                {
                    for (int i = 0; i < strlen(appendInput); i++)   // for the length of the user input copy the text they inputted and commit it to the new file
                    {   
                        fputc(appendInput[i], temporaryFileAppend);
                    }
                    fputc('\n', temporaryFileAppend); // declare a new line to move onto the next line
                    while(1)
                    {
                        currentChar = fgetc(currentFileAppend); // to skip the text we want to overwrite we fgetc over to the next pointer
                        if (currentChar == '\n')
                        {
                            break;
                        }
                    }
                }
                if(currentChar == '\n') // iterate through the lines
                {
                    lineAppend++;  
                }
            }
        }
        fclose(temporaryFileAppend); // close pointer access
        fclose(currentFileAppend);
        remove(editSelectString);   // delete old file
        rename("temporaryFile.tmp", editSelectString); // rename updated file the same name as the old one
    }

    if (stillEditing == 3) // delete function
    {
        int removeLine;
        FILE *temporaryFile, *currentFile;
        currentFile = fopen(editSelectString, "r"); // pointers to copy from old file to new file
        temporaryFile = fopen("temporaryFile.tmp", "w");
        wprintw(win2, "Input the line number you want removed: ");  // prompt user to input line number to remove
        char removeLineInput[10];   
        wgetstr(win2, removeLineInput);
        removeLine = atoi(removeLineInput); // cast string input to an integer using atoi
        int line = 0;

        while(1)    // use iteration to copy old file to new file
        {
            currentChar = fgetc(currentFile);   
            if(currentChar==EOF)
                break;
            else
            {
                if(line != removeLine)  // as long as we are not currently on the line we want to delete we can commit the old text to the new file.
                {
                    fputc(currentChar, temporaryFile);
                }
                if(currentChar == '\n')
                {
                    line++;  
                }
            }
        }
        fclose(temporaryFile); // close pointer access
        fclose(currentFile);
        remove(editSelectString);   // delete old file 
        rename("temporaryFile.tmp", editSelectString); // rename temporary file same name as old file
        
    }

    if (stillEditing == 4) // to copy paste text
    {
        flushinp();
        char * textToCopy;
        textToCopy = (char*)calloc(1024, sizeof(char)); // to avoid bugs involving ghost characters allocate heap memory to store copied text
        FILE *temporaryCopyFile, *currentCopyFile, *getLine;
        getLine = fopen(editSelectString, "r");
        
        temporaryCopyFile = fopen("temporaryFile.tmp", "w");

        wprintw(win2, "Input the line number you want copied: ");   // prompt user to see which line they want to copy
        char copyLineInput[10];
        wgetstr(win2, copyLineInput);

        wprintw(win2, "Input the line number you want the text pasted to (it will be overwritten): "); // prompt user to see which line they want to paste to
        char pasteLineInput[10];
        wgetstr(win2, pasteLineInput);

        int lineNumberCopy = atoi(copyLineInput); // cast user input to int using atoi
        int lineNumberPaste = atoi(pasteLineInput);

        char c1, c2;      
        int line1 = 0, line2 = 0, i = 0;

        while (1)   // use iterative trick to save the copied text to our heaped memory
        {
            c1 = fgetc(getLine);
            if(c1==EOF)
                break;
            else
            {
                if(line1 == lineNumberCopy) // when we come across the line we want we copy each character to our heaped array
                {
                    textToCopy[i] = c1;
                    i++;
                }
                if(c1 == '\n')
                {
                    line1++;  
                }
            }
        }
        
        fclose(getLine); // close pointer access to old file
        currentCopyFile = fopen(editSelectString, "r");
        while(1)
        {
            c2 = fgetc(currentCopyFile);
            if(c2==EOF)
                break;
            else
            {
                if(line2 != lineNumberPaste)    // copy every element from old file to new one using fgets and fputs
                {
                    fputc(c2, temporaryCopyFile);
                }
                else
                {
                    for (int j = 0; j < strlen(textToCopy); j++) // once we reach the line we want to paste to we copy the text to the specified line
                    {   
                        fputc(textToCopy[j], temporaryCopyFile);
                    }
                    while(1)
                    {
                        if (c2 == '\n') // to overwrite the current line we don't commit/fput the current characters to the new file 
                        {
                            break;
                        }
                        c2 = fgetc(currentCopyFile);
                    }
                }
                if(c2 == '\n')  // iterate through the lines
                {
                    line2++;  
                }
            }
        }
        free(textToCopy);   // free heap memory we allocated to store the line text
        fclose(temporaryCopyFile); // close pointer access
        fclose(currentCopyFile);
        remove(editSelectString);   // delete old file
        rename("temporaryFile.tmp", editSelectString); // rename updated file to old file name

    }

    if (stillEditing == 5)  // to insert a line we take user line input and text input to commit it to the temporary file
    {
        int insertLine;
        char insertInput[1024];
        FILE *temporaryFileInsert, *currentFileInsert;  // file pointers to read and write from old and temporary files
        currentFileInsert = fopen(editSelectString, "r");
        temporaryFileInsert = fopen("temporaryFile.tmp", "w");
        wprintw(win2, "Input the line number you want to insert to: "); // prompt user for line insertion
        char insertLineInput[10];
        int lineInsert = 0; // variable to store line number for insertion
        wgetstr(win2, insertLineInput);
        insertLine = atoi(insertLineInput);
        wprintw(win2, "Input the text you want to insert: ");   // prompt user to input what text they want to insert
        wgetstr(win2, insertInput);
        while(1)    //to iterate through file until we reach the line we want to insert to
        {
            currentChar = fgetc(currentFileInsert);
            if(currentChar==EOF)
                break;
            else
            {
                if(lineInsert != insertLine)    // copy elements from old file to new file
                {
                    fputc(currentChar, temporaryFileInsert);
                }
                else
                {
                    for (int i = 0; i < strlen(insertInput); i++)   // once we reach the line we want to insert to we copy the user input to the file
                    {   
                        fputc(insertInput[i], temporaryFileInsert);
                    }
                    fputc('\n', temporaryFileInsert); // insert a new line to the file
                    lineInsert++;
                    fputc(currentChar, temporaryFileInsert);

                }
                if(currentChar == '\n') // iterate through the lines
                {
                    lineInsert++;  
                }
            }
        }
        fclose(temporaryFileInsert);
        fclose(currentFileInsert); // close file pointer access
        remove(editSelectString); // delete old file
        rename("temporaryFile.tmp", editSelectString); //rename updated file to old name of the old file
    }

    wrefresh(win2); // refresh current command window

    clear();    // clearing and ending window
    endwin();

    fclose(fpRead); // closing read access
    fclose(fpAppend); // closing write access

    return(0);
}

int editFileContents() // calls the editing window to enact certain line operations an functions
{
    initscr();  //initilise command window
    printw("Remember to input these commands to do the following functions: \n.insert (Insert a new line) \n.copy (Copy paste a line to another line) \n.edit (Edit a file at a specific line) \n.delete (Delete a specific line) \n.exit (Exit the program)");
    getch();
    clear();
    endwin();
    int validFileName = 0;

    char editSelectString[19];
    int row, col;
    int z = 1;
    DIR *folder;    // open current directory to view what text files are currently available to the user
    struct dirent *entry;
    int files = 0;

    folder = opendir("."); // to open the current directory the program is stored in

    if (folder == ((void *)0)) // in case of an empty folder or we don't have file permissions on the system
    {
        perror("Could not open file or file is empty");
        return(0);
    }

    initscr(); // initialise window to select which file to edit
    getmaxyx(stdscr, row, col);
    WINDOW *win = newwin(row, col, 0, 0);
    refresh();
    mvwprintw(win, 0, 0, "Please enter the name of the file you would like to edit:");

    while(entry = readdir(folder)) // output all the text files in current directory to the window
    {  
        files++;
        if(strstr(entry->d_name, ".txt") != NULL)
        {
            mvwprintw(win, z, 0, "File %3d: %s", files, entry->d_name);
            z++;
        }  
    }

    int n = 0;
    int validInput = 0;

    while (validInput == 0) // prompt the user to input an appropriate file name
    {
        mvwprintw(win, row/2 + n, col/2,"Input here: ");
        wgetstr(win, editSelectString);
        n++;
        if (fopen(editSelectString, "r") != NULL)   // in case the file they want to edit doesn't exist
        {
            validInput = 1;
        }
        else
        {
            mvwprintw(win, row/2 - 1, col/2,"Invalid input");
        }
    }

    wrefresh(win);
    clear();
    endwin(); // close command window
    closedir(folder); // close access to current directory
    int stillEditing = 999;
    while(stillEditing != 1)    // used to control what function is being run in the editing window as it runs
    {
        stillEditing = editingWindow(editSelectString, 0);
        if (stillEditing == 2)
        {
            stillEditing = editingWindow(editSelectString, 1);  // each one corresponds to a certain function e.g. append, delete, insert etc.
        }
        else if (stillEditing == 3)
        {
            stillEditing = editingWindow(editSelectString, 2);
        }
        else if (stillEditing == 4)
        {
            stillEditing = editingWindow(editSelectString, 3);
        }
        else if (stillEditing == 5)
        {
            stillEditing = editingWindow(editSelectString, 4);
        }
    }

    return(0);
}


void createNewFile() // to make a new file with a specified name
{
    FILE *fp;
    const char* fileName;
    int row, col;
    int validFileName = 0;
    char tempString[15];
    
    while(validFileName == 0) // to check for valid file names
    {
        
        initscr();

        getmaxyx(stdscr,row,col); // initialise window for output

        WINDOW *win = newwin(row, col, 0, 0);
        refresh();
            
        mvwprintw(win, 0, 0,"Please name your file. Maximum file name length is 14 characters and cannot contain spaces.\n");              
        wgetstr(win, tempString);   // to get the name of the file the user wants to create
        wrefresh(win);
        clear();

        endwin();        
        int spaceFound = 0; // to check for space in the file name
        for(int i = 0; i < 15; i++)
        {
            if((int)tempString[i] == 32)
                spaceFound = 1;
        }

        if(spaceFound == 0)
            validFileName = 1;
    }

    int nameLength = strlen(tempString); // save name in heap memory

    char *nameFinal;
    nameFinal = (char *) malloc(nameLength); // allocate heap memory in order to store our name
    strcpy(nameFinal, tempString);

    nameFinal = (char *) realloc(nameFinal, nameLength + 5); // realloc more memory to store the extra characters for the .txt extension
    strcat(nameFinal, ".txt"); // concatenate the .txt extension

    fp = fopen (nameFinal, "a"); // open file with the name we want
    fclose(fp);

    int validInput = 0;

    char userInput;

    while((int)userInput != 121 && (int)userInput != 110 ) // to check if the user wants to start editing the file
    {
        initscr();
        printw("Would you like to start editing this file? [y/n]:");
        userInput = getch();
        if ((int) userInput == 121) // for y
        {
            validInput = 1;
            editFileContents();
        }
        else if ((int) userInput == 110) // for n
        {
            validInput = 1;
        }
        clear();
        endwin();   // close the window
    }
    writeToChangeLog("A new file was created:"); // commit to the change log
    writeToChangeLog(tempString);
}

int copyFile() // function to copy a file to another file with identical contents
{
    FILE *fp1, *fp2;
    char c;

    int validFileName = 0;

    char copySelectString[19];
    int row, col;
    int x = 1;
    DIR *folder; // to access files within the current directory
    struct dirent *entry;
    int files = 0;

    folder = opendir(".");

    if (folder == ((void *)0)) // in case of an empty folder or no permissions for the current user
    {
        perror("Could not open file or file is empty");
        return(0);
    }

    initscr();
    getmaxyx(stdscr, row, col);
    WINDOW *win = newwin(row, col, 0, 0);
    refresh();
    mvwprintw(win, 0, 0, "Please enter the name of the file you would like to copy:"); //prompt user to input file name for duplication

    while(entry = readdir(folder)) // outputs contents of the directory
    {  
        files++;
        if(strstr(entry->d_name, ".txt") != NULL)
        {
            mvwprintw(win, x, 0, "File %3d: %s", files, entry->d_name);
            x++;
        }  
    }

    char text[] = "Input here: ";
    int n = 0;
    int validInput = 0;

    while (validInput == 0)
    {
        mvwprintw(win, row/2 + n, col/2,"%s",text); // to input the name of the file we want to duplicate
        wgetstr(win, copySelectString);
        n++;
        if (fopen(copySelectString, "r") != NULL)
        {
            validInput = 1;
        }
        else
        {
            mvwprintw(win, row/2 - 1, col/2,"Invalid input"); // in case the file name does not exist yet
        }
    }

    wrefresh(win);
    clear();
    endwin(); // end command window
    closedir(folder); // close directory access

    fp1 = fopen(copySelectString, "r");

    char tempString[15];
    initscr(); // initialise command window

    getmaxyx(stdscr,row,col); // declare command window size

    WINDOW *win2 = newwin(row, col, 0, 0);
    refresh();
    	
    mvwprintw(win2, 0, 0,"Please name your file. Maximum file name length is 14 characters and cannot contain spaces.\n");              
    wgetstr(win2, tempString);
    wrefresh(win2); // to refresh the current window
    clear();

    endwin(); // end current window

    int nameLength = strlen(tempString);

    char *nameFinal;
    nameFinal = (char *) malloc(nameLength); // allocation of heap memory to store name of new file
    strcpy(nameFinal, tempString);

    nameFinal = (char *) realloc(nameFinal, nameLength + 5); //allocate extra memory to store the .txt extension
    strcat(nameFinal, ".txt");

    fp2 = fopen (nameFinal, "a");

    c = fgetc(fp1); // read contents from original file
    while (c !=  EOF)
    {
        fputc(c, fp2); // input every element in the old file to the new copy
        c = fgetc(fp1); // get next element to be read from the file
    }
    fclose(fp1);
    fclose(fp2); //close file pointer access

    writeToChangeLog("File was copied:");
    writeToChangeLog(copySelectString); // commit changes to change log.

    return (0);
}


int outputFileContents(int changeLogCheck) // output contents of a file line by line
{
    FILE *fp1;
    char c;

    int validFileName = 0;  // variable declaration
    char * file;
    char outputContentSelection[19];
    int row, col;
    int x = 1;
    DIR *folder;
    struct dirent *entry;
    int files = 0;

    folder = opendir("."); // to select the file within the directory

    if (folder == ((void *)0)) // if the current directory is empty
    {
        perror("Could not open file or file is empty");
        return(0);
    }

    initscr();
    getmaxyx(stdscr, row, col);
    WINDOW *win = newwin(row, col, 0, 0);
    refresh();
    mvwprintw(win, 0, 0, "Please enter the name of the file you would like to open:"); // prompt user to input name of the file they want to access

    while(entry = readdir(folder))  // outputs names of the files currently up for viewing
    {  
        files++;
        if(strstr(entry->d_name, ".txt") != NULL)
        {
            mvwprintw(win, x, 0, "File %3d: %s", files, entry->d_name); // outputs file name to the command line window
            x++;
        }  
    }

    char text[] = "Input here: ";
    int n = 0;
    int validInput = 0;
    

    while (validInput == 0 && changeLogCheck != 1) // to check if a user has inputted an appropraite file name
    {
        mvwprintw(win, row/2 + n, col/2,"%s",text);
        wgetstr(win, outputContentSelection);
        n++;
        if (fopen(outputContentSelection, "r") != NULL) // if we return a NULL value when accessing a file then it must not be accessible fromt he current directory
        {
            validInput = 1;
        }
        else
        {
            mvwprintw(win, row/2 - 1, col/2,"Invalid input");
        }
        file = outputContentSelection;
    }

    if (changeLogCheck == 1) // open the change log if the user wants to check the updates done to certain files
    {
        file = "changeLog.txt";
    }  

    wrefresh(win);
    clear();
    endwin(); // close command window that's currently open
    closedir(folder);

    printf("this should work %s\n", file);
    
    fp1 = fopen (file, "r");

    initscr();
    getmaxyx(stdscr, row, col);
    WINDOW *win2 = newwin(row, col, 0, 0);
    refresh();
    keypad(win2, TRUE); // allow for scrolling if the file is too large to fit in the screen
    scrollok(win2, TRUE);
    int lineCounter = 0;
    int wordCounter = 0;
    int characterCounter = 0;
    while(1) // use iteration to ouput entire file to the window
    {
        c = fgetc(fp1);
        characterCounter++;
        if(c==EOF)  // at the end of the file stop
            break;
        else
        {
            wprintw(win2, "%c", c); // outputs characters to the window one by one until we reach the end of the file
            if(c == '\n')
            {
                lineCounter++;
                wordCounter++;
            }
            if(c == ' ') // to count words we count the number of spaces and new lines
            {
                wordCounter++;
            }
        }

    }
    wprintw(win2, "Number of lines: %u\n", lineCounter); // outputs line, word and character count
    wprintw(win2, "Number of words: %u\n", wordCounter);
    wprintw(win2, "Number of characters: %u", characterCounter);

    wrefresh(win2); // refreshes command window
    fclose(fp1);
    getch();
    clear();
    endwin(); //close command window

    return(0);
    
}

int main()
{
    if (fopen("changeLog.txt", "r") == NULL) // if there is not change log file then create one
    {
        fopen("changeLog.txt", "w");
    }
    char userInput;
    
    while((int) userInput != 55)
    {   
        initscr();
        printw("Press: \n(1) To create a new file\n(2) To copy a file\n(3) To delete a file\n");
        printw("(4) To edit a file\n(5) To view a file\n(6) To view the change log history \n(7) To exit the program");
        
        userInput = getch();
        if ((int) userInput == 49) // create new file
        {
            refresh();
            clear();
            endwin();
            createNewFile();
        }
        else if ((int) userInput == 50) // copy a file to a new file
        {            
            refresh();
            clear();
            endwin();
            copyFile();
            
        }
        else if ((int) userInput == 51) // delete file
        {           
            refresh();
            clear();
            endwin();
            deleteFile();
        }
        else if ((int) userInput == 52) // edit a file file
        {            
            refresh();
            clear();
            endwin();
            editFileContents();
        }
        else if ((int) userInput == 53) // to view file
        {
            refresh();
            clear();
            endwin();
            outputFileContents(0);
        }
        else if ((int) userInput == 54) // to view change log file
        {
            refresh();
            clear();
            endwin();
            outputFileContents(1);
        }

        refresh(); //close command window and reset
        clear();
        endwin();

    }
}
# crb_shell
A simple shell implementation

Internal Commands:
        cd <directory>: Change the directory to the given <directory>
        clr: clear the screen
        dir: list the contents of the current working directory
        environ: list some of the local system environment variables
        echo <arguments...>: print out to given arguments
        help: list helpful user information for the crb_shell program
        pause: pause the shell until the 'enter' key is pressed
        quit: quit the crb_shell program, and return to calling shell program
External Commands:
        All external commands are supported.
Input redirects: < or <<
        Usage: com1 args... < filename: This will use the contents of filename as the input for com1.
        Usage: com1 args... << filename: This will use the contents of filename as the input for com1.
Output redirects: > or >>
        Usage: com1 args... > filename: This will append the output of com1 to filename.
        Usage: com1 args... >> filename: This will truncate the output of com1 to filename.
Command piping: |
        Usage: com1 args... | com2 args...: This will link the output of com1 to the input of com2.
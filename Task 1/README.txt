Task 1 

Write a script modify with the following syntax:   

modify [-r] [-l|-u] <dir/file names...>   

modify [-r] <sed pattern> <dir/file names...>   

modify [-h]     


modify_examples 

which will modify file names. The script is dedicated to lowerizing (-l) file names, uppercasing (-u) file names or internally calling sed command with the given sed pattern which will operate on file names. Changes may be done either with recursion (-r) or without it. Write a second script, named modify_examples, which will lead the tester of themodify script through the typical, uncommon and even incorrect scenariosof usage of the modify script. 


Note that: 

Filename: readme.txt -> [-u] -> README.txt 
Error handling 

 
Incorrect call: modify –l –u filmename.txt -> INCORRECT -> print info that the call is incorrect! -> you can call within the script the same function you call when the modify –h (option) is selected. 
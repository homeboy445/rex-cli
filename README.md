# rex-cli

```
                         WELCOME TO REX!               

 Functionalities offered: 
  - Find all the occurrences of a certain word or a sentence in the selected codebase. 
  - Replace all the occurrences of a certain word or a sentence in the selected codebase by a something else. 
  - Revert the replacement changes done. 

 Usage: 
  - For finding all/some occurrences: 
       => rex [directory path('.' or 'full path')] --find [word or sentence] --all(for the whole directory)/--some [filePath1] [filePath2] ... 

  - For replacing all/some occurrences: 
       => rex [directory path('.' or 'full path')] --replace [word or sentence to be replaced] [word or sentence to be replaced with] --all(for the whole directory)/--some [filePath1] [filePath2] ... 

  - For reverting the changes made by the replace operation: 
       => rex [directory path('.' or 'full path')] --revert [unique code appended after '.' in the log file] 


 Terminologies: 
   --find      operation specification flag, used to find code snippets in the current working directory. 
   --replace   operation specification flag, used to replace code snippets with the provided ones in the current working directory.
   --revert    operation specification flag, used to revert the changes made by the replacement operation.
   --all       flag for considering every file in the current working directory.
   --some      flag for considering some selected few files in the current working directory.

   --display     flag for displaying the details of the operations being performed.

   --include-only     flag for only considering certain filetypes(specify file types only without a period).
   --except     flag for excluding certain file types(specify file types only without a period).


 WARNING: 
     '--revert' will only work if the files in which the changes via the replacement operation were made haven't changed.


 TIP: 
     * Just append '--display' flag at the end of your command to display the operation details.
     * To perform any operation on a certain file type, consider using '--include-only' flag, justadd it as a suffix to the whole command,like so : [whole command]-- include -only cpp java py...
     * To exclude any file(s) from the operation being performed,just add the flag '--except' and the file types, like so[whole command]-- except cpp java py... 
     * By default the operation scope would be '--all', unless specified otherwise.


```

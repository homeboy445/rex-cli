# rex-cli

#### This cli is a code maneouvering tool that can be used to find words or sentences within a certain code-base easily(much like grep) with a easy to use interface and settings. The results are dispalyed in a descriptive manner and also are being stored for future references.

## Operations
- **Find**: Ideal for finding/locating any word or sentence within a certain directory. This operation can be invoked like so, <br />
`./rex "directory path" --find "something"`
- **Replace**: Ideal for replacing any word with any other word withing a certain directory. This operation can be invoked like so, <br />
`./rex "directory path" --replace "word-to-be-replace" "word-to-be-replaced-with"`

## Operation Scopes
- **all**: This is a default option flag which restricts the scope of the current operation to the whole directory.
- **some**: This option flag should be used when the user wants only certain files to be subjected to the current operation.

**NOTE**: If no scope flag is provided the operation, the default scope will be restricted to 'all' flag.
<br />

**PRO-TIP**: Use the `--lines`(see on how to use it below) flag to display more than one line in your result output in the case of **Find** operation.

## Full Descriptive Menu
```
 Functionalities offered: 
  - Find all the occurrences of a certain word or a sentence in the selected codebase. 
  - Replace all the occurrences of a certain word or a sentence in the selected codebase by a something else. 

 Usage: 
  - For finding all/some occurrences: 
       => rex [directory path('.' or 'full path')] --find [word or sentence] --all(for the whole directory)/--some [filePath1] [filePath2] ... 

  - For replacing all/some occurrences: 
       => rex [directory path('.' or 'full path')] --replace [word or sentence to be replaced] [word or sentence to be replaced with] --all(for the whole directory)/--some [filePath1] [filePath2] ... 


 Flags(actual precedence order): 
   --find      operation specification flag, used to find code snippets in the current working directory. 
   --replace   operation specification flag, used to replace code snippets with the provided ones in the current working directory.
   --list     if multiple words are to be found, present this flag and pass the keywords in a listing fashion.
   --all       flag for considering every file in the current working directory(default).
   --some      flag for considering some selected few files in the current working directory.
   --lines     flag for displaying more than one line in the result output[just provide the integer after it].
   --include-only     flag for only considering certain filetypes(specify file types only without a period).
   --except     flag for excluding certain file types(specify file types only without a period).

 NOTE: Currently only these file types are supported: c, cpp, js, mjs, ts, java, py, txt.

 TIP: 
     * Just append '--display' flag at the end of your command to display the operation details(in the case of replace operation).
     * To perform any operation on a certain file type, consider using '--include-only' flag, just add it as a suffix to the whole command,like so : [whole command] --include-only cpp java py...
     * To exclude any file(s) from the operation being performed,just add the flag '--except' and the file types, like so: [whole command] --except cpp java py... 
     * By default the operation scope would be '--all', unless specified otherwise.
```



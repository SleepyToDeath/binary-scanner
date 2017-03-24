### What is this?
Security guys sometimes need to find out all potential existences
of several specific dangerous instructions in a binary file or a
memory block, probably not aligned to instruction boundary. 
This is a library and a tool that serve this purpose.
It uses Aho-Corasick algorithm and should be very fast.

### How to use?
Directory `ac` is the library and can be embeded into your code.
`string.c` and `scanner.c` are two wrappers of the library. Once
compiled, `string` is the tool that does string matching and `scanner`
is the tool that does binary scanning.

Tools:  
`$string`  
This will read pattern string and main string from pattern.txt and
main.txt respectively and print all matched pattern string index(by
the order in pattern.txt, starting from 0) and offset in main string
(by byte).

`$scanner FILE_NAME GRANULARITY`  
This will read pattern instruction(in 0,1 form) from pattern.txt
and scan for it in the specified file by specified granularity(in bits). 
(for example, if you believe an instruction can't start at the middle of
a byte, you set the GRANULARITY to 8)(please don't set this value too high,
the memory consumption is at the level of 2^G)
All matched instruction index and offset(by bit, no matter what granularity)
in the file will be printed. This tool is unable to recognize the file format, 
so if you want results in a specific section, filter the result yourself.

Library:  
To use the library, build a `struct task` and pass it to the `do_task`
and everything will finish automatically.
This is an online library so you can process the main binary stream
piece by piece, which saves a lot of memory space.
For more information, see comments in ac/ac.h.


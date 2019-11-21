# ICT1002CProject
SIT ICT1002 C Chatbot Project for Trimester 1.

## What This Is
A simple chatbot written in C, it has a few commands - load,quit,save,who,what,where. There is documentation inside to explain how these <br>
are implemented and how they behave.

## How to Run This
First compile it:
```
gcc chatbot.c HashMap.c knowledge.c main.c -I./ -o chatbot
```
Then run it.
```
./chatbot     (on *nix)
./chatbot.exe (on Windows)
```

## Technologies/ Concepts Used
- Base knowledge stored in a simple struct.
- Additional knowledge stored in a HashMap that we implemented. See HashMap.h for HashMap macro settings.
- HashMap uses djb2 hashing function and is an array of doubly linked lists in case hash collision occurs.
- HashMap can grow dynamically via the heap.
- Storage and searching of responses are care insensitive.

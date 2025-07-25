# binary-translation-detector (btd)
This is a simple C program that will determine whether your environment is running under a binary translation layer. This is a type of software emulation that "translates" a different instruction set architecture for a different one in real time. For example, Microsoft XTA translates x86 code into ARM code. 


# Why?
This is mainly meant as just a fun and experimental program that I've developed while working on VMAware, a VM detection library. VMAware originally added binary translators as a detectable VM brand, but my co-developer (@NotRequiem) convinced me to ditch that idea as it was out of the scope of the project. 

So instead of just deleting them, I'm adding them in this separate repository as I figured it was interesting enough to have it published as a gimmicky and stupidly simple C side project for the sake of demonstrating that this is possible as a proof of concept. 

Does this have any practical implications? Absolutely fucking not. But hey, it's kinda cool (maybe, idk). Anyways, this is just a publish-and-dump kind of thing, so it won't be improved or worked on in the future.


# Which binary translators can it detect?
For now, the program can detect 4 brands:
 - Apple Rosetta
 - Microsoft x86-to-ARM (also known as Microsoft XTA)
 - Microsoft Prism
 - PowerVM Lx86

# Eternal-Hate
Optimized skeleton script of the Eternal Hate Botnet.

Ok, so...

I made this code with the sole purpose of creating a weaponized network of worms, with 3 main objectives:

1. to be able to replicate and propagate itself
2. once the target is infected, to be able to completetly destroy (shred) the OS, wether is Linux or Windows
3. secure that rules 1 and 2 are always executed.


# Notes: the source code and the compiled version assumes an ssh authentication mechanism IS NOT INVOLVED, so the infected servers will look for SSH connection with no authentication rules set, to move laterally. So, you may want to modify this part.

# To run the binary:
$ ./worm_revisited

# To compile the source code:
$ sudo gcc -o output_file source_code.c -lssl -lcrypto -lcurl

RUN ON AN ISOLATED ENVIRONMENT!! Use a disposable docker cluster.

IM NOT RESPONSIBLE FOR ANY MISUSE OF THIS CODE IN ANY SHAPE OR FORM. THIS CODE WAS RELEASED FOR EDUCATIONAL PURPOSES.

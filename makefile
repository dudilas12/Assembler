assembler:input.o label.o mainfile.o defenitions.h linkedlist.o faze2.o structs.h dynamicArray.o bitfield.o combineFiles.o
	gcc -g -ansi -Wall -pedantic input.o label.o mainfile.o faze2.o linkedlist.o dynamicArray.o bitfield.o combineFiles.o -o assembler
dynamicArray.o:dynamicArray.c defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic dynamicArray.c -o dynamicArray.o
label.o:label.c defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic label.c -o label.o
input.o:input.c label.o defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic input.c -o input.o
mainfile.o:mainfile.c defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic mainfile.c -o mainfile.o
linkedlist.o:linkedlist.c defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic linkedlist.c -o linkedlist.o
faze2.o:faze2.c defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic faze2.c -o faze2.o
bitfield.o:bitfield.c defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic bitfield.c -o bitfield.o
combineFiles.o:combineFiles.c defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic combineFiles.c -o combineFiles.o


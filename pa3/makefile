all:y86emul y86dis

y86emul:
	gcc -Wall -lm y86emul.c -o y86emul
	./y86emul prog1.y86.txt
	./y86emul prog2.y86.txt
y86dis:
	gcc -Wall -lm y86dis.c -o y86dis
	./y86dis prog1.y86.txt
	./y86dis prog2.y86.txt
clean:
	rm y86emul y86dis

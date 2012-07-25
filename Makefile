all:
	gcc -Wall obfuscate.c elf_parse.c nightflower.c -o nightflower

clean:
	rm -f nightflower

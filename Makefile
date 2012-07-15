all:
	gcc -Wall elf_parse.c nightflower.c -o nightflower

clean:
	rm -f nightflower

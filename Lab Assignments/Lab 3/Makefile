dev: 
	@echo "Atharva Swami\n190010008@iitdh.ac.in"

compiler: main.c microParser.y microLexer.l
	@bison -d -o microParser.c microParser.y
	@flex microLexer.l
	@gcc lex.yy.c microParser.c main.c -o a.out

clean:
	@rm -f lex.yy.c microParser.c microParser.h
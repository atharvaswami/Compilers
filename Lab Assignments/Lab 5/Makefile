dev:
	@echo "Atharva Swami\n190010008@iitdh.ac.in"
	
lex.yy.c: scanner.l
	@flex $^

parser.c: parser.y
	@bison -d $^ -o parser.c

compiler: lex.yy.c parser.c main.cpp headers/ast.cpp headers/*.hpp
	@g++ -std=c++11 -w -g lex.yy.c parser.c main.cpp headers/ast.cpp -o a.out
	@chmod +x runme

clean:
	@rm -f lex.yy.c parser.c parser.h
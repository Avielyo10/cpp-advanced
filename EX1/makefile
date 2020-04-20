all: main.cpp Editor.h Editor.cpp Document.h Document.cpp
	@g++ -std=c++11 main.cpp Editor.cpp Document.cpp
	@./a.out < input.txt > a.txt
	@diff out.txt a.txt
	@rm -rf a.out a.txt

FILES = ./build/main.o ./build/tokenizer.o ./build/parser.o

all: $(FILES)
	g++ $(FILES) -o script-language

./build/main.o: ./src/main.c
	g++ -c ./src/main.c -o ./build/main.o

./build/tokenizer.o: ./src/tokenizer/tokenizer.c
	g++ -c ./src/tokenizer/tokenizer.c -o ./build/tokenizer.o

./build/parser.o: ./src/parser/parser.c
	g++ -c ./src/parser/parser.c -o ./build/parser.o
	
clean:
	rm -rf ./build/*.o script-language
	mkdir -p ./build
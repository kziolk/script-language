all: ./build/main.o ./build/tokenizer.o
	g++ ./build/main.o ./build/tokenizer.o -o script-language

./build/main.o: ./src/main.c
	g++ -c ./src/main.c -o ./build/main.o

./build/tokenizer.o: ./src/tokenizer/tokenizer.c
	g++ -c ./src/tokenizer/tokenizer.c -o ./build/tokenizer.o
	
clean:
	rm ./build/*.o script-language
ALL: main
main:
	g++ -o main main.cpp trie.cpp -std=c++17 -lstdc++fs -pthread

clean:
	rm main

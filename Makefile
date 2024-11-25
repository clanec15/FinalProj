main: main.cpp Headers/FileOperations/* Headers/TUIController.hpp Headers/MatrixOps.hpp
	g++ -o ./FinalProj Headers/FileOperations/* Headers/TUIController.hpp Headers/MatrixOps.hpp main.cpp -O3 -std=c++17 -Wpedantic -static
	mkdir input
	mkdir output
clean:
	rm -f  ./FinalProj
	rm -rf input
	rm -rf output
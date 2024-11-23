main: main.cpp Headers/FileOperations/* Headers/TUIController.hpp Headers/MatrixOps.hpp
	g++ -o ./FinalProj Headers/FileOperations/* Headers/TUIController.hpp Headers/MatrixOps.hpp main.cpp -O3 
	mkdir input
	mkdir output
clean:
	rm -f  ./FinalProj
	rm -rf input
	rm -rf output
	rm -f main

main: main.cpp Headers/FileReader.hpp Headers/FileSalvor.hpp Headers/RowParser.hpp Headers/TUIController.hpp
	g++ -o ./FinalProj Headers/FileReader.hpp Headers/FileSalvor.hpp Headers/RowParser.hpp Headers/TUIController.hpp main.cpp
	mkdir input
	mkdir output
	cp T1.txt input/
	cp T2-faltante.txt input/

clean:
	rm -f  ./FinalProj
	rm -rf input
	rm -rf output

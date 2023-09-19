CMAKE_DEFINES = 

BUILD_DIR = build
INSTALL_DIR = ~/
DOC_DIR = doc

EXE_NAME = SmartCalc_v2
FILES_TO_COVER = Calculator.cc Tokenizer.cc Translator.cc

all: build

configure:
	cmake -B $(BUILD_DIR) . $(CMAKE_DEFINES)

install: build
	cp -R $(BUILD_DIR)/$(EXE_NAME).app $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/$(EXE_NAME).app

html:
	makeinfo --no-split --html $(DOC_DIR)/doc.texi -o $(DOC_DIR)/doc.html

dvi: html
	open $(DOC_DIR)/doc.html
	
dist: clean
	tar -cvz -f $(EXE_NAME).tar.gz $(shell find . -type f -name "*")

build: configure
	cd build && cmake --build . --target SmartCalc_v2

tests: configure
	cd build && cmake --build . --target tokenizer_test
	cd build && cmake --build . --target model_integration
	./$(BUILD_DIR)/tokenizer_test
	./$(BUILD_DIR)/model_integration

gcov_report: add_lcov_definition configure tests
	mkdir -p gcov
	cp $(shell find build -type f -name "*.gcda") gcov
	cp $(shell find build -type f -name "*.gcno") gcov
	geninfo $(addprefix gcov/, $(addsuffix .gcda, $(FILES_TO_COVER))) -o gcov/data.info --gcov-tool /usr/bin/gcov --ignore-errors inconsistent --exclude v1 --exclude containers
	genhtml gcov/data.info -o report --ignore-errors inconsistent
	open report/index.html

clean:
	rm -rf *.tar.gz
	rm -rf $(DOC_DIR)/*.html
	rm -rf report
	rm -rf $(BUILD_DIR)
	rm -rf gcov

rebuild: clean tests

add_lcov_definition:
		$(eval CMAKE_DEFINES += -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage -fno-elide-constructors")
		$(eval CMAKE_DEFINES += -DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs")

# --no-external

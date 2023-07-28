BUILD_DIR = build
INSTALL_DIR = ~/
DOC_DIR = doc

EXE_NAME = SmartCalc_v2

all: build

configure:
	cmake -B $(BUILD_DIR) .

install: build
	cp $(BUILD_DIR)/$(EXE_NAME).app $(INSTALL_DIR)

uninstall: clean
	rm -rf $(INSTALL_DIR)/$(EXE_NAME).app

html:
	makeinfo --no-split --html $(DOC_DIR)/doc.texi
	
dist:
	tar -cvz -f $(EXE_NAME).tar.gz $(shell find . -type f -name "*")

build: configure
	cd build && cmake --build . --target SmartCalc_v2

test: configure
		cd build && cmake --build . --target tokenizer_test
		cd build && cmake --build . --target model_integration

clean:
	rm -rf *.info
	rm -rf *.exe
	rm -rf *.tar.gz
	rm -rf *.gcda *.gcno
	rm -rf *.dSYM
	rm -rf *.html
	rm -rf report
	rm -rf build

rebuild: clean tests

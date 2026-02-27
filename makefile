# Define CFLAGS so the implicit rule for .o files uses -g
CFLAGS = -g

OBJ = $(patsubst %.c,%.o,$(wildcard *.c))

inverted_search.exe : $(OBJ)
	gcc -g -o $@ $^

# ── Automated Test Target ──
.PHONY : test
test: inverted_search.exe
	@echo "\n[1/3] Generating test text files..."
	@echo "hello world c programming" > test1.txt
	@echo "embedded systems programming world" > test2.txt
	@echo "c language embedded testing" > test3.txt
	@echo "new data structure for the search engine" > test_update.txt
	
	@echo "[2/3] Generating automated menu inputs..."
	@echo "1" > test_input.txt
	@echo "2" >> test_input.txt
	@echo "4" >> test_input.txt
	@echo "1" >> test_input.txt
	@echo "test_update.txt" >> test_input.txt
	@echo "2" >> test_input.txt
	@echo "3" >> test_input.txt
	@echo "embedded" >> test_input.txt
	@echo "6" >> test_input.txt
	
	@echo "[3/3] Running inverted_search.exe with automated inputs...\n"
	./inverted_search.exe test1.txt test2.txt test3.txt < test_input.txt

.PHONY : clean
clean :
	rm -f inverted_search.exe *.o test*.txt database.txt
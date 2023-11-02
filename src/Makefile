.PHONY: test

CFLAGS=-I ./ -Wall -g #-DSENTINEL

test: test-rbtree
	./test-rbtree
	valgrind ./test-rbtree

test-rbtree: test-rbtree.o rbtree.o

rbtree.o:
	$(MAKE) -C rbtree.o

clean:
	rm -f test-rbtree *.o

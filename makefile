all: lisp test

test:
	(cd test; make test)

lisp:
	(cd src; make lisp)

clean:
	(cd src; make clean)
	(cd test; make clean)

.PHONY: all lisp test clean
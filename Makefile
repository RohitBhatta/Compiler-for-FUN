TESTS=$(sort $(wildcard *.test))
PROGS=$(subst .test,,$(TESTS))
OUTS=$(patsubst %.test,%.out,$(TESTS))
DIFFS=$(patsubst %.test,%.diff,$(TESTS))
RESULTS=$(patsubst %.test,%.result,$(TESTS))

.SECONDARY:

.PROCIOUS : %.o %.S %.out

CFLAGS=-g -std=c99 -O0 -Werror -Wall

p3 : p3.o Makefile
	gcc $(CFLAGS) -o p3 p3.o

%.o : %.c Makefile
	gcc $(CFLAGS) -MD -c $*.c

%.o : %.S Makefile
	gcc -MD -c $*.S

%.S : %.test p3
	@echo "========== $* =========="
	./p3 < $*.test > $*.S

progs : $(PROGS)

$(PROGS) : % : %.o
	gcc -o $@ $*.o

outs : $(OUTS)

$(OUTS) : %.out : %
	./$* > $*.out

diffs : $(DIFFS)

$(DIFFS) : %.diff : Makefile %.out %.ok
	@(((diff -b $*.ok $*.out > /dev/null 2>&1) && (echo "===> $* ... pass")) || (echo "===> $* ... fail" ; echo "----- expected ------"; cat $*.ok ; echo "----- found -----"; cat $*.out)) > $*.diff 2>&1

$(RESULTS) : %.result : Makefile %.diff
	@cat $*.diff

test : Makefile $(DIFFS)
	@cat $(DIFFS)

clean :
	rm -f $(PROGS)
	rm -f *.S
	rm -f *.out
	rm -f *.d
	rm -f *.o
	rm -f p3
	rm -f *.diff

-include *.d

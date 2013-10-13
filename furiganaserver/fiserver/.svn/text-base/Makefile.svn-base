SERVER_PROG=fiserver
UNITTEST_PROG=unit_tests
CFLAGS=	-W -Wall -I../mongoose -I../json-c-0.9 -pthread -g

all:
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl -lmecab -ljson" ; \
	  $(CC) $(CFLAGS) ficommon.c server.c ../mongoose/mongoose.c  $$LIBS $(ADD) -o $(SERVER_PROG) 
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl -lmecab -ljson" ; \
	  $(CC) $(CFLAGS) ../mongoose/mongoose.c ficommon.c unit_tests.c $$LIBS $(ADD) -o $(UNITTEST_PROG) 
	./$(UNITTEST_PROG)

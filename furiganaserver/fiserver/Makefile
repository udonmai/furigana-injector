CC=gcc
SERVER_PROG=fiserver
UNITTEST_PROG=unit_tests
CFLAGS= -W -Wall -pthread -g
INCS= -I../json-c-0.9 -I ../mongoose
LIBS= -ldl -lmecab -ljson-c -L/usr/local/Cellar/json-c/0.11/lib -L /usr/local/Cellar/mecab/0.996/lib

all:
	#OS=`uname`; \
	#  test "$$OS" = Dawin && LIBS="-ldl -L/usr/local/Cellar/json-c/0.11/lib -L/usr/local/Cellar/mecab/0.996/lib -lmecab -ljson" ; 
	  $(CC) $(CFLAGS) $(INCS) ficommon.c server.c ../mongoose/mongoose.c $(LIBS) $(ADD) -o $(SERVER_PROG) 
	#OS=`uname`; \
	#  test "$$OS" = Dawin && LIBS="-ldl -lmecab -ljson" ; 
		$(CC) $(CFLAGS) $(INCS) ../mongoose/mongoose.c ficommon.c unit_tests.c $(LIBS) $(ADD) -o $(UNITTEST_PROG) 
		./$(UNITTEST_PROG)

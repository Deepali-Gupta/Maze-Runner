    #~ make should compile the source code
    #~ make DEBUG should execute the application with DEBUG_MODE. Explain the different debug symbols in the README file
    #~ make clean should clean all the temporary and the executables
    #~ make server should spawn the server
    #~ make client server=x should spawn a client which connects to server x. x is a combination of server's ip address, and port number
    #~ Any extra feature which needs to be invoked using a separate make target should be explained in the README file
    
    
#------------------------------------------------------------------------------

CC=g++ -w
SRC=./src
LIBS= -lGL -lGLU -lpthread -lglut -lglui -lssl -L/usr/lib -lcrypto

#------------------------------------------------------------------------------
# make                 compile source code
# make clean           to remove all .obj and .exe files 
# make DEBUG           for debug-mode
# make server          spawn the server
#------------------------------------------------------------------------------

debug: CC+= -DDEBUG

all: $(SRC)/run

$(SRC)/run: $(SRC)/main.cpp $(SRC)/display.cpp $(SRC)/buttons.cpp $(SRC)/tcp_server.cpp $(SRC)/tcp_client.cpp
	$(CC) $(SRC)/main.cpp $(SRC)/display.cpp $(SRC)/buttons.cpp $(SRC)/tcp_server.cpp $(SRC)/tcp_client.cpp -o $(SRC)/run $(LIBS)

clean:
	rm $(SRC)/run $(SRC)/game $(SRC)/maze $(SRC)/file.txt $(SRC)/players.txt $(SRC)/hint $(SRC)/ai2.txt $(SRC)/ai3.txt $(SRC)/ai4.txt $(SRC)/ai5.txt $(SRC)/ai6.txt $(SRC)/ai2 $(SRC)/ai3 $(SRC)/ai4 $(SRC)/ai5 $(SRC)/ai6 $(SRC)/ai_2 $(SRC)/ai_3 $(SRC)/ai_4 $(SRC)/ai_5 $(SRC)/ai_6 

server: $(SRC)/run
	cd src && ./run

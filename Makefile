TARGET_C = Client
TARGET_S = Server
CC = g++

PREF_CL = ./src_client/
PREF_SH = ./src_shared/
PREF_SR = ./src_server/

SRC_SH = $(wildcard $(PREF_SH)*.cpp)
SRC_SR = $(wildcard $(PREF_SR)*.cpp)
SRC_CL = $(wildcard $(PREF_CL)*.cpp)
OBJ_CL = $(patsubst $(PREF_CL)%.cpp, $(PREF_CL)%.o, $(SRC_CL))
OBJ_SH = $(patsubst $(PREF_SH)%.cpp, $(PREF_SH)%.o, $(SRC_SH))
OBJ_SR = $(patsubst $(PREF_SR)%.cpp, $(PREF_SR)%.o, $(SRC_SR))

LIB = -lmysqlclient

$(TARGET_C): $(OBJ_CL) $(OBJ_SH)
	$(CC) -o $(TARGET_C) $(OBJ_CL) $(OBJ_SH)

$(TARGET_S): $(OBJ_SR) $(OBJ_SH)
	$(CC) -o $(TARGET_S) $(OBJ_SR) $(OBJ_SH) $(LIB)

$(PREF_CL)%.o: $(PREF_CL)%.cpp 
	$(CC) -c $< -o $@

$(PREF_SH)%.o: $(PREF_SH)%.cpp 
	$(CC) -c $< -o $@

$(PREF_SR)%.o: $(PREF_SR)%.cpp 
	$(CC) -c $< -o $@

clear:
	rm -f $(PREF_CL)*.o $(PREF_SH)*.o $(PREF_SR)*.o
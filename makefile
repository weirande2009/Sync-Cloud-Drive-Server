CC = g++

DIR_INC = ./include
DIR_SRC = ./source
DIR_OBJ = ./obj
DIR_BIN = ./bin

mongodb_header = /usr/local/include/mongocxx/v_noabi
bson_header = /usr/local/include/bsoncxx/v_noabi
LIBS = -lprotobuf -lpthread -lbsoncxx -lmongocxx

DAO = dao
PROTOBUF = protobuf
SERVER = server
SERVICE = service
SIMPLE_SERVER = simple_server
UTILITIES = utilities

DAO_DIR_SRC = $(DIR_SRC)/$(DAO)
PROTOBUF_DIR_SRC = $(DIR_SRC)/$(PROTOBUF)
SERVER_DIR_SRC = $(DIR_SRC)/$(SERVER)
SERVICE_DIR_SRC = $(DIR_SRC)/$(SERVICE)
SIMPLE_SERVER_DIR_SRC = $(DIR_SRC)/$(SIMPLE_SERVER)
UTILITIES_DIR_SRC = $(DIR_SRC)/$(UTILITIES)

DAO_SRC = $(wildcard $(DAO_DIR_SRC)/*.cc)
PROTOBUF_SRC = $(wildcard $(PROTOBUF_DIR_SRC)/*.cc)
SERVER_SRC = $(wildcard $(SERVER_DIR_SRC)/*.cc)
SERVICE_SRC = $(wildcard $(SERVICE_DIR_SRC)/*.cc)
SIMPLE_SERVER_SRC = $(wildcard $(SIMPLE_SERVER_DIR_SRC)/*.cc)
UTILITIES_SRC = $(wildcard $(UTILITIES_DIR_SRC)/*.cc)
MAIN_SRC = main.cc

# DAO_OBJ = $(patsubst %.cc,$(DIR_OBJ)/%.o,$(notdir $(DAO_SRC))) 
# PROTOBUF_OBJ = $(patsubst %.cc,$(DIR_OBJ)/%.o,$(notdir $(PROTOBUF_SRC))) 
# SERVER_OBJ = $(patsubst %.cc,$(DIR_OBJ)/%.o,$(notdir $(SERVER_SRC))) 
# SERVICE_OBJ = $(patsubst %.cc,$(DIR_OBJ)/%.o,$(notdir $(SERVICE_SRC))) 
# SIMPLE_SERVER_OBJ = $(patsubst %.cc,$(DIR_OBJ)/%.o,$(notdir $(SIMPLE_SERVER_SRC))) 
# UTILITIES_OBJ = $(patsubst %.cc,$(DIR_OBJ)/%.o,$(notdir $(UTILITIES_SRC)))
# MAIN_OBJ = $(DIR_OBJ)/main.o
DAO_OBJ = $(patsubst %.cc,%.o,$(notdir $(DAO_SRC))) 
PROTOBUF_OBJ = $(patsubst %.cc,%.o,$(notdir $(PROTOBUF_SRC))) 
SERVER_OBJ = $(patsubst %.cc,%.o,$(notdir $(SERVER_SRC))) 
SERVICE_OBJ = $(patsubst %.cc,%.o,$(notdir $(SERVICE_SRC))) 
SIMPLE_SERVER_OBJ = $(patsubst %.cc,%.o,$(notdir $(SIMPLE_SERVER_SRC))) 
UTILITIES_OBJ = $(patsubst %.cc,%.o,$(notdir $(UTILITIES_SRC)))
MAIN_OBJ = main.o

OBJ = $(DAO_OBJ) $(PROTOBUF_OBJ) $(SERVER_OBJ) $(SERVICE_OBJ) $(SIMPLE_SERVER_OBJ) $(UTILITIES_OBJ) $(MAIN_OBJ)

TARGET = main

CFLAGS = -g -Wall -fdiagnostics-color=always -I${DIR_INC} -I${mongodb_header} -I${bson_header} -L/usr/local/lib/ $(LIBS) -std=c++17

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $^ $(CFLAGS) -o $(TARGET) 

$(DAO_OBJ):%.o:$(DAO_DIR_SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

$(PROTOBUF_OBJ):%.o:$(PROTOBUF_DIR_SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_OBJ):%.o:$(SERVER_DIR_SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVICE_OBJ):%.o:$(SERVICE_DIR_SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

$(SIMPLE_SERVER_OBJ):%.o:$(SIMPLE_SERVER_DIR_SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

$(UTILITIES_OBJ):%.o:$(UTILITIES_DIR_SRC)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ):%.o:%.cc
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean

clean:
	rm -rf *.o \;



TARGET    =     avm2

CC        =     gcc
LD        =     gcc

INCLUDES =     include
COBJ     =     .obj
CSRC     =     src

HEADERS  =     $(INCLUDES)/*.h
CSOURCE  =     $(CSRC)/*.c
COBJECT  =     $(COBJ)/memory.o \
               $(COBJ)/instructions.o \
               $(COBJ)/execute.o \
               $(COBJ)/main.o


CFLAGS    =    -c -I$(INCLUDES) -fPIC -O3

$(TARGET): $(COBJECT)
	$(LD) $^ $(LDFLAGS) -o $@ 

$(COBJ)/%.o: $(CSRC)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

install: $(TARGET)
	cp $(TARGET) $(HOME)/.amicayse/bin

update: pull install

pull:
	git pull origin master
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

install:
	make
	cp $(TARGET) $(HOME)/.amicayse/bin

update:
	git pull origin master
	make install
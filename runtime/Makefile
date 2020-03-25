TARGET    =     avm2

CC        =     gcc
LD        =     gcc

INCLUDES  =     include
COBJ     =     .obj
CSRC     =     src

HEADERS   =     $(INCLUDES)/*.h
CSOURCE  =     $(CSRC)/*.c
COBJECT  =     $(COBJ)/.o      # add filenames here

CFLAGS    =    -c -I$(INCLUDES) -fPIC -O3
# LDFLAGS    =    -shared

$(TARGET): $(COBJECT)
	$(LD) $^ $(LDFLAGS) -o $@ 

$(COBJ)/%.o: $(CSRC)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@


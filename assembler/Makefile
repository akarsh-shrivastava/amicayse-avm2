TARGET    =     las2

FX        =     flex++
CX        =     g++
LD        =     g++

INCLUDES  =     include
CXOBJ     =     .obj
CXSRC     =     src/cpp

FXSRC     =     src/flex
FXTAR     =     src/cpp

FXSOURCE  =     $(FXSRC)/*.l
FXTARGET  =     $(FXTAR)/lexer.cpp

HEADERS   =     $(INCLUDES)/*.h \
                ../runtime/include/*.h
CXSOURCE  =     $(CXSRC)/*.cpp
CXOBJECT  =     $(CXOBJ)/lexer.o \
                $(CXOBJ)/parser.o \
                $(CXOBJ)/transformer.o \
                $(CXOBJ)/generator.o \
                $(CXOBJ)/main.o

CXFLAGS    =    -c -I$(INCLUDES) -fPIC -O3 --std=c++17 -fpermissive
LDFLAGS    =    -lparsegen

$(TARGET): $(CXOBJECT)
	$(LD) $^ $(LDFLAGS) -o $@ 

$(CXOBJ)/%.o: $(CXSRC)/%.cpp $(HEADERS)
	$(CX) $(CXFLAGS) $< -o $@

$(FXTAR)/%.cpp: $(FXSRC)/%.l $(HEADERS)
	$(FX) -o$@ $<

install: $(TARGET)
	cp $(TARGET) $(HOME)/.amicayse/bin

update: pull install

pull:
	git pull origin master
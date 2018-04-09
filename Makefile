# Makefile for DES

CC = gcc -c
LD = gcc
RM = rm -rf
ECHO = echo
MKDIR = mkdir -p
CP = cp

CFLAGS = -Wall -Werror
LDFLAGS = -Wall -Werror
LDLIBS =

OPT_CFLAGS = -O4
DBG_CFLAGS = -g

CFLAGS_tablegen.c = -Wno-unused-result
CFLAGS_sboxgen.c = -Wno-unused-result

TARGET = DES
TESTKEY = abcd1234cdef5678

TOPDIR = $(shell pwd)
SRC = $(TOPDIR)/src
BUILDDIR = $(TOPDIR)/build
OBJDIR = $(BUILDDIR)/objs
GENSRC = $(BUILDDIR)/gensrc
TOOLSDIR = $(BUILDDIR)/tools

TABLEGEN = $(TOOLSDIR)/tablegen
SBOXGEN = $(TOOLSDIR)/sboxgen
TOOLS = $(SBOXGEN) $(TABLEGEN)

OBJFILES = $(sort \
               $(patsubst $(SRC)/data/%.data,$(OBJDIR)/%.o,$(wildcard $(SRC)/data/*.data)) \
               $(patsubst $(SRC)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRC)/*.c)) \
               )

default: $(TARGET)
$(TARGET): $(addprefix $(BUILDDIR)/,$(TARGET))
	@$(CP) $^ $(TOPDIR)

OBJ_CMD = $(CC) -I$(SRC) $(CFLAGS) $(OPT_CFLAGS) $(DBG_CFLAGS) $(CFLAGS_$(notdir $<)) $< -o $@
$(OBJFILES):
	@$(MKDIR) $(dir $@)
	@$(ECHO) Compiling $(notdir $<)
	@$(ECHO) '$(OBJ_CMD)' > $@.cmdline
	@$(OBJ_CMD) 2> $@.log

TARGET_CMD = $(LD) -o $@ $(LDFLAGS) $^ $(LDLIBS)
$(addprefix $(BUILDDIR)/,$(TARGET)):
	@$(MKDIR) $(dir $@)
	@$(ECHO) Linking $(notdir $@)
	@$(ECHO) '$(TARGET_CMD)' > $@.cmdline
	@$(TARGET_CMD) 2> $@.log

clean:
	@$(RM) $(BUILDDIR) $(TARGET)

sweep: clean
	@$(RM) $(SRC)/*~ *~

test: $(TARGET)
	./$(TARGET) $(TESTKEY) -i Makefile | ./$(TARGET) $(TESTKEY) -d

TOOLS_CMD = $(LD) $(TOOLS_LDFLAGS) $^ $(TOOLS_LDLIBS) -o $@
$(TOOLS):
	@$(MKDIR) $(dir $@)
	@$(ECHO) Building tool $(notdir $@)
	@$(ECHO) '$(TOOLS_CMD)' > $@.cmdline
	@$(TOOLS_CMD) 2> $@.log

define gensrc
  $1_CMD = $2 $3 $4 $1 < $$< > $$@
  $(GENSRC)/$1.c:
	@$(MKDIR) $$(dir $$@)
	@$(ECHO) Generating $$(notdir $$@)
	@$(ECHO) '$$($1_CMD)' > $$@.cmdline
	@$$($1_CMD)
endef

$(eval $(call gensrc,pc1,$(TABLEGEN),64,56))
$(eval $(call gensrc,pc2,$(TABLEGEN),56,48))
$(eval $(call gensrc,ip,$(TABLEGEN),64,64))
$(eval $(call gensrc,pi,$(TABLEGEN),64,64))
$(eval $(call gensrc,e,$(TABLEGEN),32,48))
$(eval $(call gensrc,p,$(TABLEGEN),32,32))
$(eval $(call gensrc,sboxes, $(SBOXGEN)))

## Dependencies
$(addprefix $(BUILDDIR)/,$(TARGET)): $(OBJFILES)
$(TABLEGEN): $(SRC)/tools/tablegen.c
$(SBOXGEN): $(SRC)/tools/sboxgen.c

$(GENSRC)/pc1.c: $(SRC)/data/pc1.data $(TABLEGEN) 
$(GENSRC)/pc2.c: $(SRC)/data/pc2.data $(TABLEGEN) 
$(GENSRC)/ip.c: $(SRC)/data/ip.data $(TABLEGEN) 
$(GENSRC)/pi.c: $(SRC)/data/pi.data $(TABLEGEN) 
$(GENSRC)/e.c: $(SRC)/data/e.data $(TABLEGEN) 
$(GENSRC)/p.c: $(SRC)/data/p.data $(TABLEGEN) 
$(GENSRC)/sboxes.c: $(SRC)/data/sboxes.data $(SBOXGEN)

$(OBJDIR)/pc1.o: $(GENSRC)/pc1.c $(SRC)/types.h
$(OBJDIR)/pc2.o: $(GENSRC)/pc2.c $(SRC)/types.h
$(OBJDIR)/ip.o: $(GENSRC)/ip.c $(SRC)/types.h
$(OBJDIR)/pi.o: $(GENSRC)/pi.c $(SRC)/types.h
$(OBJDIR)/e.o: $(GENSRC)/e.c $(SRC)/types.h
$(OBJDIR)/p.o: $(GENSRC)/p.c $(SRC)/types.h
$(OBJDIR)/sboxes.o: $(GENSRC)/sboxes.c $(SRC)/types.h

$(OBJDIR)/F.o: $(SRC)/F.c
$(OBJDIR)/options.o: $(SRC)/options.c
$(OBJDIR)/main.o: $(SRC)/main.c
$(OBJDIR)/keygen.o: $(SRC)/keygen.c
$(OBJDIR)/feistel.o: $(SRC)/feistel.c

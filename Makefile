# Makefile for DES

TARGET = DES
EXTRA_OBJS = $(patsubst $(SRC)/data/%.data,$(OBJDIR)/%.o,$(wildcard $(SRC)/data/*.data))

include make/build.gmk

## Code generator tools
TABLEGEN = $(TOOLSDIR)/tablegen
SBOXGEN = $(TOOLSDIR)/sboxgen
TOOLS = $(SBOXGEN) $(TABLEGEN)

## Build codegen tools
TOOLS_CMD = $(LD) $(LDFLAGS_$@) $^ $(LDLIBS_$@) -o $@
$(TOOLS):
	@$(MKDIR) $(dir $@)
	@$(ECHO) Building tool $(notdir $@)
	@$(ECHO) '$(TOOLS_CMD)' > $@.cmdline
	@$(TOOLS_CMD) 2> $@.log
	@[ -s $@.log ] || $(RM) $@.log

## Generate C source from data files
define gensrc
  $1_CMD = $2 $3 $4 $1 < $$< > $$@
  $(GENSRC)/$1.c:
	@$(MKDIR) $$(dir $$@)
	@$(ECHO) Generating $$(notdir $$@)
	@$(ECHO) '$$($1_CMD)' > $$@.cmdline
	@$$($1_CMD) 2> $$@.log
	@[ -s $$@.log ] || $(RM) $$@.log
endef

$(eval $(call gensrc,pc1,$(TABLEGEN),64,56))
$(eval $(call gensrc,pc2,$(TABLEGEN),56,48))
$(eval $(call gensrc,ip,$(TABLEGEN),64,64))
$(eval $(call gensrc,pi,$(TABLEGEN),64,64))
$(eval $(call gensrc,e,$(TABLEGEN),32,48))
$(eval $(call gensrc,p,$(TABLEGEN),32,32))
$(eval $(call gensrc,sboxes,$(SBOXGEN)))

DATAFILE = $(BUILDDIR)/data.txt
$(DATAFILE): $(wildcard $(SRC)/data/*.data)
	@$(MKDIR) $(dir $@)
	@$(RUBY) $(SRC)/tools/datafile.rb $^ > $(DATAFILE)

paper: $(DATAFILE)

## Test
TESTKEY = 1234567890abcdef
TESTFILE = $(BUILDDIR)/test/testfile
$(TESTFILE): $(SRCFILES)
	@$(MKDIR) $(dir $@)
	@$(CAT) $^ > $@

test: $(TARGET) $(TESTFILE)
	@$(CP) $(TESTFILE) $(TESTFILE).orig
	@./$(TARGET) $(TESTKEY) -i $(TESTFILE).orig | ./$(TARGET) $(TESTKEY) -d > $(TESTFILE)
	@$(DIFF) -B $(TESTFILE).orig $(TESTFILE) && $(ECHO) SUCCESS

### Dependencies

## Link dependencies are all object files for the executable
$(addprefix $(BUILDDIR)/,$(TARGET)): $(OBJFILES)

## The first dependency for object files must be their source file
$(OBJDIR)/feistel.o: $(SRC)/feistel.c $(SRC)/types.h $(SRC)/tables.h
$(OBJDIR)/keygen.o: $(SRC)/keygen.c $(SRC)/types.h $(SRC)/tables.h
$(OBJDIR)/main.o: $(SRC)/main.c $(SRC)/types.h $(SRC)/options.h $(SRC)/feistel.h $(SRC)/tables.h
$(OBJDIR)/options.o: $(SRC)/options.c $(SRC)/types.h

$(OBJDIR)/pc1.o: $(GENSRC)/pc1.c $(SRC)/types.h
$(OBJDIR)/pc2.o: $(GENSRC)/pc2.c $(SRC)/types.h
$(OBJDIR)/ip.o: $(GENSRC)/ip.c $(SRC)/types.h
$(OBJDIR)/pi.o: $(GENSRC)/pi.c $(SRC)/types.h
$(OBJDIR)/e.o: $(GENSRC)/e.c $(SRC)/types.h
$(OBJDIR)/p.o: $(GENSRC)/p.c $(SRC)/types.h
$(OBJDIR)/sboxes.o: $(GENSRC)/sboxes.c $(SRC)/types.h

## Codegen tools
$(TABLEGEN): $(SRC)/tools/tablegen.c
$(SBOXGEN): $(SRC)/tools/sboxgen.c

## Generated permutation tables and sboxes
$(GENSRC)/pc1.c: $(SRC)/data/pc1.data $(TABLEGEN)
$(GENSRC)/pc2.c: $(SRC)/data/pc2.data $(TABLEGEN)
$(GENSRC)/ip.c: $(SRC)/data/ip.data $(TABLEGEN)
$(GENSRC)/pi.c: $(SRC)/data/pi.data $(TABLEGEN)
$(GENSRC)/e.c: $(SRC)/data/e.data $(TABLEGEN)
$(GENSRC)/p.c: $(SRC)/data/p.data $(TABLEGEN)
$(GENSRC)/sboxes.c: $(SRC)/data/sboxes.data $(SBOXGEN)

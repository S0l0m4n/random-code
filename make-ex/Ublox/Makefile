# Makefile

CC := gcc
CFLAGS += -I . -I .. 	# ensure header files in directory above can be seen
CFLAGS += -g3 				# add debug info

# headers defined here will trigger a rebuild when changed
DEPS = Debug.h

# object files will be collected in OBJ_DIR
OBJ_DIR = obj

OBJ_FILES = main.o AtCmdRespFieldParser.o 			# list object files to build
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(OBJ_FILES))		# add OBJ_DIR prefix to filename

# default target
default: out

# make sure OBJ_DIR is created if it doesn't exist (see that it's added as
# dependency to the `out` target)
$(OBJ_DIR):
	mkdir $@

# rule for building all object files
$(OBJ_DIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# $(CC) -c -o $@    $<		 $(CFLAGS)
#						   ^     ^
#						   |     |
#						   |  src file
#					  obj file 

# generate executable from object files
out: $(OBJ_DIR) $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

#	$(CC) -o $@ $(OBJ) $(CFLAGS)
#						^    
#					  |  
#			  executable
#	NOTE: we use $(OBJ) not $< because the latter would also include $(OBJ_DIR)

# invalidate file names which match these targets 
.PHONY: clean out

# define a clean target which clears out the executable and the OBJ_DIR
clean:
	@if [ -e out ]; then \
		rm out; \
	fi; \
	if [ -d $(OBJ_DIR) ]; then \
		rm -r $(OBJ_DIR); \
	fi; 

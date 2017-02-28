PHASE1= phase1
PHASE22= phase2

CC= gcc
CFLAGS= -Werror -Wextra -Wall

SRC1= ChainMain.c
SRC2= ReconstitueReseau.c Hachage.c
SRC_SHARED= SVGwriter.c entree_sortie.c Chaine.c Reseau.c

OBJ1= $(SRC1:.c=.o)
OBJ2= $(SRC2:.c=.o)
OBJ_SHARED= $(SRC_SHARED:.c=.o)

INC= -I includes/

VPATH= srcs/ex1:srcs/ex2:srcs/ex4:srcs/shared

all: $(PHASE1) $(PHASE2)

### Executables 

$(PHASE1): $(OBJ1) $(OBJ_SHARED)
	@$(CC) $(CFLAGS) -o $@ $^ $(INC) -lm	
	@echo "Linking [$^]"

$(PHASE2: $(OBJ2) $(OBJ_SHARED)
	@$(CC) $(CFLAGS) -o $@ $^ $(INC) -lm
	@echo "Linking [$^]"


### Obj files

$(OBJ1): $(SRC1)
	@$(CC) $(CFLAGS) -c $^ $(INC)
	@echo "Compiling [$^]"

$(OBJ_SHARED): $(SRC_SHARED)
	@$(CC) $(CFLAGS) -c $^ $(INC)
	@echo "Compiling [$^]"

$(OBJ2): $(SRC2)
	@$(CC) $(CFLAGS) -c $^ $(INC)
	@echo "Compiling [$^]"
	
clean:
	rm $(OBJ1) $(OBJ2) $(OBJ_SHARED)

fclean: clean
	rm $(PHASE1) $(PHASE2)

re: fclean all

.PHONY: clean fclean re

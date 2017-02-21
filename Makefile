EX1= ex1
EX2= ex2
EX4= ex4

CC= gcc
CFLAGS= -Werror -Wextra -Wall

SRC1= ChainMain.c
SRC2= ReconstitueReseau.c
SRC4= Hachage.c HachageMain.c
SRC_SHARED= SVGwriter.c entree_sortie.c Chaine.c Reseau.c

OBJ1= $(SRC1:.c=.o)
OBJ2= $(SRC2:.c=.o)
OBJ4= $(SRC4:.c=.o)
OBJ_SHARED= $(SRC_SHARED:.c=.o)

INC= -I includes/

VPATH= srcs/ex1:srcs/ex2:srcs/ex4:srcs/shared

all: $(EX1) $(EX2) $(EX4)

### Executables 

$(EX1): $(OBJ1) $(OBJ_SHARED)
	@$(CC) $(CFLAGS) -o $@ $^ $(INC) -lm	
	@echo "Linking [$^]"

$(EX2): $(OBJ2) $(OBJ_SHARED)
	@$(CC) $(CFLAGS) -o $@ $^ $(INC) -lm
	@echo "Linking [$^]"

$(EX4): $(OBJ4) $(OBJ_SHARED)
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
	
$(OBJ4): $(SRC4)
	@$(CC) $(CFLAGS) -c $^ $(INC)
	@echo "Compiling [$^]"

clean:
	rm $(OBJ1) $(OBJ2) $(OBJ_SHARED) $(OBJ4)

fclean: clean
	rm $(EX1) $(EX2) $(EX4) 

re: fclean all

.PHONY: clean fclean re

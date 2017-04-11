MAINGRAPHE= MainGraphe

CC= gcc
CFLAGS= -Wextra -Wall

SRC_MAIN_GRAPHE= MainGraphe.c
SRC_SHARED= entree_sortie.c SVGwriter.c Graphe.c Struct_File.c Struct_Liste.c evaluation_NChaines.c Tas2Clefs.c

OBJ_MAIN_GRAPHE= $(SRC_MAIN_GRAPHE:.c=.o)
OBJ_SHARED= $(SRC_SHARED:.c=.o)

INC= -I includes/

VPATH= srcs

all: MainGraphe

### Executables

$(MAINGRAPHE): $(OBJ_MAIN_GRAPHE) $(OBJ_SHARED)
	@$(CC) $(CFLAGS) -o $@ $^ $(INC) -lm
	@echo "Linking [$^]"

### Obj files

$(OBJ_MAIN_GRAPHE): $(SRC_MAIN_GRAPHE)
	@$(CC) $(CFLAGS) -c $^ $(INC)
	@echo "Compiling [$^]"

$(OBJ_SHARED): $(SRC_SHARED)
	@$(CC) $(CFLAGS) -c $^ $(INC)
	@echo "Compiling [$^]"

clean:
	rm $(OBJ_MAIN_GRAPHE) $(OBJ_SHARED)

fclean: clean
	rm $(MAIN_GRAPHE)

re: fclean all

.PHONY: clean fclean re

CC = gcc

CFLAGS = -g -Wall -std=c17 -std=gnu99 -pedantic -I$(INCDIR)
LDLIBS = -lm -lMLV

# Répertoires source et include
SRCDIR = src
INCDIR = include
OBJ_DIR = bin

OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/moteur.o $(OBJ_DIR)/graphique.o $(OBJ_DIR)/vague.o $(OBJ_DIR)/game.o $(OBJ_DIR)/gemmes.o $(OBJ_DIR)/mana.o $(OBJ_DIR)/terrain.o

$(OBJ_DIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

tower: $(OBJ)
	$(CC) -o $(OBJ_DIR)/$@ $^ $(CFLAGS) $(LDLIBS)

moteur.o: $(SRCDIR)/moteur.c $(INCDIR)/moteur.h $(INCDIR)/graphique.h
	$(INCDIR)/vague.h $(INCDIR)/game.h $(INCDIR)/gemmes.h $(INCDIR)/terrain.h $(INCDIR)/mana.h
	$(CC) -c $< $(CFLAGS)

graphique.o: $(SRCDIR)/graphique.c $(INCDIR)/graphique.h $(INCDIR)/vague.h
	$(INCDIR)/game.h $(INCDIR)/gemmes.h $(INCDIR)/terrain.h $(INCDIR)/mana.h
	$(CC) -c $< $(CFLAGS)

vague.o: $(SRCDIR)/vague.c $(INCDIR)/vague.h $(INCDIR)/gemmes.h $(INCDIR)/terrain.h $(INCDIR)/mana.h
	$(CC) -c $< $(CFLAGS)

game.o: $(SRCDIR)/game.c $(INCDIR)/game.h $(INCDIR)/vague.h $(INCDIR)/terrain.h
	$(CC) -c $< $(CFLAGS)

gemmes.o: $(SRCDIR)/gemmes.c $(INCDIR)/gemmes.h
	$(CC) -c $< $(CFLAGS)

mana.o: $(SRCDIR)/mana.c $(INCDIR)/mana.h
	$(CC) -c $< $(CFLAGS)

terrain.o: $(SRCDIR)/terrain.c $(INCDIR)/terrain.h
	$(CC) -c $< $(CFLAGS)

main.o: $(SRCDIR)/main.c $(INCDIR)/moteur.h $(INCDIR)/graphique.h $(INCDIR)/vague.h
	$(INCDIR)/game.h $(INCDIR)/terrain.h $(INCDIR)/mana.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f bin/*

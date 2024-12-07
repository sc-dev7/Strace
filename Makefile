##
## EPITECH PROJECT, 2022
## Makefile
## File description:
## it makes files
##

# Compiler and flags
CC      = gcc
CFLAGS  += -std=gnu99 -W -Wall -Wextra -g

# Directories
SRCDIR  = src
OBJDIR  = obj
BINDIR  = .

# Drapeaux d'inclusion par défaut
INCFLAGS =

# Source files (recursively find all cpp files in src directory
# and its subdirectories)
SOURCES	= $(wildcard $(SRCDIR)/**/*.c $(SRCDIR)/*.c)

# Liste des fichiers objet
OBJECTS	:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Détecter le système d'exploitation
UNAME_S := $(shell uname -s)

# Executable name
NAME	=	strace

TARGET = $(BINDIR)/$(NAME)

# Règle par défaut
all: $(TARGET)

# Compilation des fichiers objet
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

# Création de l'exécutable
$(TARGET): $(OBJDIR) $(BINDIR) $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Nettoyer les fichiers objets
clean: $(OBJDIR)
	find $(OBJDIR) -name '*.o' -type f -exec rm -f {} \;

# Nettoyer les fichiers objets et l'exécutable
fclean: clean
	rm -f $(TARGET)

# Règle "re" pour reconstruire à partir de zéro
re: fclean all

# Phony targets
.PHONY: all clean fclean re

# Create directories if they don't exist
$(OBJDIR) $(BINDIR):
	mkdir -p $@

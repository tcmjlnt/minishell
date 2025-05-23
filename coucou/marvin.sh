#!/bin/bash

# cette ligne "shebang" permet de lancer le script en utilisant explicitement bash
# - il faut que ce soit la premiere ligne du script - c'est imperatif

#  L'ANSI-C quoting $'string' ne fonctionne pas de la même façon sur zsh
#  (regles de parsing plus strictes sur zsh)
#  du coup le script en l'etat fonctionnerait pas sur zsh

touch \"\\?\$*\'MaRViN\'*\$?\\\"
touch "\"\?\$*'MaRViN'*\$?\\\""
touch '"\?$*'\'MaRViN\''*$?\"'
touch '"\?$*'"'"MaRViN"'"'*$?\"'
touch $'"\\?$*\'MaRViN\'*$?\\"'


ls -lRa *MaRV* | cat -e
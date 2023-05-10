# README

## Description
Ce projet est un module de noyau Linux appelé "epirandom" qui permet de générer des caractères aléatoires en utilisant un alphabet spécifié ou l'alphabet par défaut (base64). Le comportement du module peut être contrôlé en utilisant un seuil spécifié qui peut être 1, 2, 3, 4 ou 5.

## Installation

### Prérequis
- Système d'exploitation Linux
- GCC installé
- Les headers du noyau Linux installés

### Étapes
1. Cloner ce repository : `git clone https://github.com/<votre nom d'utilisateur>/epirandom.git`
2. Accéder au répertoire : `cd epirandom`
3. Compiler le module : `make all`
4. Installer le module : `make device`

## Utilisation

Le module "epirandom" peut être utilisé en lisant à partir du fichier `/dev/epirandom`. Les différentes commandes disponibles sont :

- `cat /dev/epirandom` : affiche une sortie infinie de caractères aléatoires.
- `dd if=/dev/epirandom of=out bs=512 count=1` : génère une sortie de caractères aléatoires avec une limite spécifiée.
- `sudo insmod _epirandom.ko alphabet=a,b,c,X,Y,Z,0,1,2,3,4,5,6,7,8,9,\\,\\?,\\\'` : génère une sortie de caractères aléatoires en utilisant un alphabet spécifié.
- `sudo insmod _epirandom.ko alphabet='abcXYZ0123456789,?\'` : génère une sortie de caractères aléatoires en utilisant un alphabet spécifié.

Il est également possible de supprimer le module en utilisant la commande `make deldevice`.

## Paramètres

Le module "epirandom" prend en compte deux paramètres :
- `alphabet` : spécifie l'alphabet utilisé pour la génération de caractères aléatoires. Si ce paramètre n'est pas spécifié, l'alphabet par défaut (base64) sera utilisé.
- `threshold` : spécifie le seuil pour contrôler le comportement du module. Ce paramètre peut être 1, 2, 3, 4 ou 5. Si ce paramètre n'est pas spécifié, le seuil par défaut (4) sera utilisé.

Pour spécifier un paramètre, ajouter le paramètre à la commande `sudo insmod _epirandom.ko`, comme illustré ci-dessus.


# 2017-Moteur-de-recherche
Moteur de recherche en C

Projet réalisé par : Timon De Morel, Guillaume Malle et Guillaume Lictevout

Dans le cadre de notre 3ème année de licence informatique à Paris 7 Diderot

Je me suis principalement chargé du crawler, qui récupère le contenu des pages web grâce à libcurl, puis le donne au parser qui renvoit une liste de mots et une liste de liens, ensuite je donne les mots à l'index qui se chargera de les ajouter, et j'ajoute les nouveaux liens à la liste des prochaines pages à récupérer.

Je me suis aussi chargé de créé un graphe des liens visité avec dot.

## Installation nécessaires

* PCRE :

    * Télécharger pcre depuis http://www.pcre.org/
    * Extraire l'archive
    * Dans le répertoire pcre, lancer les commandes :

        * sudo ./configure --enable-utf8 --enable-unicode-properties
        * sudo make
        * sudo make install

* licburl :

    * Entrer les commandes (on peut avoir besoin d'être en admin) :

        * apt-get install libcurl4-openss1-dev
        * sudo apt-get install curl

* libxml :

    * Lancer la commande : sudo apt-get install libxml2-dev libxml2-doc

## Exécution

* Générer les exécutables en faisant un make dans le dossier rendu

* Pour le crawler : ./crawler [profondeur (int)] [url-1] [url-2] [...] [url-n]

* Pour le query_engine : ./query_engine

* Le crawler prend une profondeur (profondeur maximale d'exploration des pages) ainsi qu'une liste d'URLs qu'il visitera en premier (minimum un)

* Le graph se créé tout seul à la fin de l'exécution du crawler. Cependant, si une erreur survient inopinément, ou si vous interrompez le programme et que vous voulez voir le graph de ce qui a été exploré, lancez ces commandes dans la console :

    * echo "}" >> index.csv
    * dot -Tpng -o graphe.png graph.dot

## Spécifications

* Index :

    * Fonctions :

        * Ajouter un mot
        * Ajouter une URL à un mot
        * Supprimer une URL associée à un mot
        * Supprimer un mot (et donc toutes les URLs qui lui sont associées)
		* Renvoyer les URLs associées à un mot

	* Fonctionnement interne :

		* Vérifier la présence d'un mot
		* Vérifier la présence d'une url pour un mot donné
		* Générer une hashtable à partir du fichier index.csv. Chaque mot correspond à une clé, tandis qu'un tableau regroupant les URLs associées au mot correspond à la valeur.
		* Ecrire dans le fichier index.csv le contenu de la hashtable, selon le format de l'index décrit plus loin.

    * Format :

        * Le fichier index.csv est organisé de la manière suivante : chaque ligne correspond à un mot ; la première colonne contient un mot, puis les colonnes suivantes contiennent chacune une URL. Le mot est séparé
		des URLs par "}", et les URLs sont séparées les unes des autres par "{".


* Query Engine :

    * Fonctionnement :

        * L'utilisateur entre un mot, le programme charge l'index dans une hashtable, puis cherche le mot et renvoie les URLs correspondantes le cas échéant.

    * Fonctions :

        * Récupérer une requête
        * Charger l'index (fonction de l'index)
        * Récupérer les URLs (fonction de l'index)
        * Afficher les URLs

## Journal

Le journal de l'avancement du projet au long du semestre est disponible : Journal.txt

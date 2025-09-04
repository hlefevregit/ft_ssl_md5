# ft_ssl — Projet 42

Bienvenue dans le projet **ft_ssl**, une réimplémentation en C des algorithmes cryptographiques :

- ✅ Hash : `md5`, `sha256`
- ✅ Encodage : `base64`
- ✅ Chiffrement : `des-ecb`, `des-cbc`

---

## 🔍 Objectif

Reproduire les comportements des commandes Unix suivantes :

- `md5`, `sha256` (similaires à `openssl md5/sha256`)
- `base64 -e/-d`
- `openssl des-ecb -e/-d`
- `openssl des-cbc -e/-d`

En respectant les contraintes de l'école 42 (`open`, `read`, `write`, `malloc`, etc.).

---

## ⚙️ Compilation

```bash
make
```

---

## ▶️ Utilisation

```bash
./ft_ssl <command> [options]
```

### Commandes disponibles

| Commande   | Description                       |
|------------|-----------------------------------|
| `md5`      | Hash MD5                          |
| `sha256`   | Hash SHA-256                      |
| `base64`   | Encodage / décodage Base64        |
| `des-ecb`  | Chiffrement DES en mode ECB       |
| `des-cbc`  | Chiffrement DES en mode CBC       |

---

### Exemples

```bash
# MD5 depuis une string
./ft_ssl md5 -s "hello"

# SHA256 depuis un fichier
./ft_ssl sha256 fichier.txt

# Base64 encodage
echo -n "hello" | ./ft_ssl base64 -e

# Base64 décodage
echo "aGVsbG8=" | ./ft_ssl base64 -d

# DES ECB encryption avec clé
./ft_ssl des-ecb -e -k 0123456789ABCDEF -i in.txt

# DES CBC decryption avec vecteur d'init
./ft_ssl des-cbc -d -k 0123456789ABCDEF -v 1122334455667788 -i encrypted.txt
```

---

## 📁 Structure du projet

```
.
├── README.md
├── includes
│   ├── ft_ssl_des.h
│   ├── ft_ssl_md5.h
│   └── ft_types.h
├── src
│   ├── base64/
│   │   ├── encoder.c
│   │   ├── process.c
│   │   └── README.md       ← Explication complète base64
│   ├── des/
│   │   ├── ecb/
│   │   │   ├── algo.txt
│   │   │   ├── des_f.c
│   │   │   ├── encrypter.c
│   │   │   ├── padding.c
│   │   │   ├── process.c
│   │   │   ├── schedule.c
│   │   │   └── README.md   ← Explication complète DES-ECB
│   │   ├── cbc/
│   │   │   ├── algo.txt
│   │   │   ├── encrypter.c
│   │   │   ├── process.c
│   │   │   └── README.md   ← Explication DES-CBC
│   │   ├── process.c
│   │   └── args_process.c
│   ├── md5/
│   │   ├── padding.c
│   │   ├── result.c
│   │   ├── schema.txt
│   │   ├── hash.c
│   │   └── README.md       ← Explication complète MD5
│   ├── sha256/
│   │   ├── padding.c
│   │   ├── result.c
│   │   ├── hash.c
│   │   └── README.md       ← Explication complète SHA-256
│   ├── main.c
│   ├── parser.c
│   ├── reader.c
│   ├── flags.c
│   └── utils.c
└── Makefile
```

---

## 📚 Documentation technique

| Algo      | README dédié              | Schéma illustratif          |
|-----------|---------------------------|-----------------------------|
| MD5       | [`src/md5/README.md`](src/md5/README.md)             | ✓ intégré                   |
| SHA-256   | [`src/sha256/README.md`](src/sha256/README.md)       | ✓ intégré                   |
| Base64    | [`src/base64/README.md`](src/base64/README.md)       | ✓ intégré                   |
| DES-ECB   | [`src/des/ecb/README.md`](src/des/ecb/README.md)     | ✓ intégré                   |
| DES-CBC   | [`src/des/cbc/README.md`](src/des/cbc/README.md)     | en cours                    |

---

## ✅ Fonctionnalités prises en charge

- [x] `md5` avec `-p`, `-q`, `-r`, `-s`
- [x] `sha256` idem
- [x] Encodage et décodage `base64`
- [x] `des-ecb` chiffrement / déchiffrement
- [x] `des-cbc` avec IV (initialization vector)
- [x] Lecture depuis STDIN / fichier
- [x] Flags combinables (`-pqr`, etc.)
- [x] Implémentations internes (sans lib crypto externe)

---

## 📦 Exemple de test

```bash
# Encode → Decode
echo "foobar" | ./ft_ssl base64 -e | ./ft_ssl base64 -d
foobar

# DES ECB
echo "test1234" > file.txt
./ft_ssl des-ecb -e -k 0123456789ABCDEF -i file.txt -o out
./ft_ssl des-ecb -d -k 0123456789ABCDEF -i out
```

---

## 🔒 Notes techniques

- Respect strict des contraintes Unix (pas de `printf`, `fopen`, etc.)
- Gestion propre de la mémoire (valgrind OK)
- Découplage clair par modules (base64, md5, etc.)
- Utilisation de `t_context` pour centraliser les flags et données

---

## 📎 Références utiles

- [RFC 1321 — MD5](https://www.ietf.org/rfc/rfc1321.txt)
- [FIPS PUB 180-4 — SHA-256](https://csrc.nist.gov/publications/detail/fips/180/4/final)
- [DES (Data Encryption Standard)](https://datatracker.ietf.org/doc/html/rfc3602)
- `man base64`, `man openssl enc`

---

✨ Bon encodage, bon hashage, et bon chiffrement !

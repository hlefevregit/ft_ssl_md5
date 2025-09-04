# ft_ssl_des_ecb — Implémentation de l’algorithme DES en mode ECB

Ce projet implémente **DES-ECB** (Electronic Code Book) dans le cadre de `ft_ssl`.  
Ce fichier vous guide à travers le fonctionnement du mode ECB, le chiffrement DES et les étapes clés du traitement.

---

## 🔐 Vue d’ensemble de DES-ECB

```
+-------------------+
|    Message (M)    |
+-------------------+
          |
          v
+------------------------------+
|     Découpage en blocs       |
|        de 64 bits (8B)       |
+------------------------------+
     |       |       |
     v       v       v
 [Bloc 1] [Bloc 2] ... [Bloc N]
     |       |       |
     v       v       v
+-------------------------------+
|     Chiffrement DES (ECB)    |
|     Clé de 64 bits (56 bits) |
+-------------------------------+
     |       |       |
     v       v       v
 [Crypt1] [Crypt2] ... [CryptN]
     |       |       |
     v       v       v
+-------------------+
|  Données chiffrées |
+-------------------+
```

---

## 🔁 Fonctionnement du mode ECB

- **ECB** : chaque bloc est **indépendamment chiffré** avec la même clé.
- Avantage : simplicité
- Inconvénient : fuites d'information si blocs similaires (cf. [exemple du pingouin](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Electronic_Codebook_(ECB)))

---

## 🔧 Algorithme DES (Data Encryption Standard)

### Étapes générales :

1. **Initial Permutation (IP)**
2. **16 rounds de Feistel** :
   - Expansion E (32 → 48 bits)
   - XOR avec une sous-clé (48 bits)
   - Substitution (S-boxes) → 32 bits
   - Permutation (P)
3. **Permutation finale (FP)**

Chaque round utilise une **clé dérivée** de la clé principale via des permutations et décalages.

---

## 📦 Padding

DES opère par blocs de 8 octets (64 bits).  
Si le message n'est pas un multiple de 8, un **padding PKCS#7** est ajouté :

- Exemple :
  - message = `HELLO` (5 octets)
  - padding = 3 x `0x03`
  - résultat = `HELLO`

Au déchiffrement, on retire ces octets de padding.

---

## 📌 Exemple d’utilisation

### Chiffrement avec mot de passe :
```bash
$ echo "secret" > msg.txt
$ ./ft_ssl des-ecb -e -a -i msg.txt -P
Enter password:
Verifying - Enter password:
salt=4A8B1D83C674C8F7
key=31DDF2D4B9A48AA1
iv=F66E184F2DE65C4E
U2FsdGVkX1++qsmxl7P2Jg==
```

### Déchiffrement :
```bash
$ ./ft_ssl des-ecb -d -a -i encrypted.txt -k 31DDF2D4B9A48AA1
secret
```

---

## 🗂️ Fichiers du projet

- `process.c`          : traitement ECB
- `encrypter.c`        : logique DES par bloc
- `schedule.c`         : sous-clés de round
- `padding.c`          : padding/unpadding
- `../../base64`       : gestion base64 pour -a
- `main.c`             : parsing global
- `ft_ssl.h`           : structures et constantes

---

## 🧪 Test de conformité

Pour valider votre implémentation :

```bash
# Génère une clé (même que OpenSSL si salt et password identiques)
$ echo "hello" | openssl enc -des-ecb -e -nosalt -K 0123456789ABCDEF -in - -out out.bin
$ cat out.bin | ./ft_ssl des-ecb -d -k 0123456789ABCDEF
hello
```

---

✨ Merci pour votre lecture ! Le mode ECB est simple mais très instructif à implémenter.
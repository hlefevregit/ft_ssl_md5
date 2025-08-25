# ft_ssl_sha256 — Algorithme SHA-256 expliqué

Ce projet implémente **SHA-256** en C dans le cadre du projet `ft_ssl`.  
Ce fichier décrit en détail l'algorithme, depuis le padding jusqu'au hash final.

---

## 🔐 Vue d’ensemble de l’algorithme SHA-256

```
+-------------------+
|    Message (M)    |
+-------------------+
          |
          v
+-----------------------------+
|          Padding           |
|  - ajout de 0x80           |
|  - 0x00 jusqu’à 448 mod 512|
|  - taille (64 bits BE)     |
+-----------------------------+
          |
          v
+------------------------------+
|  Découpage en blocs de 512b |
+------------------------------+
     |       |       |
     v       v       v
 [Bloc 1] [Bloc 2] ... [Bloc N]
     \       |       /
      \      |      /
       v     v     v
+------------------------------------------+
| Pour chaque bloc :                       |
|                                          |
| 1. Diviser en 16 mots W[0..15]           |
| 2. Étendre à 64 mots W[16..63] :         |
|    W[t] = σ₁(W[t-2]) + W[t-7] +          |
|           σ₀(W[t-15]) + W[t-16]          |
|                                          |
| 3. Initialiser a–h avec H0..H7 initiaux  |
+------------------------------------------+
          |
          v
+---------------------------------------------------+
|        64 Rounds d'itérations :                   |
|                                                   |
| Pour i = 0 à 63 :                                 |
|   T1 = h + Σ₁(e) + Ch(e,f,g) + K[i] + W[i]        |
|   T2 = Σ₀(a) + Maj(a,b,c)                         |
|   h = g                                           |
|   g = f                                           |
|   f = e                                           |
|   e = d + T1                                      |
|   d = c                                           |
|   c = b                                           |
|   b = a                                           |
|   a = T1 + T2                                     |
+---------------------------------------------------+
          |
          v
+---------------------------------------------------+
| Ajout aux hash courants :                         |
| H₀ += a, H₁ += b, ..., H₇ += h                    |
+---------------------------------------------------+
          |
          v
+------------------------------+
|  Résultat final SHA-256      |
|  = H₀ || H₁ || ... || H₇     |
+------------------------------+
```

---

## 🔧 Fonctions utilisées

| Fonction        | Définition                                      |
|-----------------|--------------------------------------------------|
| Ch(x, y, z)     | `(x & y) ^ (~x & z)`                            |
| Maj(x, y, z)    | `(x & y) ^ (x & z) ^ (y & z)`                   |
| Σ₀(x)           | `ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22)`           |
| Σ₁(x)           | `ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25)`           |
| σ₀(x)           | `ROTR(x,7) ^ ROTR(x,18) ^ SHR(x,3)`             |
| σ₁(x)           | `ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10)`           |

- `ROTR(x, n)` : rotation droite de `n` bits
- `SHR(x, n)` : décalage logique droit (remplit par 0)

---

## 📥 Padding

- Ajout de `0x80` (bit `1`)
- Complément en `0x00` jusqu'à ce que `(message_len + 1 + padding) ≡ 448 mod 512`
- Ajout de la **longueur du message original** codée sur 64 bits **big-endian**

---

## 📊 Expansion des mots W[64]

SHA-256 travaille avec un tableau `W[64]` :

- Les 16 premiers mots sont directement issus du bloc
- Les 48 suivants sont calculés par :

```c
W[i] = σ₁(W[i-2]) + W[i-7] + σ₀(W[i-15]) + W[i-16];
```

Cela permet d'ajouter de la diffusion à travers les blocs.

---

## 🧠 Récapitulatif final

- Initialisation de 8 registres H0 à H7 (valeurs constantes)
- Pour chaque bloc :
  - Calcul de W[64]
  - Application des 64 rounds
  - Mise à jour des registres
- À la fin : concaténation des H0 à H7 pour donner le **SHA-256 digest**

---

## ✅ Exemple d’utilisation

```bash
$ echo -n "hello" | ./ft_ssl sha256
(stdin)= 2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824
```

---

## 📂 Fichiers du projet

- `main.c`          : parsing global
- `sha256/`         : implémentation complète SHA-256
- `ft_ssl.h`        : structures partagées
- `Makefile`        : compilation simple

---

## 🧪 Test de conformité

Ce SHA-256 doit reproduire le comportement de :

```bash
$ echo -n "hello" | openssl sha256
```

---

✨ Merci pour votre lecture, et bonne exploration du hashage !

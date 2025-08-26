# ft_ssl_md5 — Algorithme MD5 expliqué

Ce projet implémente l'algorithme **MD5** (et SHA256) en C, avec les contraintes UNIX (open, read, write, malloc...). Ce fichier résume **tout le fonctionnement interne de MD5** étape par étape.

---

## 🧱 Structure générale de l’algorithme MD5

```
+-------------------+
|    Message (M)    |
+-------------------+
          |
          v
+-------------------+
|     Padding       |
|  - ajout de 0x80  |
|  - puis 0x00s     |
|  - + taille (64b) |
+-------------------+
          |
          v
+------------------------------+
| Découpage en blocs de 512 b |
+------------------------------+
     |       |       |
     v       v       v
 [Bloc 1] [Bloc 2] ... [Bloc N]
     \       |       /
      \      |      /
       v     v     v
+------------------------------------------+
| Pour chaque bloc :                       |
| 1. Initialiser A, B, C, D                |
| 2. Diviser bloc en 16 mots (M[0..15])    |
| 3. Appliquer 64 opérations :             |
|    - 4 rounds × 16 itérations            |
|    - Fonctions F, G, H, I                |
|    - Décalages (s)                       |
|    - Constantes K[i]                     |
| 4. Mettre à jour A, B, C, D              |
+------------------------------------------+
          |
          v
+--------------------------+
|   A || B || C || D       |
| (en little endian)       |
+--------------------------+
          |
          v
+--------------------------+
|     Résultat final MD5   |
| (128 bits = 32 hex)      |
+--------------------------+
```

---

## 🧩 Padding

Le padding suit les règles :

- Ajout de `0x80` (bit 1)
- Ajout de `0x00` jusqu’à ce que la taille ≡ 56 mod 64
- Ajout de la **longueur du message** en bits (sur 64 bits, little-endian pour MD5)

### Exemple visuel :

```
Message : "abc" (3 octets = 24 bits)

→ 'abc' + 0x80 + 0x00 * (56 - 4) + [24 bits codés sur 64 bits]
→ Total = 64 octets (512 bits)
```

### Cas particulier : message long (> 448 bits mod 512)

```
Supposons un message de 2001 bits (~250.125 octets)

→ Bloc 1 : 512 bits (du message original)
→ Bloc 2 : suite + 0x80 + 0x00...
→ Bloc 3 : uniquement la taille du message (64 bits)
```

---

## 🔁 Rounds de MD5

MD5 comporte **64 itérations** divisées en **4 rounds** :

| Round | i ∈         | Fonction logique     | Formule de g[i]           |
|-------|-------------|----------------------|----------------------------|
| 1     | 0–15        | F = (B & C) \| (~B & D) | `g = i`                    |
| 2     | 16–31       | G = (B & D) \| (C & ~D) | `g = (5×i + 1) mod 16`     |
| 3     | 32–47       |  H = B ^ C ^ D        | `g = (3×i + 5) mod 16`     |
| 4     | 48–63       | I = C ^ (B \| ~D)    | `g = (7×i) mod 16`         |

---

## 📊 Décalages s[i]

```c
const uint32_t S[64] = {
  // Round 1
   7, 12, 17, 22,  7, 12, 17, 22,
   7, 12, 17, 22,  7, 12, 17, 22,
  // Round 2
   5,  9, 14, 20,  5,  9, 14, 20,
   5,  9, 14, 20,  5,  9, 14, 20,
  // Round 3
   4, 11, 16, 23,  4, 11, 16, 23,
   4, 11, 16, 23,  4, 11, 16, 23,
  // Round 4
   6, 10, 15, 21,  6, 10, 15, 21,
   6, 10, 15, 21,  6, 10, 15, 21
};
```

---

## ⚙️ Itération complète

```c
F = round_function(B, C, D);
T = A + F + M[g[i]] + K[i];
T = ROTATE_LEFT(T, s[i]);
A = D;
D = C;
C = B;
B = B + T;
```

Après les 64 itérations :
```c
A += AA;
B += BB;
C += CC;
D += DD;
```

Résultat final (little endian) : `A || B || C || D` → 128 bits (32 hex chars)

---

## ✅ Exemple de résultat

```bash
$ echo -n "42 is nice" | ./ft_ssl md5
(stdin)= 35f1d6de0302e2086a4e472266efb3a9
```

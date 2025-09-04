
# ft_ssl_base64 — Encodage Base64 expliqué

Ce projet implémente **Base64** en C dans le cadre du projet `ft_ssl`.  
Ce fichier décrit en détail l’algorithme d’encodage et de décodage Base64, ainsi qu’un schéma explicatif des transformations internes.

---

## 🧠 Qu’est-ce que Base64 ?

Base64 est un système d'encodage binaire-vers-texte permettant de représenter des données binaires en utilisant uniquement des caractères ASCII.  
Il est couramment utilisé dans les emails, les URL ou les certificats pour transmettre des données de manière sûre.

---

## 🔧 Principe général de l'encodage

L'encodage Base64 fonctionne par blocs de **3 octets (24 bits)** convertis en **4 groupes de 6 bits**.

```
[ Octet 1 ] [ Octet 2 ] [ Octet 3 ]   (3 x 8 bits = 24 bits)
      ↓             ↓             ↓
+--------+--------+--------+--------+
| 6 bits | 6 bits | 6 bits | 6 bits | → Index dans table base64 (64 symboles)
+--------+--------+--------+--------+
```

### 🧮 Exemple de conversion :

Imaginons que nous ayons les 3 octets suivants :

- Octet A = `01000011` (0x43 = 'C')
- Octet B = `01100001` (0x61 = 'a')
- Octet C = `01110100` (0x74 = 't')

Concaténés :  
`010000110110000101110100`

Découpés en groupes de 6 bits :  
`010000` `110110` `000101` `110100`

Ce qui donne en base64 :

| Bits      | Valeur décimale | Caractère base64 |
|-----------|------------------|------------------|
| 010000    | 16               | Q                |
| 110110    | 54               | 2                |
| 000101    | 5                | F                |
| 110100    | 52               | 0                |

**Résultat :** `Q2F0`

---

## 🧩 Table de correspondance base64

La table base64 standard contient 64 caractères :

```
ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
```

Chaque index (0–63) correspond à un groupe de 6 bits.

---

## 🧼 Padding

Quand l'entrée ne contient pas un multiple de 3 octets :

- S’il reste 2 octets → on ajoute `=` à la fin (1 padding)
- S’il reste 1 octet → on ajoute `==` à la fin (2 paddings)

Exemples :

| Entrée     | Binaire                        | Encodé base64   |
|------------|--------------------------------|------------------|
| foobar     | `666f6f626172`                | `Zm9vYmFy`       |
| foobar\n  | `666f6f6261720A`              | `Zm9vYmFyCg==`   |

---

## 🔁 Décodage

Le processus est inversé :

- Lire 4 caractères → 4 * 6 = 24 bits
- Reconvertir en 3 octets (3 * 8 bits)

---

## 📂 Fichiers du projet

- `encoder.c`       : encode/decode de base64
- `process.c` : gestion des tables, padding, nettoyage
- `ft_ssl_des.h`       : structures partagées
- `main.c`         : parsing global et dispatch
- `Makefile`       : compilation

---

## ✅ Exemple d’utilisation

```bash
$ echo -n "foobar" | ./ft_ssl base64 -e
Zm9vYmFy

$ echo "Zm9vYmFy" | ./ft_ssl base64 -d
foobar
```

---

## 🧪 Test de conformité

Comparez avec OpenSSL :

```bash
$ echo -n "foobar" | openssl base64
Zm9vYmFy

$ echo "Zm9vYmFy" | openssl base64 -d
foobar
```

---

✨ Merci pour votre lecture, et bon encodage !

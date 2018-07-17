// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup hash Hash

    Provides a dynamically sized hash table implementation based upon sparse
    groups. Sparse groups are arrays of elements that use a single bit to
    indicate occupancy. Only the elements of the array that are occupied are
    stored, this results in a compact representation of a hash table at the
    cost of some extra processing to perform the insertion.
    @{
*/

typedef struct _HashTable HashTable;

/** @brief Allocate a HashTable.
    @param size is the initial size of the HashTable
*/
HashTable *hash_new (int size);

/** @brief Free a HashTable.
    @param ht is a pointer to a HashTable
*/
void hash_free (HashTable *ht);

/** @brief Put the data item into the HashTable. 
    @param ht is a pointer to a HashTable
    @param data is pointer to the data item
*/
void hash_put (HashTable *ht, void *data);

/** @brief Delete the hash entry that matches the key.
    @param ht is a pointer to a HashTable
    @param key is a pointer to a hash key
    @returns the hash entry associated with the key
*/
void *hash_delete (HashTable *ht, void *key);

/** @brief Get the data item that matches the key.
    @param ht is a pointer to a HashTable
    @param key is a pointer to a hash key
    @returns the hash entry associated with the key
*/
void *hash_get (HashTable *ht, void *key);

/** @brief Allocate a new HashTable with entries hashed by character strings.
    @param size is the initial size of the HashTable
    @param get_key is a user supplied function to get the key from a hash entry
    @returns a new HashTable
*/
HashTable *new_string_hash (int size, void *(*get_key) (void *data));

/** @brief Allocate a new HashTable with entries hashed by 64-bit integers.
    @param size is the initial size of the HashTable
    @param get_key is a user supplied function to get the key from a hash entry
    @returns a new HashTable
*/
HashTable *new_int64_hash (int size, void *(*get_key) (void *data));

/** @brief Create a global hash table with functions to initialize the
    HashTable, find, insert, and delete entries from the HashTable.
*/
#define global_hash(name, kind, size) \
  HashTable *name##_hash = NULL;		   \
  void *find_##name (void *key) {		   \
    return hash_get (name##_hash, key);	   \
  }						   \
  void insert_##name (void *data) {		   \
    hash_put (name##_hash, data);		   \
  }						   \
  void *delete_##name (void *key) {		   \
    return hash_delete (name##_hash, key);	   \
  }						   \
  void name##_init () {					\
    name##_hash = new_##kind##_hash (size, name##_key);	\
  }

typedef struct {
  void *g, *ht; int i;
} HashPointer;

void *hash_next (HashPointer *p);

void *hash_iterate (HashPointer *p, HashTable *ht);

void hash_erase (HashPointer *p);

#define foreach_h(h, hp, ht)					\
  for (h = hash_iterate (hp, ht); h != NULL; h = hash_next (hp))

/** @} */

#ifndef HEADER_ONLY

/* sparsehash implementation inspired by the google sparsehash concept
   https://github.com/sparsehash/sparsehash */

// SparseGroup contains up to 58 elements
// count and occupancy are marked by bits 
typedef struct {
  void **slot;   // elements
  uint64_t bits; // count (6) / bitmap (58)
} SparseGroup;

// count the bits up to bit position i
// https://graphics.stanford.edu/~seander/bithacks.html
int bit_rank (uint64_t bits, int i) {
  if (i == 0) return 0;
  // Shift out bits after given position.
  uint64_t r = bits << (64 - i);
  // Count set bits in parallel.
  // r = (r & 0x5555...) + ((r >> 1) & 0x5555...);
  r = r - ((r >> 1) & ~0UL/3);
  // r = (r & 0x3333...) + ((r >> 2) & 0x3333...);
  r = (r & ~0UL/5) + ((r >> 2) & ~0UL/5);
  // r = (r & 0x0f0f...) + ((r >> 4) & 0x0f0f...);
  r = (r + (r >> 4)) & ~0UL/17;
  // r = r % 255;
  r = (r * (~0UL/255)) >> 56;
  return (int)r;
}

// insert data into sparesgroup g at position i
void sg_insert (SparseGroup *g, int i, void *data) {
  int s, count, size;
  uint64_t bit = 1ull << i;
  s = bit_rank (g->bits, i);
  count = g->bits >> 58; count++;
  // printf ("sg_insert %x %d %d %d\n", g, i, s, count);
  size = count * sizeof (void *);
  if (g->slot) {
    g->slot = realloc (g->slot, size);
    memmove (g->slot+s+1, g->slot+s, (count - (s+1)) * sizeof (void *));
  } else g->slot = calloc (1, size);
  g->bits += 1ull << 58; // add one to the count
  g->bits |= bit; // mark the element as occupied
  g->slot[s] = data;
}

#define sg_empty(g, i) ((g->bits & (1ull << i)) == 0)
#define sg_element(g, i) (g->slot + bit_rank (g->bits, i))

// HashTable is an array of SparseGroups
typedef struct _HashTable {
  void *(*get_key) (void *);
  int (*hash) (void *);
  int (*compare) (void *, void *);
  int i, items, min, max, size;
  SparseGroup *table, *g, *last;
} HashTable;

void *hash_next (HashPointer *p) {
  HashTable *ht = p->ht;
  SparseGroup *sg = p->g;
  int count = sg->bits >> 58;
  if (++p->i == count) {
  next_group:
    if (p->g == ht->last) return NULL;
    p->g = ++sg; count = sg->bits >> 58; p->i = 0;
  }
  while (p->i < count) {
    void *data = sg->slot[p->i];
    if (data) return data; p->i++;
  } goto next_group;
}

void *hash_iterate (HashPointer *p, HashTable *ht) {
  p->g = ht->table; p->ht = ht; p->i = -1;
  return hash_next (p);
}

void hash_erase (HashPointer *p) {
  HashTable *ht = p->ht; SparseGroup *sg = p->g;
  sg->slot[p->i] = NULL; ht->items--;
}

#define hash_mark(ht, h, j) { ht->g = h; ht->i = j; } 
#define hash_insert(ht, data) sg_insert (ht->g, ht->i, data);

// return pointer to hash entry with given key or NULL if non-existent
void **hash_find (HashTable *ht, void *key) {
  int mask = ht->size - 1, probes = 0;
  int index = ht->hash (key) & mask;
  ht->g = NULL;
  while (1) {
    SparseGroup *g = ht->table + index / 58;
    int i = index % 58; // group index
    if (!g->slot || sg_empty (g, i)) {
      if (!ht->g) hash_mark (ht, g, i);
      return NULL;
    } else {
      void **e = sg_element (g, i);
      if (*e) {
	if (ht->compare (key, ht->get_key (*e)) == 0)
	  return e;
      } else if (!ht->g)      // deleted element
	hash_mark (ht, g, i); // mark the location
    }
    index = (index + ++probes) & mask;
  }
}

void hash_init (HashTable *ht, int size) {
  int groups = (size + 57) / 58;
  ht->size = size; ht->min = (size * 40) / 100;
  ht->max = (size * 80) / 100;
  ht->table = calloc (1, sizeof (SparseGroup) * groups);
  ht->last = ht->table + (groups - 1);
}

// size must be power of two
HashTable *hash_new (int size) {
  HashTable *ht = calloc (1, sizeof (HashTable));
  hash_init (ht, size);
  return ht;
}

void hash_free (HashTable *ht) {
  SparseGroup *g = ht->table;
  do {
    if (g->slot) free (g->slot); g++;
  } while (g < ht->last);
  free (ht->table); free (ht);
}

HashTable *hash_resize (HashTable *ht, int size) {
  HashTable *gt = hash_new (size);
  HashPointer p; void *data;
  gt->get_key = ht->get_key;
  gt->hash = ht->hash;
  gt->compare = ht->compare;
  gt->items = ht->items;
  foreach_h (data, &p, ht) hash_insert (gt, data);
  hash_free (ht); return gt;
}

void hash_put (HashTable *ht, void *data) {
  void *key = ht->get_key (data), **e;
  if (e = hash_find (ht, key)) *e = data;
  else {
    if (ht->items == ht->max)
      ht = hash_resize (ht, ht->size << 1);
    hash_insert (ht, data);
    ht->items++;
  }
}

void *hash_delete (HashTable *ht, void *key) {
  void **e, *tmp = NULL;
  if (e = hash_find (ht, key)) {
    tmp = *e; *e = NULL;
    if (ht->items == ht->min)
      ht = hash_resize (ht, ht->size >> 1);
    ht->items--;
  } return tmp;
}

void *hash_get (HashTable *ht, void *key) {
  void **e = hash_find (ht, key);
  return e? *e : NULL;
}

// http://www.cse.yorku.ca/~oz/hash.html (djb2)
int string_hash (void *data) {
  char *str = data; int c;
  unsigned long hash = 5381;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash;
}

HashTable *new_string_hash (int size, void *(*get_key) (void *data)) {
  HashTable *ht = hash_new (size);
  ht->compare = (int (*) (void *, void *))strcmp;
  ht->hash = string_hash;
  ht->get_key = get_key;
  return ht;
}

int int64_compare (void *a, void *b) {
  int64_t *x = a, *y = b;
  return !(*x == *y);
}

// Thomas Wang - 64bit hash function
// https://gist.github.com/badboy/6267743
int int64_hash (void *data) {
  uint64_t key = *(uint64_t *)data;
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return (int)key;
}

HashTable *new_int64_hash (int size, void *(*get_key) (void *data)) {
  HashTable *ht = hash_new (size);
  ht->compare = int64_compare;
  ht->hash = int64_hash;
  ht->get_key = get_key;
  return ht;
}

int int128_compare (void *a, void *b) {
  return memcmp (a, b, 16);
}

int int128_hash (void *data) {
  char *str = data; int n = 0;
  unsigned long hash = 5381;
  while (n++ < 16)
    hash = ((hash << 5) + hash) + *str++; // hash * 33 + c
  return hash;
}

HashTable *new_int128_hash (int size, void *(*get_key) (void *data)) {
  HashTable *ht = hash_new (size);
  ht->compare = int128_compare;
  ht->hash = int128_hash;
  ht->get_key = get_key;
  return ht;
}

#endif

// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup security Security
    @{
*/

void print_bytes (unsigned char *data, int n);

/** @brief Generate SFDI from LFDI hash.
    @param lfdi is the 20 byte LFDI
    @returns the computed SFDI result
*/
uint64_t sfdi_gen (uint8_t *lfdi);

/** @brief Generate SFDI and LFDI hash values from a device certificate.
    @param lfdi is a 20 byte buffer that stores the LFDI result
    @param buffer contains the device certificate, it must be a multiple of
    SHA256_HASH_SIZE (64 bytes) and have at least 9 bytes of padding, this
    can be obtained by allocating a buffer of size sha256_size (length).
    @param length is the length of the device certificate
    @returns the SFDI hash result truncated from the LFDI result
*/
uint64_t lfdi_hash (uint8_t *lfdi, uint8_t *buffer, int length);

/** @brief Generate SFDI and LFDI hash values from a device certificate file.
    @param lfdi is a 20 byte buffer that stores the LFDI result
    @param path is the path of the device certificate
    @returns the SFDI hash result truncated from the LFDI result
*/
uint64_t lfdi_gen (uint8_t *lfdi, const char *path);

/** @brief Generate SFDI and LFDI hash values from a device certificate file and
    print them to the console.
    @param lfdi is a 20 byte buffer that stores the LFDI result
    @param path is the path of the device certificate
    @returns the SFDI hash result truncated from the LFDI result
*/
uint64_t load_device_cert (uint8_t *lfdi, const char *path);

/** @brief Initialize the device SFDI and LDFI.

    Initialize the global variable device_lfdi and device_sfdi by computing the
    hash of a device certificate.
    @param path is the path of the device certificate.
 */
void security_init (const char *path);

/** @} */

#ifdef HEADER_ONLY

extern uint8_t device_lfdi[20];
extern uint64_t device_sfdi;

#else

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

uint8_t device_lfdi[20];
uint64_t device_sfdi = 0;

void print_bytes (unsigned char *data, int n) { int i;
  for (i = 0; i < n; i++) printf ("%02x", data[i]);
}

int check_digit (uint64_t x) { int sum = 0;
  while (x) { sum += x % 10; x /= 10; }
  return (10 - (sum % 10)) % 10;
}

uint64_t sfdi_gen(uint8_t *lfdi) {
  int i = 0; uint64_t sfdi = 0;
  while (i < 5) sfdi = (sfdi << 8) + lfdi[i++];
  sfdi >>= 4; sfdi = sfdi * 10 + check_digit (sfdi);
  return sfdi;
}

uint64_t lfdi_hash (uint8_t *lfdi, uint8_t *buffer, int length) {
  uint8_t hash[SHA256_HASH_SIZE];
  sha256 (hash, buffer, length);
  memcpy (lfdi, hash, 20);
  return sfdi_gen (lfdi);
}

uint64_t lfdi_gen (uint8_t *lfdi, const char *path) {
  uint8_t *buffer; int length; uint64_t sfdi;
  FILE *f = fopen (path, "r");
  if (!f) { printf ("error opening file %s\n", path); exit (0); }
  fseek (f, 0, SEEK_END); length = ftell (f); fseek (f, 0, SEEK_SET);
  buffer = malloc (sha256_size (length));
  fread (buffer, length, 1, f); fclose (f);
  sfdi = lfdi_hash (lfdi, buffer, length);
  free (buffer); return sfdi;
}

uint64_t load_device_cert (uint8_t *lfdi, const char *path) {
  uint64_t sfdi = lfdi_gen (lfdi, path);
  printf ("load device certificate: %s\n", path);
  printf ("  lfdi: "); print_bytes (lfdi, 20);
  printf ("\n  sfdi: %" PRIu64 "\n", sfdi);
  return sfdi;
}

// generate lfdi/sfdi from device certificate 
void security_init (const char *path) {
  device_sfdi = load_device_cert (device_lfdi, path);
}

#endif

// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#define CIPHER_LIST "ECDHE-ECDSA-AES128-CCM8"

/** @addtogroup security
    @{
*/

typedef int (*VerifyFunc) (void *context, uint8_t *cert, int length);

/** @brief Initialize the TLS library.

    Initialize the TLS library calling the appropriate functions. Load the
    device certificate 'path'.x509 and use the device private key
    'path'.pem, where 'path' is the first function parameter.
    @param path is used to identify the device certificate and private key
    files, 'path'.x509 and 'path'.pem respectively.
    @param verify is called as extra step to verify the client TLS certificate.
    Certificates are in any case are verified against their CA certificate
    chain, this extra step this can be used to impliment a filter by only
    accepting clients with the right credentials (SFDI, LFDI), this parameter
    can be NULL to indicate not to take this step.
    @returns the path of the device certificate 'path'.x509
*/
void tls_init (const char *path, VerifyFunc verify);

/** @brief Load a CA certificate.

    This certificate can be sent along with client certificate as part of
    establishing a TLS session, it can also be used in CA certificate chain
    verification of a client certificate.
    @param path is the file name of the CA certificate
*/
void load_cert (const char *path);

/** @brief Load a CA certificate directory.

    Call @ref load_cert for every file in the directory.
    @param path is the file name of the directory to load
*/
void load_cert_dir (const char *path);

/** @} */

#ifndef HEADER_ONLY

#include <openssl/opensslconf.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

void print_ssl_error (char *func) {
  char buffer[120]; int err;
  while (err = ERR_get_error()) {
    ERR_error_string (err, buffer);
    printf ("%s: %d, %s\n", func, err, buffer); fflush (stdout);
  } 
}

SSL_CTX *ssl_ctx = NULL;

int bio_read (BIO *bio, char *buffer, int size) {
  TcpPort *p = BIO_get_data (bio);
  int n = net_read (p, buffer, size);
  // printf ("ssl_read %d, %d\n", size, n); fflush (stdout);
  BIO_clear_retry_flags (bio);
  if (n == -1 && event_pending (p))
      BIO_set_retry_read (bio);
  return n;
}

int bio_write (BIO *bio, const char *buffer, int size) {
  TcpPort *p = BIO_get_data (bio);
  int n = net_write (p, buffer, size);
  BIO_clear_retry_flags (bio);
  if (n == -1 && event_pending (p))
    BIO_set_retry_write (bio);
  return n;
}

long bio_ctrl (BIO *bio, int cmd, long larg, void *parg) {
  // printf ("ssl_crtl: %d\n", cmd); fflush (stdout);
  if (cmd == BIO_CTRL_FLUSH) return 1;
  return 0;
}

BIO_METHOD *ssl_bio;

void init_bio () {
  int type = BIO_get_new_index () | BIO_TYPE_SOURCE_SINK;
  ssl_bio = BIO_meth_new (type, "");
  BIO_meth_set_read (ssl_bio, bio_read);
  BIO_meth_set_write (ssl_bio, bio_write);
  BIO_meth_set_ctrl (ssl_bio, bio_ctrl);
}

typedef int (*VerifyFunc) (void *ctx, uint8_t *cert, int length);

VerifyFunc _verify_peer = NULL;

/* verify the presence of critical and non-critical extensions required by
   sections 8.11.6, 8.11.8, and 8.11.10 */
int check_cert (int status, X509 *cert) {
  int count = X509_get_ext_count (cert), i;
  for (i = 0; i < count; i++) {
    X509_EXTENSION *ext = X509_get_ext (cert, i);
    ASN1_OBJECT *obj = X509_EXTENSION_get_object (ext);
    int critical = X509_EXTENSION_get_critical (ext);
    unsigned nid = OBJ_obj2nid (obj);
    switch (nid) {
    case NID_policy_mappings:
    case NID_name_constraints: return 0;
    case NID_subject_alt_name:
      if (X509_check_ca (cert)) return 0;
    case NID_key_usage:
    case NID_basic_constraints:
    case NID_certificate_policies:
      if (!critical) return 0; break;
    case NID_authority_key_identifier:
    case NID_subject_key_identifier:
    default: if (critical) return 0;
    }
  } return status;
}

int verify_peer (int status, X509_STORE_CTX *ctx) {
  SSL *ssl = X509_STORE_CTX_get_ex_data
    (ctx, SSL_get_ex_data_X509_STORE_CTX_idx ());
  void *user = SSL_get_app_data (ssl);
  X509 *x509 = X509_STORE_CTX_get0_cert (ctx), // peer cert
    *curr = X509_STORE_CTX_get_current_cert (ctx);
  status = check_cert (status, x509);
  if (x509 != curr) return status; // only check the peer cert
  if (x509 && _verify_peer) {
    int length = i2d_X509 (x509, NULL);
    uint8_t *cert, *p = malloc (sha256_size (length)); cert = p;
    i2d_X509 (x509, &p);
    if (!_verify_peer (user, cert, length)) status = 0;
    free (cert);
  } return status;
}

const uint8_t *ssl_session_id (void *ssl) {
  SSL_SESSION *ss = SSL_get_session (ssl);
  return SSL_SESSION_get0_id_context (ss, NULL);
}

int ssl_load_cert (const char *path) {
  return SSL_CTX_load_verify_locations (ssl_ctx, path, NULL);
}

void load_cert (const char *path) {
  if (ssl_load_cert (path) != 1) {
    printf ("load_cert: error opening certificate file: %s\n", path);
    exit (0);
  } printf ("loaded certificate \"%s\"\n", path);
}

void _load_cert (const char *path, void *ctx) {
  if (strstr (path, ".pem")) load_cert (path);
}

void load_cert_dir (const char *path) {
  process_dir (path, NULL, _load_cert);
}

int _tls_initialized = 0;

void tls_init (const char *path, VerifyFunc verify) {
  int ret, type; char *private = strdup (path), *ext;
  if ((ssl_ctx = SSL_CTX_new (TLS_method ())) == NULL) {
    print_ssl_error ("tls_init"); exit (0);
  }
  init_bio (); _verify_peer = verify;
  SSL_CTX_set_verify (ssl_ctx, SSL_VERIFY_PEER |
		      SSL_VERIFY_FAIL_IF_NO_PEER_CERT, verify_peer);
  if (!SSL_CTX_set_cipher_list (ssl_ctx, CIPHER_LIST)) {
    printf ("tls_init: error selecting %s cipher list\n", CIPHER_LIST);
    exit (0);
  }
  if (ext = strstr (private, ".x509")) {
    strcpy (ext, ".pem"); type = SSL_FILETYPE_ASN1;
  } else type = SSL_FILETYPE_PEM;
  if (SSL_CTX_use_certificate_file (ssl_ctx, path, type) != 1) {
    printf ("tls_init: error opening certificate file: %s\n", path);
    exit (0);
  }
  if ((ret = SSL_CTX_use_PrivateKey_file
       (ssl_ctx, private, SSL_FILETYPE_PEM)) != 1) {
    printf ("tls_init: error (%d) opening private key file: %s\n",
	    ret, private);
    exit (0);
  } free (private); _tls_initialized = 1;
}

void *ssl_new (void *conn) {
  if (_tls_initialized) {
    BIO *bio = BIO_new (ssl_bio);
    SSL *ssl = SSL_new (ssl_ctx);
    BIO_set_data (bio, conn);
    BIO_set_init (bio, 1);
    SSL_set_bio (ssl, bio, bio);
    SSL_set_app_data (ssl, conn);
    return ssl;
  } printf ("ssl_new: error, TLS library not initialized\n"); exit (0);
}

#define ssl_free(ssl) SSL_free (ssl)
#define ssl_close(ssl) SSL_shutdown (ssl)

int ssl_ret, ssl_err;

#define ssl_pending() \
  (ssl_err == SSL_ERROR_WANT_READ || ssl_err == SSL_ERROR_WANT_WRITE)

int ssl_handshake (void *ssl) { ERR_clear_error ();
  if ((ssl_ret = SSL_do_handshake (ssl)) == 1) return 1;
  ssl_err = SSL_get_error (ssl, ssl_ret);
  return 0;
}

int ssl_read (void *ssl, char *buffer, int size) { int ret;
  ERR_clear_error (); ret = SSL_read (ssl, buffer, size);
  if (ret <= 0) ssl_err = SSL_get_error (ssl, ret); 
  return ret;
}

int ssl_write (void *ssl, const char *data, int length) { int ret;
  ERR_clear_error (); ret = SSL_write (ssl, data, length);
  if (ret <= 0) ssl_err = SSL_get_error (ssl, ret);
  return ret;
}

void ssl_accept (void *ssl) {
  SSL_set_accept_state (ssl);
}

void ssl_connect (void *ssl) {
  SSL_set_connect_state (ssl);
}

#endif

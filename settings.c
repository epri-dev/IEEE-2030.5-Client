// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

typedef struct {
  SE_DERAvailability_t *dera;
  SE_DERCapability_t *dercap;
  SE_DERSettings_t *derg;
  SE_DERStatus_t *ders;
} Settings;

void load_settings (const char *name, void *ctx) {
  Settings *ds = ctx;
  char *buffer = file_read (name, NULL),
    *data = utf8_start (buffer);
  Parser *p = parser_new ();
  void *obj; int type;
  parse_init (p, &se_schema, data);
  obj = parse_doc (p, &type);
  if (parse_error (p)) {
    printf ("load_device_setting: error parsing XML file %s\n", name);
    print_parse_stack (p); exit (0);
  }
  switch (type) {
  case SE_DERAvailability: ds->dera = obj; break;
  case SE_DERCapability: ds->dercap = obj; break;
  case SE_DERSettings: ds->derg = obj; break;
  case SE_DERStatus: ds->ders = obj; break;
  }
  free (buffer); parser_free (p); return;
}

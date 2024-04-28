open Utils

type t = {
  user_id : int32;
  client_random_value : string;
  encrypted_master_key : string;
  master_key_iv : string;
  encrypted_protection_key : string;
  protection_key_iv : string;
  exported_protection_key : string;
  encrypted_verification_key : string;
  verification_key_iv : string;
  exported_verification_key : string;
}

let create_table =
  [%rapper
    execute
      {sql|
CREATE TABLE IF NOT EXISTS secrets (
  user_id INT PRIMARY KEY REFERENCES users(id),

  client_random_value BYTEA NOT NULL,

  encrypted_master_key BYTEA NOT NULL,
  master_key_iv BYTEA NOT NULL,

  encrypted_protection_key BYTEA NOT NULL,
  protection_key_iv BYTEA NOT NULL,
  exported_protection_key BYTEA NOT NULL,

  encrypted_verification_key BYTEA NOT NULL,
  verification_key_iv BYTEA NOT NULL,
  exported_verification_key BYTEA NOT NULL
);   
|sql}]
    ()

let get_by_user_id =
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{user_id},

  @Base64Octets{client_random_value},

  @Base64Octets{encrypted_master_key},
  @Base64Octets{master_key_iv},

  @Base64Octets{encrypted_protection_key},
  @Base64Octets{protection_key_iv},
  @Base64Octets{exported_protection_key},

  @Base64Octets{encrypted_verification_key},
  @Base64Octets{verification_key_iv},
  @Base64Octets{exported_verification_key}
FROM
  secrets
WHERE
  user_id = %int32{user_id};
|sql}
      record_out]

let get_by_username =
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{user_id},

  @Base64Octets{client_random_value},

  @Base64Octets{encrypted_master_key},
  @Base64Octets{master_key_iv},

  @Base64Octets{encrypted_protection_key},
  @Base64Octets{protection_key_iv},
  @Base64Octets{exported_protection_key},

  @Base64Octets{encrypted_verification_key},
  @Base64Octets{verification_key_iv},
  @Base64Octets{exported_verification_key}
FROM
  secrets
JOIN
  users
ON
  users.id = secrets.user_id
WHERE
  username = %string{username};
|sql}
      record_out]

let insert =
  [%rapper
    execute
      {sql|
INSERT INTO secrets (
  user_id,

  client_random_value,

  encrypted_master_key,
  master_key_iv,

  encrypted_protection_key,
  protection_key_iv,
  exported_protection_key,

  encrypted_verification_key,
  verification_key_iv,
  exported_verification_key
)
VALUES(
  %int32{user_id},

  %Base64Octets{client_random_value},

  %Base64Octets{encrypted_master_key},
  %Base64Octets{master_key_iv},

  %Base64Octets{encrypted_protection_key},
  %Base64Octets{protection_key_iv},
  %Base64Octets{exported_protection_key},

  %Base64Octets{encrypted_verification_key},
  %Base64Octets{verification_key_iv},
  %Base64Octets{exported_verification_key}
);
|sql}]

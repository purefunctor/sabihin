open Utils

type t = {
  user_id : int32;
  encrypted_master_key : string;
  encrypted_protection_key : string;
  exported_protection_key : string;
  encrypted_verification_key : string;
  exported_verification_key : string;
}

let create_table =
  [%rapper
    execute
      {sql|
CREATE TABLE IF NOT EXISTS secrets (
  user_id INT PRIMARY KEY REFERENCES users(id),

  encrypted_master_key BYTEA NOT NULL,

  encrypted_protection_key BYTEA NOT NULL,
  exported_protection_key BYTEA NOT NULL,

  encrypted_verification_key BYTEA NOT NULL,
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

  @Base64Octets{encrypted_master_key},

  @Base64Octets{encrypted_protection_key},
  @Base64Octets{exported_protection_key},

  @Base64Octets{encrypted_verification_key},
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

  @Base64Octets{encrypted_master_key},

  @Base64Octets{encrypted_protection_key},
  @Base64Octets{exported_protection_key},

  @Base64Octets{encrypted_verification_key},
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

  encrypted_master_key,

  encrypted_protection_key,
  exported_protection_key,

  encrypted_verification_key,
  exported_verification_key
)
VALUES(
  %int32{user_id},

  %Base64Octets{encrypted_master_key},

  %Base64Octets{encrypted_protection_key},
  %Base64Octets{exported_protection_key},

  %Base64Octets{encrypted_verification_key},
  %Base64Octets{exported_verification_key}
);
|sql}]

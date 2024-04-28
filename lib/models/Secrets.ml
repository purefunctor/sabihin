type t = {
  user_id : int32;
  client_random_value : bytes;
  encrypted_master_key : bytes;
  master_key_iv : bytes;
  encrypted_protection_key : bytes;
  protection_key_iv : bytes;
  exported_protection_key : bytes;
  encrypted_verification_key : bytes;
  verification_key_iv : bytes;
  exported_verification_key : bytes;
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
  let open Utils in
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{user_id},

  @ByteOctets{client_random_value},

  @ByteOctets{encrypted_master_key},
  @ByteOctets{master_key_iv},

  @ByteOctets{encrypted_protection_key},
  @ByteOctets{protection_key_iv},
  @ByteOctets{exported_protection_key},

  @ByteOctets{encrypted_verification_key},
  @ByteOctets{verification_key_iv},
  @ByteOctets{exported_verification_key}
FROM
  secrets
WHERE
  user_id = %int32{user_id};
|sql}
      record_out]

let get_by_username =
  let open Utils in
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{user_id},

  @ByteOctets{client_random_value},

  @ByteOctets{encrypted_master_key},
  @ByteOctets{master_key_iv},

  @ByteOctets{encrypted_protection_key},
  @ByteOctets{protection_key_iv},
  @ByteOctets{exported_protection_key},

  @ByteOctets{encrypted_verification_key},
  @ByteOctets{verification_key_iv},
  @ByteOctets{exported_verification_key}
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
  let open Utils in
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

  %ByteOctets{client_random_value},

  %ByteOctets{encrypted_master_key},
  %ByteOctets{master_key_iv},

  %ByteOctets{encrypted_protection_key},
  %ByteOctets{protection_key_iv},
  %ByteOctets{exported_protection_key},

  %ByteOctets{encrypted_verification_key},
  %ByteOctets{verification_key_iv},
  %ByteOctets{exported_verification_key}
);
|sql}]

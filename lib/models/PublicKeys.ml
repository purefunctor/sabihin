type t = {
  user_id : int32;
  exported_protection_key : bytes;
  exported_verification_key : bytes;
}

let create_table =
  [%rapper
    execute
      {sql|
CREATE TABLE IF NOT EXISTS public_keys (
  user_id INT PRIMARY KEY REFERENCES users(id),
  exported_protection_key BYTEA NOT NULL,
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
  @ByteOctets{exported_protection_key},
  @ByteOctets{exported_verification_key}
FROM
  public_keys
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
  @ByteOctets{exported_protection_key},
  @ByteOctets{exported_verification_key}
FROM
  public_keys
JOIN
  users
ON
  users.id = public_keys.user_id
WHERE
  username = %string{username};
      |sql}
      record_out]

let get_by_public_id =
  let open Utils in
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{user_id},
  @ByteOctets{exported_protection_key},
  @ByteOctets{exported_verification_key}
FROM
  public_keys
JOIN
  users
ON
  users.id = public_keys.user_id
WHERE
  public_id = %string{public_id};
      |sql}
      record_out]

let insert =
  let open Utils in
  [%rapper
    execute
      {sql|
INSERT INTO public_keys (
  user_id,
  exported_protection_key,
  exported_verification_key
)
VALUES(
  %int32{user_id},
  %ByteOctets{exported_protection_key},
  %ByteOctets{exported_verification_key}
);
      |sql}]

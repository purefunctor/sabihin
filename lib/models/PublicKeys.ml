type t = { user_id : int32; protection_key : bytes; verification_key : bytes }

let create_table =
  [%rapper
    execute
      {sql|
CREATE TABLE IF NOT EXISTS public_keys (
  user_id INT PRIMARY KEY REFERENCES users(id),
  protection_key BYTEA NOT NULL,
  verification_key BYTEA NOT NULL
);
      |sql}]
    ()

let get_by_user_id =
  let open Utils in
  [%rapper
    get_one
      {sql|
SELECT
  @int32{user_id},
  @ByteOctets{protection_key},
  @ByteOctets{verification_key}
FROM
  public_keys
WHERE
  user_id = %int32{user_id};
      |sql}
      record_out]

let insert =
  let open Utils in
  [%rapper
    execute
      {sql|
INSERT INTO public_keys (
  user_id,
  protection_key,
  verification_key
)
VALUES(
  %int32{user_id},
  %ByteOctets{protection_key},
  %ByteOctets{verification_key}
);
      |sql}]

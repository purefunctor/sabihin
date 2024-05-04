open Utils

type t = {
  id : int32;
  public_id : string;
  username : string;
  auth_token : string;
  client_random : string;
}

let create_table =
  [%rapper
    execute
      {sql|
CREATE TABLE IF NOT EXISTS users (
  id SERIAL PRIMARY KEY,
  public_id VARCHAR(12) DEFAULT nanoid(12, '0123456789abcdefghijklmnopqrstuvwxyz'),
  username VARCHAR(16) UNIQUE NOT NULL,
  auth_token VARCHAR(128) NOT NULL,
  client_random BYTEA NOT NULL
);
      |sql}]
    ()

let get_by_id =
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{id},
  @string{public_id},
  @string{username},
  @string{auth_token},
  @Base64Octets{client_random}
FROM 
  users
WHERE 
  id = %int32{id};
      |sql}
      record_out]

let get_by_username =
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{id},
  @string{public_id},
  @string{username},
  @string{auth_token},
  @Base64Octets{client_random}
FROM
  users
WHERE 
  username = %string{username};
      |sql}
      record_out]

let insert =
  [%rapper
    get_one
      {sql|
INSERT INTO users (
  username, 
  auth_token,
  client_random
)
VALUES(
  %string{username}, 
  %string{auth_token},
  %Base64Octets{client_random}
)
RETURNING 
  @int32{id},
  @string{public_id}
      |sql}]

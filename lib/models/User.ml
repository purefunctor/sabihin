type t = { id : int32; username : string; auth_token : string }

let create_table =
  [%rapper
    execute
      {sql|
CREATE TABLE IF NOT EXISTS users (
  id SERIAL PRIMARY KEY,
  username VARCHAR(16) UNIQUE NOT NULL,
  auth_token VARCHAR(128) NOT NULL
);
      |sql}]
    ()

let get_by_id =
  [%rapper
    get_opt
      {sql|
SELECT
  @int32{id},
  @string{username},
  @string{auth_token}
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
  @string{username},
  @string{auth_token}
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
  auth_token
)
VALUES(
  %string{username}, 
  %string{auth_token}
)
RETURNING 
  @int32{id};
      |sql}]

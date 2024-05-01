open Mirage_crypto.Hash
open Vault_native

let cipher_secret = ref None
let set_cipher_secret value = cipher_secret := Some (Cstruct.of_string value)

let get_cipher_secret () =
  match !cipher_secret with
  | Some value -> value
  | None -> failwith "set_cipher_secret was not called."

let double_hmac value =
  let key = get_cipher_secret () in
  value |> Cstruct.of_string |> SHA512.hmac ~key |> SHA512.hmac ~key
  |> Cstruct.to_hex_string

let server_random = ref None
let set_server_random value = server_random := Some (Bytes.of_string value)

let get_server_random () =
  match !server_random with
  | Some value -> value
  | None -> failwith "set_server_random was not called."

let server_salt () = get_server_random () |> Salt.compute_digest

open Mirage_crypto.Hash

let current_key = ref None
let set_current_key key = current_key := Some (Cstruct.of_string key)

let get_current_key () =
  match !current_key with
  | Some current_key -> current_key
  | None -> failwith "set_current_key was not called."

let double_hmac value =
  let key = get_current_key () in
  value |> Cstruct.of_string |> SHA512.hmac ~key |> SHA512.hmac ~key
  |> Cstruct.to_hex_string

open Mirage_crypto.Hash

let compute_base (salt : Bytes.t) =
  if Bytes.length salt != 16 then
    invalid_arg
    @@ Printf.sprintf "expected 16 bytes got %i bytes" (Bytes.length salt);
  let salt_buffer = Buffer.create 256 in

  (* Encode first 10 bytes. *)
  Buffer.add_string salt_buffer "sabihin.ph";

  (* Encode next 230 bytes. *)
  for _ = 0 to 229 do
    Buffer.add_char salt_buffer 'L'
  done;

  (* Encode next 16 bytes. *)
  Buffer.add_bytes salt_buffer salt;

  Buffer.to_bytes salt_buffer

let compute_digest (salt : Bytes.t) =
  compute_base salt |> Cstruct.of_bytes |> SHA256.digest
  |> Cstruct.to_hex_string

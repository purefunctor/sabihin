open Mirage_crypto.Hash

type input = [ `String of String.t | `Bytes of Bytes.t ]

let compute_base (input : input) =
  let length =
    match input with
    | `String input -> String.length input
    | `Bytes input -> Bytes.length input
  in
  if length != 16 then
    invalid_arg @@ Printf.sprintf "expected length 16 got %i" length;
  let salt_buffer = Buffer.create 256 in

  (* Encode first 10 bytes. *)
  Buffer.add_string salt_buffer "sabihin.ph";

  (* Encode next 230 bytes. *)
  for _ = 0 to 229 do
    Buffer.add_char salt_buffer 'L'
  done;

  (* Encode next 16 bytes. *)
  (match input with
  | `String input -> Buffer.add_string salt_buffer input
  | `Bytes input -> Buffer.add_bytes salt_buffer input);

  Buffer.to_bytes salt_buffer

let compute_digest (input : input) =
  compute_base input |> Cstruct.of_bytes |> SHA256.digest

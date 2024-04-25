type t = Js.Typed_array.ArrayBuffer.t

let decode : string -> t =
 fun base64 ->
  let buffer = Base64_Uint8Array.decode base64 in
  Js.Typed_array.Uint8Array.buffer buffer

let encode : t -> string =
 fun buffer ->
  let buffer = Js.Typed_array.Uint8Array.fromBuffer buffer () in
  Base64_Uint8Array.encode buffer

let wrap = decode
let unwrap = encode

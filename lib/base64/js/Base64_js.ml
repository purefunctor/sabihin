open Js.Typed_array

module ArrayBuffer = struct
  type t = ArrayBuffer.t

  let decode : string -> t =
    [%mel.raw
      {|(base64) => {
          const binaryString = atob(base64);
          return Uint8Array.from(binaryString, (c) => c.codePointAt(0));
        }|}]

  let encode : t -> string =
    [%mel.raw
      {|(bytes) => {
          const binaryString = Array.from(bytes, (byte) =>
            String.fromCodePoint(byte)).join("");
          return btoa(binaryString);
        }|}]

  let wrap = decode
  let unwrap = encode
end

module Uint8Array = struct
  type t = Uint8Array.t

  let decode : string -> t =
   fun base64 ->
    let buffer = ArrayBuffer.decode base64 in
    Js.Typed_array.Uint8Array.fromBuffer buffer ()

  let encode : t -> string =
   fun buffer ->
    let buffer = Js.Typed_array.Uint8Array.buffer buffer in
    ArrayBuffer.encode buffer

  let wrap = decode
  let unwrap = encode
end

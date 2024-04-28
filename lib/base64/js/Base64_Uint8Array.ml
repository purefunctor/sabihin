type t = Js.Typed_array.Uint8Array.t

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

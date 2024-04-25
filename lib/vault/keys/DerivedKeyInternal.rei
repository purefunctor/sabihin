open Js.Typed_array;
open WebCrypto;

type t;

type freshKey = {
  derivedKey: t,
  hashedAuthenticationKey: ArrayBuffer.t,
  masterKeyIv: Uint8Array.t,
};

let create:
  (~password: string, ~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec freshKey = freshKey;

  let create:
    (~password: string, ~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);
};

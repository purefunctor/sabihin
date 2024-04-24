open Js.Typed_array;
open WebCrypto;

type t;
type masterKeyIv;

type freshKey = {
  derivedKey: t,
  hashedAuthenticationKey: ArrayBuffer.t,
  masterKeyIv,
};

let create:
  (~password: string, ~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec masterKeyIv = masterKeyIv;
  type nonrec freshKey = freshKey;

  let create:
    (~password: string, ~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);
};

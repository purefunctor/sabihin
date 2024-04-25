open Js.Typed_array;
open WebCrypto;

type t;

type freshKey = {
  masterKey: t,
  protectionKeyIv: Uint8Array.t,
  verificationKeyIv: Uint8Array.t,
};

let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec freshKey = freshKey;

  let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);
};

open Js.Typed_array;
open WebCrypto;

type t;
type protectionKeyIv;
type verificationKeyIv;

type freshKey = {
  masterKey: t,
  protectionKeyIv,
  verificationKeyIv,
};

let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec protectionKeyIv = protectionKeyIv;
  type nonrec verificationKeyIv = verificationKeyIv;
  type nonrec freshKey = freshKey;

  let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(freshKey);
};

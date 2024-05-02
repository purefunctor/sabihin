open Js.Typed_array;
open WebCrypto;

type t;

let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(t);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;

  let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(t);
};

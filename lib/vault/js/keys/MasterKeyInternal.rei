open Js.Typed_array;
open WebCrypto;

type t = pri cryptoKey;

let create: (~saltBuffer: ArrayBuffer.t) => Js.Promise.t(t);

let fromCryptoKey: cryptoKey => t;
let toCryptoKey: t => cryptoKey;

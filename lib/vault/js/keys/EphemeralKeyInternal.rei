open Js.Typed_array;
open WebCrypto;

type t = pri cryptoKey;

type freshKey = {
  ephemeralKey: t,
  messageIv: Uint8Array.t,
};

let create: unit => Js.Promise.t(freshKey);

let fromCryptoKey: cryptoKey => t;
let toCryptoKey: t => cryptoKey;

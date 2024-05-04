open Js.Typed_array;
open WebCrypto;

type t;

type freshKey = {
  ephemeralKey: t,
  messageIv: Uint8Array.t,
};

let create: unit => Js.Promise.t(freshKey);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec freshKey = freshKey;

  let create: unit => Js.Promise.t(freshKey);
};

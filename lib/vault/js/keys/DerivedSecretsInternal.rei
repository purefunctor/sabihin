open Js.Typed_array;
open WebCrypto;

type t;

type derivedSecrets = {
  derivedKey: t,
  authenticationKey: string,
  masterKeyIv: Uint8Array.t,
  protectionKeyIv: Uint8Array.t,
  verificationKeyIv: Uint8Array.t,
};

let create:
  (~password: string, ~saltBuffer: ArrayBuffer.t) =>
  Js.Promise.t(derivedSecrets);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec derivedSecrets = derivedSecrets;

  let create:
    (~password: string, ~saltBuffer: ArrayBuffer.t) =>
    Js.Promise.t(derivedSecrets);
};
